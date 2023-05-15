/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      ICoreWindowInterop.cs
 * PURPOSE:   Definition for Mile.Xaml.Interop.ICoreWindowInterop
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

using System;
using System.Runtime.InteropServices;

namespace Mile.Xaml.Interop
{
    [ComImport]
    [Guid("45D64A29-A63E-4CB6-B498-5781D298CB4F")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface ICoreWindowInterop
    {
        IntPtr WindowHandle { get; }
        bool MessageHandled { set; }
    }
}
