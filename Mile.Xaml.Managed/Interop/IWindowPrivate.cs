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

namespace Mile.Xaml.Interop
{
    [ComImport]
    [Guid("06636C29-5A17-458D-8EA2-2422D997A922")]
    [InterfaceType(ComInterfaceType.InterfaceIsIInspectable)]
    public interface IWindowPrivate
    {
        bool TransparentBackground { get; set; }
    }
}
