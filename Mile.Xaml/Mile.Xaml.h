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
 * @brief Flags for specifying the system-drawn backdrop material of a window,
 *        including behind the non-client area.
*/
typedef enum MILE_XAML_SYSTEM_BACKDROP_TYPE
{
    /**
     * @brief The default. Let the Desktop Window Manager (DWM) automatically
              decide the system-drawn backdrop material for this window.
    */
    MILE_XAML_SYSTEM_BACKDROP_TYPE_AUTO = 0,

    /**
     * @brief Don't draw any system backdrop.
    */
    MILE_XAML_SYSTEM_BACKDROP_TYPE_NONE = 1,

    /**
     * @brief Draw the backdrop material effect corresponding to a long-lived
     *        window.
    */
    MILE_XAML_SYSTEM_BACKDROP_TYPE_MICA = 2,

    /**
     * @brief Draw the backdrop material effect corresponding to a transient
     *        window.
    */
    MILE_XAML_SYSTEM_BACKDROP_TYPE_ACRYLIC = 3,

    /**
     * @brief Draw the backdrop material effect corresponding to a window with
     *        a tabbed title bar.
    */
    MILE_XAML_SYSTEM_BACKDROP_TYPE_MICA_ALT = 4

} MILE_XAML_SYSTEM_BACKDROP_TYPE;

#define MILE_XAML_GET_SYSTEM_BACKDROP_TYPE (WM_USER + 1)
#define MILE_XAML_SET_SYSTEM_BACKDROP_TYPE (WM_USER + 2)

#define MILE_XAML_SET_IMMERSIVE_DARK_MODE (WM_USER + 3)
#define MILE_XAML_GET_IMMERSIVE_DARK_MODE (WM_USER + 4)

#endif // !MILE_XAML
