/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      Mile.Xaml.cpp
 * PURPOSE:   Implementation for Mile.Xaml Public Interfaces
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include "Mile.Xaml.h"

// https://github.com/microsoft/terminal/blob
// /c727762602b8bd12e4a3a769053204d7e92b81c5
// /src/cascadia/WindowsTerminalUniversal/pch.h#L12
// This is inexplicable, but for whatever reason, cppwinrt conflicts with the
// SDK definition of this function, so the only fix is to undef it.
// from WinBase.h
// Windows::UI::Xaml::Media::Animation::IStoryboard::GetCurrentTime
#ifdef GetCurrentTime
#undef GetCurrentTime
#endif

#include <WinUser.h>

// To enable support for non-WinRT interfaces, unknwn.h must be included before
// any C++/WinRT headers.
#include <unknwn.h>

#include <winrt/Windows.Foundation.h>

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Media.h>

#include <CoreWindow.h>
#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>

#include <Mile.Helpers.h>

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#include "IWindowPrivate.h"

namespace winrt
{
    using Windows::Foundation::Uri;
    using Windows::UI::Core::CoreWindow;
    using Windows::UI::Xaml::ElementTheme;
    using Windows::UI::Xaml::FrameworkElement;
    using Windows::UI::Xaml::Input::KeyboardNavigationMode;
    using Windows::UI::Xaml::ResourceDictionary;
    using Windows::UI::Xaml::Window;
    using Windows::UI::Xaml::Controls::Control;
    using Windows::UI::Xaml::Controls::Grid;
    using Windows::UI::Xaml::Hosting::DesktopWindowXamlSource;
    using Windows::UI::Xaml::Hosting::WindowsXamlManager;
    using Windows::UI::Xaml::Media::VisualTreeHelper;
}

namespace
{
    static bool volatile g_PreferredDarkMode = false;
    static bool volatile g_IsGlobalUninitialized = false;

    thread_local winrt::WindowsXamlManager g_WindowsXamlManager = nullptr;
    thread_local HWND volatile g_CoreWindowHostWindowHandle = nullptr;
    thread_local HWND volatile g_CoreWindowHandle = nullptr;
}

EXTERN_C HWND WINAPI MileXamlGetCoreWindowHandle()
{
    return g_CoreWindowHandle;
}

