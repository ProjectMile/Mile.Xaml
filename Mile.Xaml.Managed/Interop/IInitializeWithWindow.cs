/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      IInitializeWithWindow.cs
 * PURPOSE:   Definition for Mile.Xaml.Interop.IInitializeWithWindow
 *
 * LICENSE:   The MIT License
 *
 * MAINTAINER: MouriNaruto (Kenji.Mouri@outlook.com)
 */

using System;
using System.Runtime.InteropServices;

namespace Mile.Xaml.Interop
{
    [ComImport]
    [Guid("3E68D4BD-7135-4D10-8018-9FB6D9F33FA1")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface IInitializeWithWindow
    {
        void Initialize(IntPtr hwnd);
    }
}
