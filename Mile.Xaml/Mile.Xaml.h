/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      Mile.Xaml.h
 * PURPOSE:   Definition for Mile.Xaml Public Interfaces
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
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
