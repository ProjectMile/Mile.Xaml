/*
 * PROJECT:    Mouri Internal Library Essentials
 * FILE:       IInitializeWithWindow.cs
 * PURPOSE:    Definition for Mile.Xaml.Interop.IInitializeWithWindow
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
    [Guid("3E68D4BD-7135-4D10-8018-9FB6D9F33FA1")]
    public partial interface IInitializeWithWindow
    {
        void Initialize(IntPtr hwnd);
    }
}
