/*
 * PROJECT:    Mouri Internal Library Essentials
 * FILE:       IDesktopWindowXamlSourceNative2.cs
 * PURPOSE:    Definition for Mile.Xaml.Interop.IDesktopWindowXamlSourceNative2
 *
 * LICENSE:    The MIT License
 *
 * MAINTAINER: MouriNaruto (Kenji.Mouri@outlook.com)
 */

using System;
using System.Runtime.InteropServices;
#if NET8_0_OR_GREATER
using System.Runtime.InteropServices.Marshalling;
#endif

namespace Mile.Xaml.Interop
{
#if NET8_0_OR_GREATER
    [GeneratedComInterface]
#else
    [ComImport]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
#endif
    [Guid("e3dcd8c7-3057-4692-99c3-7b7720afda31")]
    public partial interface IDesktopWindowXamlSourceNative2
    {
        void AttachToWindow(IntPtr parentWnd);

        // Original declaration: IntPtr WindowHandle { get; }
        IntPtr GetWindowHandle();

        [return:MarshalAs(UnmanagedType.I1)]
        bool PreTranslateMessage(ref System.Windows.Forms.Message message);
    }
}