EXTERN_C LRESULT CALLBACK MileXamlContentWindowDefaultCallback(
    _In_ HWND hWnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        // Note: Return -1 directly because WM_DESTROY message will be sent
        // when destroy the window automatically. We free the resource when
        // processing the WM_DESTROY message of this window.

        LPCREATESTRUCT CreateStruct =
            reinterpret_cast<LPCREATESTRUCT>(lParam);

        winrt::DesktopWindowXamlSource XamlSource;

        winrt::com_ptr<IDesktopWindowXamlSourceNative> XamlSourceNative =
            XamlSource.as<IDesktopWindowXamlSourceNative>();

        // Parent the DesktopWindowXamlSource object to current window.
        if (FAILED(XamlSourceNative->AttachToWindow(hWnd)))
        {
            return -1;
        }

        winrt::FrameworkElement Content = nullptr;
        winrt::copy_from_abi(Content, CreateStruct->lpCreateParams);
        if (Content)
        {
            XamlSource.Content(Content);
            // Focus returns to the first or the last keyboard navigation
            // stop inside of a container when the first or last keyboard
            // navigation stop is reached.
            XamlSource.Content().TabFocusNavigation(
                winrt::KeyboardNavigationMode::Cycle);
        }

        HWND XamlWindowHandle = nullptr;
        if (FAILED(XamlSourceNative->get_WindowHandle(&XamlWindowHandle)))
        {
            return -1;
        }

        if (!::SetPropW(
            hWnd,
            L"XamlWindowSource",
            reinterpret_cast<HANDLE>(winrt::detach_abi(XamlSource))))
        {
            return -1;
        }

        // Focus on XAML Island host window for Acrylic brush support.
        ::SetFocus(XamlWindowHandle);

        ::MileSetWindowSystemBackdropTypeAttribute(
            hWnd,
            MILE_WINDOW_SYSTEM_BACKDROP_TYPE_MICA);

        BOOL UseImmersiveDarkMode = (
            Content && Content.ActualTheme() == winrt::ElementTheme::Dark
            ? TRUE
            : FALSE);

        if (S_OK == ::MileEnableImmersiveDarkModeForWindow(
            hWnd,
            UseImmersiveDarkMode))
        {
            MARGINS Margins = { -1 };
            ::DwmExtendFrameIntoClientArea(hWnd, &Margins);
        }
        else
        {
            if (!::SetPropW(
                hWnd,
                L"BackgroundFallbackColor",
                ULongToHandle(::MileGetDefaultBackgroundColorValue(
                    UseImmersiveDarkMode))))
            {
                return -1;
            }
        }

        return 0;
    }
    case WM_SETFOCUS:
    {
        winrt::DesktopWindowXamlSource XamlSource = nullptr;
        winrt::copy_from_abi(
            XamlSource,
            ::GetPropW(hWnd, L"XamlWindowSource"));
        if (!XamlSource)
        {
            break;
        }

        winrt::com_ptr<IDesktopWindowXamlSourceNative> XamlSourceNative =
            XamlSource.as<IDesktopWindowXamlSourceNative>();

        HWND XamlWindowHandle = nullptr;
        winrt::check_hresult(
            XamlSourceNative->get_WindowHandle(&XamlWindowHandle));

        ::SetFocus(XamlWindowHandle);

        break;
    }
    case WM_ACTIVATE:
    {
        UINT State = static_cast<UINT>(LOWORD(wParam));
        BOOL Minimized = static_cast<BOOL>(HIWORD(wParam));

        if (Minimized || State == WA_INACTIVE)
        {
            break;
        }

        winrt::DesktopWindowXamlSource XamlSource = nullptr;
        winrt::copy_from_abi(
            XamlSource,
            ::GetPropW(hWnd, L"XamlWindowSource"));
        if (!XamlSource)
        {
            break;
        }

        winrt::com_ptr<IDesktopWindowXamlSourceNative> XamlSourceNative =
            XamlSource.as<IDesktopWindowXamlSourceNative>();

        HWND XamlWindowHandle = nullptr;
        winrt::check_hresult(
            XamlSourceNative->get_WindowHandle(&XamlWindowHandle));

        ::SetFocus(XamlWindowHandle);

        break;
    }
    case WM_SIZE:
    {
        winrt::DesktopWindowXamlSource XamlSource = nullptr;
        winrt::copy_from_abi(
            XamlSource,
            ::GetPropW(hWnd, L"XamlWindowSource"));
        if (!XamlSource)
        {
            break;
        }

        winrt::com_ptr<IDesktopWindowXamlSourceNative> XamlSourceNative =
            XamlSource.as<IDesktopWindowXamlSourceNative>();

        HWND XamlWindowHandle = nullptr;
        winrt::check_hresult(
            XamlSourceNative->get_WindowHandle(&XamlWindowHandle));
        if (!XamlWindowHandle)
        {
            break;
        }

        ::SetWindowPos(
            XamlWindowHandle,
            nullptr,
            0,
            0,
            LOWORD(lParam),
            HIWORD(lParam),
            SWP_SHOWWINDOW);

        // Reference: https://github.com/microsoft/microsoft-ui-xaml
        //            /issues/3577
        // ContentDialogs don't resize themselves when the XAML island
        // resizes. However, if we manually resize our CoreWindow, that'll
        // actually trigger a resize of the ContentDialog.
        ::PostMessageW(
            ::MileXamlGetCoreWindowHandle(),
            uMsg,
            wParam,
            lParam);

        break;
    }
    case WM_DPICHANGED:
    {
        LPRECT NewWindowRectangle = reinterpret_cast<LPRECT>(lParam);

        ::SetWindowPos(
            hWnd,
            nullptr,
            NewWindowRectangle->left,
            NewWindowRectangle->top,
            NewWindowRectangle->right - NewWindowRectangle->left,
            NewWindowRectangle->bottom - NewWindowRectangle->top,
            SWP_NOZORDER | SWP_NOACTIVATE);
    }
    case WM_ERASEBKGND:
    {
        RECT ClientArea = { 0 };
        if (::GetClientRect(hWnd, &ClientArea))
        {
            COLORREF BackgroundColor = HandleToULong(
                ::GetPropW(hWnd, L"BackgroundFallbackColor"));
            if (BackgroundColor)
            {
                HBRUSH BackgroundBrush =
                    ::CreateSolidBrush(BackgroundColor);
                if (BackgroundBrush)
                {
                    ::FillRect(
                        reinterpret_cast<HDC>(wParam),
                        &ClientArea,
                        BackgroundBrush);

                    ::DeleteObject(BackgroundBrush);
                }
            }
            else
            {
                ::FillRect(
                    reinterpret_cast<HDC>(wParam),
                    &ClientArea,
                    reinterpret_cast<HBRUSH>(
                        ::GetStockObject(BLACK_BRUSH)));
            }
        }

        return TRUE;
    }
    case WM_MENUCHAR:
    {
        // Reference: https://github.com/microsoft/terminal
        //            /blob/756fd444b1d443320cf2ed6887d4b65aa67a9a03
        //            /scratch/ScratchIslandApp
        //            /WindowExe/SampleIslandWindow.cpp#L155
        // Return this LRESULT here to prevent the app from making a bell
        // when alt+key is pressed. A menu is active and the user presses a
        // key that does not correspond to any mnemonic or accelerator key.

        return MAKELRESULT(0, MNC_CLOSE);
    }
    case WM_SETTINGCHANGE:
    {
        LPCTSTR Section = reinterpret_cast<LPCTSTR>(lParam);

        if (Section && 0 == std::wcscmp(Section, L"ImmersiveColorSet"))
        {
            winrt::DesktopWindowXamlSource XamlSource = nullptr;
            winrt::copy_from_abi(
                XamlSource,
                ::GetPropW(hWnd, L"XamlWindowSource"));
            if (XamlSource)
            {
                winrt::FrameworkElement Content =
                    XamlSource.Content().try_as<winrt::FrameworkElement>();
                if (Content &&
                    winrt::VisualTreeHelper::GetParent(Content))
                {
                    // Forward WM_SETTINGCHANGE to CoreWindow compatibility
                    // window for improve the runtime light and dark mode
                    // switch support for XAML Islands. (Noticed by
                    // dongle-the-gadget.)
                    ::SendMessageW(
                        ::MileXamlGetCoreWindowHandle(),
                        uMsg,
                        wParam,
                        lParam);

                    BOOL UseImmersiveDarkMode = (
                        Content.ActualTheme() == winrt::ElementTheme::Dark
                        ? TRUE
                        : FALSE);

                    if (S_OK != ::MileEnableImmersiveDarkModeForWindow(
                        hWnd,
                        UseImmersiveDarkMode))
                    {
                        if (::SetPropW(
                            hWnd,
                            L"BackgroundFallbackColor",
                            ULongToHandle(
                                ::MileGetDefaultBackgroundColorValue(
                                    UseImmersiveDarkMode))))
                        {
                            ::InvalidateRect(hWnd, nullptr, TRUE);
                        }
                    }
                }
            }
        }

        break;
    }
    case WM_DESTROY:
    {
        ::RemovePropW(hWnd, L"BackgroundFallbackColor");
        ::RemovePropW(hWnd, L"XamlWindowSource");

        if (hWnd == ::GetAncestor(hWnd, GA_ROOT))
        {
            ::PostQuitMessage(0);
        }

        break;
    }
    default:
        return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

