/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      Mile.Xaml.h
 * PURPOSE:   Definition for Mile.Xaml Public Interfaces
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: MouriNaruto (KurikoMouri@outlook.jp)
 */

#pragma once

#ifndef MILE_XAML
#define MILE_XAML

#include <Windows.h>

/**
 * @brief Retrieves the handle to the CoreWindow compatibility window.
 * @return The handle to the CoreWindow compatibility window.
*/
EXTERN_C HWND WINAPI MileXamlGetCoreWindowHandle();

/**
 * @brief Sets the XAML content for the Mile.Xaml.ContentWindow class window.
 * @param WindowHandle The handle to the Mile.Xaml.ContentWindow class window
 *                     for which the XAML content is to be set.
 * @param XamlContent The XAML content which is derived from
 *                    Windows.UI.Xaml.FrameworkElement.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
EXTERN_C HRESULT WINAPI MileXamlSetXamlContentForContentWindow(
    _In_ HWND WindowHandle,
    _In_opt_ LPVOID XamlContent);

/**
 * @brief The default window callback function for Mile.Xaml.ContentWindow
 *        window class.
 * @param hWnd A handle to the window.
 * @param uMsg The message.
 * @param wParam Additional message information. The contents of the wParam
 *               parameter depend on the value of the uMsg parameter.
 * @param lParam Additional message information. The contents of the lParam
 *               parameter depend on the value of the uMsg parameter.
 * @return The return value is the result of the message processing, and
 *         depends on the message sent.
*/
EXTERN_C LRESULT CALLBACK MileXamlContentWindowDefaultCallback(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam);

/**
 * @brief Retrieves the transparent background attribute for contents hosted
 *        via XAML Islands.
 * @param TransparentBackground TRUE to use transparent background, FALSE to
 *                              use opaque background.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
EXTERN_C HRESULT WINAPI MileXamlGetTransparentBackgroundAttribute(
    _Out_ PBOOLEAN TransparentBackground);

/**
 * @brief Sets the transparent background attribute for contents hosted via
 *        XAML Islands.
 * @param TransparentBackground TRUE to use transparent background, FALSE to
 *                              use opaque background.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
EXTERN_C HRESULT WINAPI MileXamlSetTransparentBackgroundAttribute(
    _In_ BOOLEAN TransparentBackground);

/**
 * @brief Retrieves the preferred dark mode attribute for application.
 * @param PreferredDarkMode TRUE to preferred dark mode if available, FALSE to
 *                          preferred the default light mode.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
EXTERN_C HRESULT WINAPI MileXamlGetPreferredDarkModeAttribute(
    _Out_ PBOOLEAN PreferredDarkMode);

/**
 * @brief Sets the preferred dark mode attribute for application.
 * @param PreferredDarkMode TRUE to preferred dark mode if available, FALSE to
 *                          preferred the default light mode.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
EXTERN_C HRESULT WINAPI MileXamlSetPreferredDarkModeAttribute(
    _In_ BOOLEAN PreferredDarkMode);

/**
 * @brief Initialize Mile.Xaml for non-main thread.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
EXTERN_C HRESULT WINAPI MileXamlThreadInitialize();

/**
 * @brief Uninitialize Mile.Xaml for non-main thread.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
EXTERN_C HRESULT WINAPI MileXamlThreadUninitialize();

/**
 * @brief Initialize Mile.Xaml for main thread.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
EXTERN_C HRESULT WINAPI MileXamlGlobalInitialize();

/**
 * @brief Uninitialize Mile.Xaml for main thread.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
EXTERN_C HRESULT WINAPI MileXamlGlobalUninitialize();

#endif // !MILE_XAML
