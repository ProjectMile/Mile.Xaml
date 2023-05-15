/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      XamlApplicationExtensions.cs
 * PURPOSE:   Definition for Mile.Xaml.XamlApplicationExtensions
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

using System;
using System.Runtime.InteropServices;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Hosting;

namespace Mile.Xaml
{
    public static class XamlApplicationExtensions
    {
        private const uint PM_NOREMOVE = 0x0000;
        private const uint PM_REMOVE = 0x0001;
        private const uint PM_NOYIELD = 0x0002;

        [DllImport("user32.dll", CharSet = CharSet.Unicode)]
        [return: MarshalAs(UnmanagedType.Bool)]
        private static extern bool PeekMessage(
            out System.Windows.Forms.Message lpMsg,
            IntPtr hWnd,
            uint wMsgFilterMin,
            uint wMsgFilterMax,
            uint wRemoveMsg);

        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Unicode)]
        private static extern IntPtr DispatchMessage(
            ref System.Windows.Forms.Message lpMsg);

        [ThreadStatic]
        private static WindowsXamlManager XamlManager = null;

        public static void ThreadInitialize(
            this Application BaseObject)
        {
            if (XamlManager == null)
            {
                XamlManager = WindowsXamlManager.InitializeForCurrentThread();
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
    }
}
