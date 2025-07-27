/*
 * PROJECT:    Mouri Internal Library Essentials
 * FILE:       ICoreWindowInterop.cs
 * PURPOSE:    Definition for Mile.Xaml.Interop.ICoreWindowInterop
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
    [Guid("45D64A29-A63E-4CB6-B498-5781D298CB4F")]
    public partial interface ICoreWindowInterop
    {
        // Original declaration: IntPtr WindowHandle { get; }
        IntPtr GetWindowHandle();

        // Original declaration: bool MessageHandled { set; }
        void SetMessageHandled([MarshalAs(UnmanagedType.I1)]bool messageHandled);
    }
}
