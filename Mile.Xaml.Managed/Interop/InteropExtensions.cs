/*
 * PROJECT:    Mouri Internal Library Essentials
 * FILE:       InteropExtensions.cs
 * PURPOSE:    Definition for Mile.Xaml.Interop.InteropExtensions
 *
 * LICENSE:    The MIT License
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
#if NET8_0_OR_GREATER
            IDesktopWindowXamlSourceNative2 InteropObject = (IDesktopWindowXamlSourceNative2)(object)BaseObject;
            return InteropObject;
#else
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
#endif
        }

        public static ICoreWindowInterop GetInterop(
            this CoreWindow BaseObject)
        {
#if NET8_0_OR_GREATER
            ICoreWindowInterop InteropObject = (ICoreWindowInterop)(object)BaseObject;
            return InteropObject;
#else
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
#endif
        }

        public static IWindowPrivate GetInterop(this Window BaseObject)
        {
#if NET8_0_OR_GREATER
            IWindowPrivate InteropObject = (IWindowPrivate)(object)BaseObject;
            return InteropObject;
#else
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
#endif
        }
    }
}
