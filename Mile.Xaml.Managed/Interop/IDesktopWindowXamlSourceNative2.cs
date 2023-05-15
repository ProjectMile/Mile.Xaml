/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      IDesktopWindowXamlSourceNative2.cs
 * PURPOSE:   Definition for Mile.Xaml.Interop.IDesktopWindowXamlSourceNative2
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
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    [Guid("e3dcd8c7-3057-4692-99c3-7b7720afda31")]
    public interface IDesktopWindowXamlSourceNative2
    {
        void AttachToWindow(IntPtr parentWnd);

        IntPtr WindowHandle { get; }

        bool PreTranslateMessage(ref System.Windows.Forms.Message message);
    }
}
