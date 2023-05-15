// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using Windows.Foundation.Metadata;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Hosting;
using Mile.Xaml.Interop;

namespace Mile.Xaml
{
    /// <summary>
    ///     WindowsXamlHostBase hosts UWP XAML content inside Windows Forms
    /// </summary>
    [System.ComponentModel.DesignerCategory("code")]
    public abstract partial class WindowsXamlHostBase : ContainerControl
    {
        /// <summary>
        /// DesktopWindowXamlSource instance
        /// </summary>
        private readonly DesktopWindowXamlSource _xamlSource;

        /// <summary>
        /// Private field that backs ChildInternal property.
        /// </summary>
        private UIElement _childInternal;

        /// <summary>
        ///    Last preferredSize returned by UWP XAML during WinForms layout pass
        /// </summary>
        private Size _lastXamlContentPreferredSize;

        /// <summary>
        ///    UWP XAML island window handle associated with this Control instance
        /// </summary>
        private IntPtr _xamlIslandWindowHandle = IntPtr.Zero;

        /// <summary>
        ///    The last dpi value retrieved from the system
        /// </summary>
        private double _lastDpi = 96.0f;

        /// <summary>
        ///     Fired when XAML content has been updated
        /// </summary>
        [Browsable(true)]
        [Category("UWP XAML")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Visible)]
        [Description("Fired when UWP XAML content has been updated")]
        public event EventHandler ChildChanged;

        /// <summary>
        /// Initializes a new instance of the <see cref="WindowsXamlHostBase"/> class.
        /// </summary>
        public WindowsXamlHostBase()
        {
            // Return immediately if control is instantiated by the Visual Studio Designer
            // https://stackoverflow.com/questions/1166226/detecting-design-mode-from-a-controls-constructor
            if (LicenseManager.UsageMode == LicenseUsageMode.Designtime)
            {
                return;
            }

            SetStyle(ControlStyles.ContainerControl, true);
            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.AllPaintingInWmPaint, true);

            // Must be a container control with TabStop == false to allow nested UWP XAML Focus
            // BUGBUG: Uncomment when nested Focus is available
            // TabStop = false;

            // Respond to size changes on this Control
            SizeChanged += OnWindowXamlHostSizeChanged;

            // Create DesktopWindowXamlSource, host for UWP XAML content
            _xamlSource = new DesktopWindowXamlSource();

            // Hook up method for DesktopWindowXamlSource Focus handling
            _xamlSource.TakeFocusRequested += this.OnTakeFocusRequested;
        }

        protected WindowsXamlHostBase(string typeName)
            : this()
        {
            if (LicenseManager.UsageMode == LicenseUsageMode.Runtime)
            {
                ChildInternal = UWPTypeFactory.CreateXamlContentByType(typeName);
                ChildInternal.SetWrapper(this);
            }
        }

        /// <summary>
        /// Exposes ChildInternal without exposing its actual Type.
        /// </summary>
        /// <returns>the underlying UWP child object</returns>
        public object GetUwpInternalObject()
        {
            return ChildInternal;
        }

        /// <summary>
        ///    Gets or sets XAML content for XamlContentHost
        /// </summary>
        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        protected UIElement ChildInternal
        {
            get => _childInternal;

            set
            {
                if (!DesignMode)
                {
                    if (value == ChildInternal)
                    {
                        return;
                    }

                    var newFrameworkElement = value as FrameworkElement;
                    var oldFrameworkElement = ChildInternal as FrameworkElement;

                    if (oldFrameworkElement != null)
                    {
                        oldFrameworkElement.SizeChanged -= OnChildSizeChanged;
                    }

                    _childInternal = value;
                    SetContent(value);

                    if (newFrameworkElement != null)
                    {
                        // If XAML content has changed, check XAML size and WindowsXamlHost.AutoSize
                        // setting to determine if WindowsXamlHost needs to re-run layout.
                        newFrameworkElement.SizeChanged += OnChildSizeChanged;
                    }

                    PerformLayout();

                    ChildChanged?.Invoke(this, new EventArgs());
                }
                else
                {
                    _childInternal = value;
                }
            }
        }

        /// <summary>
        /// Sets the root UWP XAML element on DesktopWindowXamlSource
        /// </summary>
        /// <param name="newValue">A UWP XAML Framework element</param>
        protected virtual void SetContent(UIElement newValue)
        {
            if (_xamlSource != null)
            {
                _xamlSource.Content = newValue;
            }
        }

        /// <summary>
        /// Gets or sets a value indicating whether a render transform is added to the UWP control corresponding to the current dpi scaling factor
        /// if scaling is not supported natively by the XAML island
        /// </summary>
        /// <value>The dpi scaling mode.</value>
        /// <remarks>A custom render transform added to the root UWP control will be overwritten.</remarks>
        [ReadOnly(false)]
        [Browsable(true)]
        [DefaultValue(false)]
        [Category("Layout")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Visible)]
        [Deprecated("Custom scaling transform no longer required to handle Dpi", DeprecationType.Deprecate, 0)]
        public bool DpiScalingRenderTransform
        {
            get => false;
            set
            {
            }
        }

        /// <summary>
        /// Clean up hosted UWP XAML content
        /// </summary>
        /// <param name="disposing">IsDisposing?</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                SizeChanged -= OnWindowXamlHostSizeChanged;

                var oldFrameworkElement = ChildInternal as FrameworkElement;
                if (oldFrameworkElement != null)
                {
                    oldFrameworkElement.SizeChanged -= OnChildSizeChanged;
                }

                ChildInternal?.ClearWrapper();

                this._childInternal = null;

                // Required by CA2213: _xamlSource?.Dispose() is insufficient.
                if (_xamlSource != null)
                {
                    _xamlSource.TakeFocusRequested -= OnTakeFocusRequested;
                    _xamlSource.Dispose();
                }
            }

            base.Dispose(disposing);
        }

        /// <summary>
        /// Raises the HandleCreated event.  Assign window render target to UWP XAML content.
        /// </summary>
        /// <param name="e">EventArgs</param>
        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);

            if (!DesignMode)
            {
                // Attach window to DesktopWindowXamSource as a render target
                var XamlSourceNative = _xamlSource.GetInterop();
                XamlSourceNative.AttachToWindow(Handle);
                _xamlIslandWindowHandle = XamlSourceNative.WindowHandle;

                // Set window style required by container control to support Focus
                if (Interop.UnsafeNativeMethods.SetWindowLong(Handle, Interop.NativeDefines.GWL_EXSTYLE, Interop.NativeDefines.WS_EX_CONTROLPARENT) == 0)
                {
                    throw new InvalidOperationException("WindowsXamlHostBase::OnHandleCreated: Failed to set WS_EX_CONTROLPARENT on control window.");
                }

                if (ChildInternal != null)
                {
                    SetContent(ChildInternal);
                    ChildInternal.SetWrapper(this);
                }

                UpdateDpiScalingFactor();
            }
        }

        /// <summary>
        /// Override that disposes the current instance when the parent handle has been destroyed
        /// </summary>
        /// <param name="e">Ignored</param>
        protected override void OnHandleDestroyed(EventArgs e)
        {
            this.Dispose(true);
            base.OnHandleDestroyed(e);
        }
    }
}