EXTERN_C HRESULT WINAPI MileXamlGetTransparentBackgroundAttribute(
    _Out_ PBOOLEAN TransparentBackground)
{
    try
    {
        return winrt::check_hresult(winrt::Window::Current().as<IWindowPrivate>(
            )->get_TransparentBackground(TransparentBackground));
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.code();
    }
}

EXTERN_C HRESULT WINAPI MileXamlSetTransparentBackgroundAttribute(
    _In_ BOOLEAN TransparentBackground)
{
    try
    {
        return winrt::check_hresult(winrt::Window::Current().as<IWindowPrivate>(
            )->put_TransparentBackground(TransparentBackground));
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.code();
    }
}

EXTERN_C HRESULT WINAPI MileXamlGetPreferredDarkModeAttribute(
    _Out_ PBOOLEAN PreferredDarkMode)
{
    if (!PreferredDarkMode)
    {
        return E_INVALIDARG;
    }

    *PreferredDarkMode = g_PreferredDarkMode;
    return S_OK;
}

EXTERN_C HRESULT WINAPI MileXamlSetPreferredDarkModeAttribute(
    _In_ BOOLEAN PreferredDarkMode)
{
    MILE_PREFERRED_APP_MODE PreferredAppMode = (
        PreferredDarkMode
        ? MILE_PREFERRED_APP_MODE_AUTO
        : MILE_PREFERRED_APP_MODE_DEFAULT);

    ::MileSetPreferredAppMode(PreferredAppMode);

    // Call twice to get the current status.
    g_PreferredDarkMode = (
        MILE_PREFERRED_APP_MODE_AUTO == ::MileSetPreferredAppMode(
            PreferredAppMode));
    return S_OK;
}

