/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      IWindowPrivate.cs
 * PURPOSE:   Definition for Mile.Xaml.Interop.IWindowPrivate
 *
 * LICENSE:   The MIT License
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
    [Guid("06636C29-5A17-458D-8EA2-2422D997A922")]
    public partial interface IWindowPrivate
    {
        void GetIids(out int iidCount, out IntPtr iids);
        void GetRuntimeClassName(out IntPtr className);
        void GetTrustLevel(out int trustLevel);
        [return: MarshalAs(UnmanagedType.I1)] bool GetTransparentBackground();
        void SetTransparentBackground([MarshalAs(UnmanagedType.I1)] bool transparentBackground);
    }
}
