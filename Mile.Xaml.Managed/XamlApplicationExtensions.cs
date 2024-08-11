/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      XamlApplicationExtensions.cs
 * PURPOSE:   Definition for Mile.Xaml.XamlApplicationExtensions
 *
 * LICENSE:   The MIT License
 *
 * MAINTAINER: MouriNaruto (Kenji.Mouri@outlook.com)
 */

using Mile.Xaml.Interop;
using System;
using System.Runtime.InteropServices;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Hosting;

namespace Mile.Xaml
{
    public static partial class XamlApplicationExtensions
    {
        private const uint PM_NOREMOVE = 0x0000;
        private const uint PM_REMOVE = 0x0001;
        private const uint PM_NOYIELD = 0x0002;

#if NET8_0_OR_GREATER
        [LibraryImport("user32.dll", EntryPoint = "PeekMessageW", SetLastError = true, StringMarshalling = StringMarshalling.Utf16)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static partial bool PeekMessage(
            out System.Windows.Forms.Message lpMsg,
            IntPtr hWnd,
            uint wMsgFilterMin,
            uint wMsgFilterMax,
            uint wRemoveMsg);
#else
        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool PeekMessage(
            out System.Windows.Forms.Message lpMsg,
            IntPtr hWnd,
            uint wMsgFilterMin,
            uint wMsgFilterMax,
            uint wRemoveMsg);
#endif

#if NET8_0_OR_GREATER
        [LibraryImport("user32.dll", EntryPoint = "DispatchMessageW", SetLastError = true, StringMarshalling = StringMarshalling.Utf16)]
        private static partial IntPtr DispatchMessage(
            ref System.Windows.Forms.Message lpMsg);
#else
        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        private static extern IntPtr DispatchMessage(
            ref System.Windows.Forms.Message lpMsg);
#endif


        [ThreadStatic]
        private static WindowsXamlManager XamlManager = null;

        [ThreadStatic]
        private static System.Windows.Forms.Form CoreWindowHostWindow = null;

        public static void ThreadInitialize(
            this Application BaseObject)
        {
            if (XamlManager == null)
            {
                XamlManager = WindowsXamlManager.InitializeForCurrentThread();
            }

            BaseObject.SetTransparentBackgroundAttribute(true);

            // Prevent showing the dummy/empty/ghost DesktopWindowXamlSource window
            // in the task bar.
            // Reference: https://github.com/microsoft/terminal/issues/6507
            // Reference: https://github.com/microsoft/microsoft-ui-xaml/issues/6490
            // Fixes: https://github.com/M2Team/NanaZip/issues/225
            // For also fixing the window with empty content due to CoreWindow is
            // not exist issue, create a host window without message loop is a
            // better workaround.
            if (CoreWindowHostWindow == null)
            {
                CoreWindowHostWindow = new CoreWindowHost();
            }
        }

        public static void ThreadUninitialize(
            this Application BaseObject)
        {
            if (XamlManager != null)
            {
                XamlManager.Dispose();
                XamlManager = null;
            }

            System.Windows.Forms.Message Message;
            if (PeekMessage(out Message, IntPtr.Zero, 0, 0, PM_REMOVE))
            {
                DispatchMessage(ref Message);
            }
        }

        public static bool GetTransparentBackgroundAttribute(
            this Application BaseObject)
        {
            return Window.Current.GetInterop().GetTransparentBackground();
        }

        public static void SetTransparentBackgroundAttribute(
            this Application BaseObject,
            bool AttributeValue)
        {
            Window.Current.GetInterop().SetTransparentBackground(AttributeValue);
        }
    }
}