EXTERN_C HRESULT WINAPI MileXamlThreadInitialize()
{
    try
    {
        if (g_WindowsXamlManager)
        {
            return S_OK;
        }

        g_WindowsXamlManager =
            winrt::WindowsXamlManager::InitializeForCurrentThread();

        winrt::check_hresult(::MileXamlSetTransparentBackgroundAttribute(TRUE));

        // Prevent showing the dummy/empty/ghost DesktopWindowXamlSource window
        // in the task bar.
        // Reference: https://github.com/microsoft/terminal/issues/6507
        // Reference: https://github.com/microsoft/microsoft-ui-xaml/issues/6490
        // Fixes: https://github.com/M2Team/NanaZip/issues/225
        // For also fixing the window with empty content due to CoreWindow is
        // not exist issue, create a host window without message loop is a
        // better workaround.
        g_CoreWindowHostWindowHandle = ::CreateWindowExW(
            0,
            L"Mile.Xaml.ContentWindow",
            L"Mile.Xaml.CoreWindowHostWindow",
            0,
            CW_USEDEFAULT,
            0,
            CW_USEDEFAULT,
            0,
            nullptr,
            nullptr,
            nullptr,
            nullptr);
        winrt::check_pointer(g_CoreWindowHostWindowHandle);

        HWND CoreWindowHandle = nullptr;
        winrt::check_hresult(
            winrt::CoreWindow::GetForCurrentThread().as<ICoreWindowInterop>(
            )->get_WindowHandle(&CoreWindowHandle));
        g_CoreWindowHandle = CoreWindowHandle;

        return S_OK;
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.code();
    }
}

EXTERN_C HRESULT WINAPI MileXamlThreadUninitialize()
{
    try
    {
        if (g_CoreWindowHostWindowHandle)
        {
            g_CoreWindowHandle = nullptr;
            ::DestroyWindow(g_CoreWindowHostWindowHandle);
            g_CoreWindowHostWindowHandle = nullptr;
        }

        if (g_WindowsXamlManager)
        {
            g_WindowsXamlManager.Close();      
        }
        g_WindowsXamlManager = nullptr;

        {
            MSG Message;
            while (::PeekMessageW(&Message, nullptr, 0, 0, PM_REMOVE))
            {
                ::DispatchMessageW(&Message);
            }
        }

        return S_OK;
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.code();
    }
}

EXTERN_C HRESULT WINAPI MileXamlGlobalInitialize()
{
    try
    {
        WNDCLASSEXW WindowClass;
        WindowClass.cbSize = sizeof(WNDCLASSEXW);
        WindowClass.style = 0;
        WindowClass.lpfnWndProc = ::MileXamlContentWindowDefaultCallback;
        WindowClass.cbClsExtra = 0;
        WindowClass.cbWndExtra = 0;
        WindowClass.hInstance = nullptr;
        WindowClass.hIcon = nullptr;
        WindowClass.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
        WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(
            ::GetStockObject(BLACK_BRUSH));
        WindowClass.lpszMenuName = nullptr;
        WindowClass.lpszClassName = L"Mile.Xaml.ContentWindow";
        WindowClass.hIconSm = nullptr;
        winrt::check_bool(::RegisterClassExW(&WindowClass));

        winrt::check_hresult(::MileXamlSetPreferredDarkModeAttribute(TRUE));

        winrt::check_hresult(::MileXamlThreadInitialize());

        return S_OK;
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.code();
    }
}

EXTERN_C HRESULT WINAPI MileXamlGlobalUninitialize()
{
    try
    {
        if (g_IsGlobalUninitialized)
        {
            return S_OK;
        }

        g_IsGlobalUninitialized = true;

        winrt::check_hresult(::MileXamlThreadUninitialize());

        BOOLEAN PreferredDarkMode = FALSE;
        winrt::check_hresult(
            ::MileXamlGetPreferredDarkModeAttribute(&PreferredDarkMode));
        if (PreferredDarkMode)
        {
            winrt::check_hresult(
                ::MileXamlSetPreferredDarkModeAttribute(FALSE));
        }

        ::UnregisterClassW(L"Mile.Xaml.ContentWindow", nullptr);        

        return S_OK;
    }
    catch (winrt::hresult_error const& ex)
    {
        return ex.code();
    }
}
