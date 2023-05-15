/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      InteropExtensions.cs
 * PURPOSE:   Definition for Mile.Xaml.Interop.InteropExtensions
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

using System.Runtime.InteropServices;
using System;
using Windows.UI.Core;
using Windows.UI.Xaml.Hosting;

namespace Mile.Xaml.Interop
{
    public static class InteropExtensions
    {
        public static IDesktopWindowXamlSourceNative2 GetInterop(
            this DesktopWindowXamlSource BaseObject)
        {
            IntPtr BaseObjectIntPtr = Marshal.GetIUnknownForObject(BaseObject);
            try
            {
                object TypedObject = Marshal.GetTypedObjectForIUnknown(
                    BaseObjectIntPtr,
                    typeof(IDesktopWindowXamlSourceNative2));
                return (IDesktopWindowXamlSourceNative2)TypedObject;
            }
            finally
            {
                Marshal.Release(BaseObjectIntPtr);
            }
        }

        public static ICoreWindowInterop GetInterop(
            this CoreWindow BaseObject)
        {
            IntPtr BaseObjectIntPtr = Marshal.GetIUnknownForObject(BaseObject);
            try
            {
                object TypedObject = Marshal.GetTypedObjectForIUnknown(
                    BaseObjectIntPtr,
                    typeof(ICoreWindowInterop));
                return (ICoreWindowInterop)TypedObject;
            }
            finally
            {
                Marshal.Release(BaseObjectIntPtr);
            }
        }
    }
}
