/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      Mile.Xaml.cpp
 * PURPOSE:   Implementation for the Mile.Xaml
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include "pch.h"

#include "Mile.Xaml.Private.h"

#include "Application.g.cpp"

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
    using Windows::UI::Xaml::FocusState;
    using Windows::UI::Xaml::FrameworkElement;
    using Windows::UI::Xaml::ResourceDictionary;
    using Windows::UI::Xaml::Window;
    using Windows::UI::Xaml::Controls::Control;
    using Windows::UI::Xaml::Controls::Grid;
    using Windows::UI::Xaml::Hosting::DesktopWindowXamlSource;
    using Windows::UI::Xaml::Hosting::DesktopWindowXamlSourceTakeFocusRequestedEventArgs;
    using Windows::UI::Xaml::Media::VisualTreeHelper;
}

namespace
{
    static bool volatile g_PreferredDarkMode = false;

    static LRESULT CALLBACK MileXamlContentWindowCallback(
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
            XamlSource.Content(Content);

            HWND XamlWindowHandle = nullptr;
            if (FAILED(XamlSourceNative->get_WindowHandle(&XamlWindowHandle)))
            {
                return -1;
            }

            // When focus is moving out from XAML island, move it back in again.
            XamlSource.TakeFocusRequested([](
                winrt::DesktopWindowXamlSource sender,
                winrt::DesktopWindowXamlSourceTakeFocusRequestedEventArgs args)
            {
                sender.Content().as<winrt::Control>().Focus(
                    winrt::FocusState::Programmatic);
            });

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
                Content.ActualTheme() == winrt::ElementTheme::Dark
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
                        Content.RequestedTheme(winrt::ElementTheme::Default);

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

namespace winrt::Mile::Xaml::implementation
{
    Application::Application(winrt::XamlMetadataProviders const& Providers)
    {
        for(auto const& Provider : Providers)
        {
            this->m_Providers.Append(Provider);
        }

        this->Initialize();
    }

    Application::Application()
    {
    }

    void Application::Initialize()
    {
        const auto out = outer();
        if (out)
        {
            winrt::IXamlMetadataProvider provider(nullptr);
            winrt::check_hresult(out->QueryInterface(
                winrt::guid_of<winrt::IXamlMetadataProvider>(),
                reinterpret_cast<void**>(winrt::put_abi(provider))));
            this->m_Providers.Append(provider);
        }

        this->m_WindowsXamlManager =
            winrt::WindowsXamlManager::InitializeForCurrentThread();

        winrt::check_hresult(::MileXamlSetTransparentBackgroundAttribute(TRUE));

        WNDCLASSEXW WindowClass;
        WindowClass.cbSize = sizeof(WNDCLASSEXW);
        WindowClass.style = 0;
        WindowClass.lpfnWndProc = ::MileXamlContentWindowCallback;
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

        // Prevent showing the dummy/empty/ghost DesktopWindowXamlSource window
        // in the task bar.
        // Reference: https://github.com/microsoft/terminal/issues/6507
        // Reference: https://github.com/microsoft/microsoft-ui-xaml/issues/6490
        // Fixes: https://github.com/M2Team/NanaZip/issues/225
        // For also fixing the window with empty content due to CoreWindow is
        // not exist issue, create a host window without message loop is a
        // better workaround.
        this->m_CoreWindowHostWindowHandle = ::CreateWindowExW(
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
            winrt::get_abi(winrt::Grid()));
        winrt::check_pointer(this->m_CoreWindowHostWindowHandle);
    }

    void Application::Close()
    {
        if (this->m_IsClosed)
        {
            return;
        }

        this->m_IsClosed = true;

        ::DestroyWindow(this->m_CoreWindowHostWindowHandle);

        BOOLEAN PreferredDarkMode = FALSE;
        winrt::check_hresult(
            ::MileXamlGetPreferredDarkModeAttribute(&PreferredDarkMode));
        if (PreferredDarkMode)
        {
            winrt::check_hresult(
                ::MileXamlSetPreferredDarkModeAttribute(FALSE));
        }

        ::UnregisterClassW(L"Mile.Xaml.ContentWindow", nullptr);

        this->m_WindowsXamlManager.Close();
        this->m_Providers.Clear();
        this->m_WindowsXamlManager = nullptr;

        this->Exit();

        {
            MSG Message;
            while (::PeekMessageW(&Message, nullptr, 0, 0, PM_REMOVE))
            {
                ::DispatchMessageW(&Message);
            }
        }
    }

    Application::~Application()
    {
        this->Close();
    }

    winrt::IXamlType Application::GetXamlType(winrt::TypeName const& type)
    {
        for (const auto& Provider : this->m_Providers)
        {
            const auto Current = Provider.GetXamlType(type);
            if (Current != nullptr)
            {
                return Current;
            }
        }

        return nullptr;
    }

    winrt::IXamlType Application::GetXamlType(winrt::hstring const& fullName)
    {
        for (const auto& Provider : this->m_Providers)
        {
            const auto Current = Provider.GetXamlType(fullName);
            if (Current != nullptr)
            {
                return Current;
            }
        }

        return nullptr;
    }

    winrt::XmlnsDefinitions Application::GetXmlnsDefinitions()
    {
        std::list<winrt::XmlnsDefinition> Definitions;

        for (const auto& Provider : this->m_Providers)
        {
            for (const auto& Definition : Provider.GetXmlnsDefinitions())
            {
                Definitions.insert(Definitions.begin(), Definition);
            }
        }

        return winrt::XmlnsDefinitions(Definitions.begin(), Definitions.end());
    }

    winrt::XamlMetadataProviders Application::MetadataProviders()
    {
        return this->m_Providers;
    }
}

namespace winrt::Mile::Xaml::factory_implementation
{
    Application::Application()
    {
        // Workaround a bug where twinapi.appcore.dll and threadpoolwinrt.dll
        // gets loaded after it has been unloaded because of a call to
        // GetActivationFactory.
        LPCWSTR PreloadModuleNames[] =
        {
            L"twinapi.appcore.dll",
            L"threadpoolwinrt.dll",
        };
        const size_t PreloadModuleNamesCount =
            sizeof(PreloadModuleNames) / sizeof(*PreloadModuleNames);
        for (size_t i = 0; i < PreloadModuleNamesCount; ++i)
        {
            this->m_PreloadModules.push_back(
                ::LoadLibraryExW(
                    PreloadModuleNames[i],
                    nullptr,
                    LOAD_LIBRARY_SEARCH_SYSTEM32));
        }
    }

    Application::~Application()
    {
        for (auto const& PreloadModules : this->m_PreloadModules)
        {
            ::FreeLibrary(PreloadModules);
        }
        this->m_PreloadModules.clear();
    }
}
