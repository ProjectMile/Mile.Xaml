﻿// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.ComponentModel;
using System.Windows.Forms;
using Windows.UI.Xaml;

namespace Mile.Xaml
{
    /// <summary>
    ///     A sample Windows Forms control that hosts XAML content
    /// </summary>
    [DesignerCategory("code")]
    public class WindowsXamlHost : WindowsXamlHostBase
    {
        /// <summary>
        /// Gets or sets a value indicating whether the control dynamically sizes to its content
        /// </summary>
        [ReadOnly(false)]
        [Browsable(true)]
        [DefaultValue(false)]
        [Category("Layout")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Visible)]
        public override bool AutoSize
        {
            get => base.AutoSize;

            set => base.AutoSize = value;
        }

        /// <summary>
        /// Gets or sets the automatic size mode.
        /// </summary>
        /// <value>The automatic size mode.</value>
        /// <remarks>A value indicating if the control dynamically sizes to its content.</remarks>
        [ReadOnly(false)]
        [Browsable(true)]
        [Category("Layout")]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Visible)]
#pragma warning disable CA1721 // Property names should not match get methods
        public AutoSizeMode AutoSizeMode
#pragma warning restore CA1721 // Property names should not match get methods
        {
            get => GetAutoSizeMode();

            set => SetAutoSizeMode(value);
        }

        /// <summary>
        /// Gets or sets XAML Content by type name
        /// </summary>
        /// <example><code>XamlClassLibrary.MyUserControl</code></example>
        /// <remarks>
        /// Content creation is deferred until after the parent hwnd has been created.
        /// </remarks>
        [Browsable(true)]
        [Category("XAML")]
        public string InitialTypeName
        {
            get;
            set;
        }

        /// <summary>
        /// Gets or sets XAML content for XamlContentHost
        /// </summary>
        /// <value>The <see cref="UIElement"/>.</value>
        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public UIElement Child
        {
            get => ChildInternal;

            set => ChildInternal = value;
        }

        /// <summary>
        /// Raises the <see cref="Control.HandleCreated" /> event.
        /// </summary>
        /// <param name="e">An <see cref="EventArgs" /> that contains the event data.</param>
        /// <remarks>Assign window render target to UWP XAML content.</remarks>
        protected override void OnHandleCreated(EventArgs e)
        {
            // Create content if TypeName has been set and xamlRoot has not been set
            if (!DesignMode && !string.IsNullOrEmpty(InitialTypeName) && Child == null)
            {
                Child = UWPTypeFactory.CreateXamlContentByType(InitialTypeName);
            }

            base.OnHandleCreated(e);
        }
    }
}
