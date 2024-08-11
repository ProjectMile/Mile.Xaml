// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

using System;
using System.Runtime.InteropServices;

namespace Mile.Xaml.Interop
{
    /// <summary>
    /// This class is for methods that are potentially dangerous. Any caller of these methods must perform a full security review to make sure that the usage is secure because no stack walk will be performed.
    /// </summary>
    internal static partial class UnsafeNativeMethods
    {
        /// <summary>
        /// This code returns a pointer to a native control with focus.
        /// </summary>
        /// <SecurityNote>
        ///  SecurityCritical: This code happens to return a critical resource and causes unmanaged code elevation
        /// </SecurityNote>
        /// <returns>handle</returns>
#if NET8_0_OR_GREATER
        [LibraryImport(ExternDll.User32, SetLastError = true, EntryPoint = "SetFocus")]
        internal static partial IntPtr IntSetFocus(IntPtr hWnd);
#else
        [DllImport(ExternDll.User32, EntryPoint = "SetFocus", ExactSpelling = true, CharSet = CharSet.Auto, SetLastError = true)]
        internal static extern IntPtr IntSetFocus(IntPtr hWnd);
#endif

        /// <summary>
        /// Changes an attribute of the specified window. The function also sets the 32-bit (long) value at the specified offset into the extra window memory.
        /// </summary>
        /// <param name="hWnd">Target window</param>
        /// <param name="nIndex">Zero-based offset</param>
        /// <param name="dwNewLong">The replacement value</param>
        /// <returns>A positive integer indicates success; zero indicates failure</returns>
#if NET8_0_OR_GREATER
        [LibraryImport(ExternDll.User32, SetLastError = true, EntryPoint = "SetWindowLongW", StringMarshalling = StringMarshalling.Utf16)]
        internal static partial int SetWindowLong(IntPtr hWnd, int nIndex, uint dwNewLong);
#else
        [DllImport(ExternDll.User32, SetLastError = true)]
        internal static extern int SetWindowLong(IntPtr hWnd, int nIndex, uint dwNewLong);
#endif
    }
}
