/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      InteropExtensions.cs
 * PURPOSE:   Definition for Mile.Xaml.Interop.InteropExtensions
 *
 * LICENSE:   The MIT License
 *
 * MAINTAINER: MouriNaruto (Kenji.Mouri@outlook.com)
 */

using System.Runtime.InteropServices;
using System;
using Windows.UI.Core;
using Windows.UI.Xaml.Hosting;
using Windows.UI.Xaml;

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

        public static IWindowPrivate GetInterop(this Window BaseObject)
        {
            IntPtr BaseObjectIntPtr = Marshal.GetIUnknownForObject(BaseObject);
            try
            {
                object TypedObject = Marshal.GetTypedObjectForIUnknown(
                    BaseObjectIntPtr,
                    typeof(IWindowPrivate));
                return (IWindowPrivate)TypedObject;
            }
            finally
            {
                Marshal.Release(BaseObjectIntPtr);
            }
        }
    }
}
