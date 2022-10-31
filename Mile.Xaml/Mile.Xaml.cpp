/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      Application.cpp
 * PURPOSE:   Implementation for the Mile.Xaml
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#include "pch.h"

#include "Mile.Xaml.h"

#include "Application.g.cpp"
#include "XamlControlsResources.g.cpp"

#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Media.h>

#include <CoreWindow.h>
#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>

#include <Mile.Windows.DwmHelpers.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

bool IsWindowsVersionOrLater(
    DWORD const& MajorVersion,
    DWORD const& MinorVersion,
    DWORD const& BuildNumber)
{
    OSVERSIONINFOEXW OSVersionInfoEx = { 0 };
    OSVersionInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
    OSVersionInfoEx.dwMajorVersion = MajorVersion;
    OSVersionInfoEx.dwMinorVersion = MinorVersion;
    OSVersionInfoEx.dwBuildNumber = BuildNumber;
    return ::VerifyVersionInfoW(
        &OSVersionInfoEx,
        VER_BUILDNUMBER,
        ::VerSetConditionMask(
            ::VerSetConditionMask(
                ::VerSetConditionMask(
                    0,
                    VER_MAJORVERSION,
                    VER_GREATER_EQUAL),
                VER_MINORVERSION,
                VER_GREATER_EQUAL),
            VER_BUILDNUMBER,
            VER_GREATER_EQUAL));
}

static bool IsSupportSystemBackdrop()
{
    static bool CachedResult = IsWindowsVersionOrLater(10, 0, 22523);
    return CachedResult;
}

/**
 * @brief Flags for specifying the system-drawn backdrop material of a window,
 *        including behind the non-client area.
 * @return Auto Let the Desktop Window Manager automatically decide the
 *              system-drawn backdrop material for this window. (Applies to
 *              TitleBar only, not supporting extend into client aero.)
 * @return None Don't draw any system backdrop.
 * @return Mica Draw the backdrop material effect corresponding to a long-lived
 *              window.
 * @return Acrylic Draw the backdrop material effect corresponding to a
 *                 transient window.
 * @return MicaAlt Draw the backdrop material effect corresponding to a
 *                 window with a tabbed title bar.
*/
enum class DwmSystemBackdropType : DWORD
{
    Auto = 0, 
    None = 1,
    Mica = 2,
    Acrylic = 3,
    MicaAlt = 4
};

/**
 * @brief 
 * @return Defalut
 * @return Auto
 * @return Dark
 * @return Light 
*/
enum class UxThemePreferredAppModeType : DWORD
{
    Defalut = 0, 
    Auto = 1,
    Dark = 2,
    Light = 3
};

/**
 * @brief Extends the window frame into the client area.
 * @param WindowHandle The handle to the window for which the attribute value
 *                     is to be set.
 * @param LeftWidth describes the left margins to use when extending the frame
 *                  into the client area.
 * @param TopHeight describes the top margins to use when extending the frame
 *                  into the client area.
 * @param RightWidth describes the right margins to use when extending the frame
 *                   into the client area.
 * @param BottomHeight describes the bottom margins to use when extending the frame
 *                     into the client area.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
HRESULT MileSetWindowFrameMargins(
    HWND WindowHandle,
    int LeftWidth,
    int TopHeight,
    int RightWidth,
    int BottomHeight)
{
    MARGINS Value{ 0 };
    Value.cxLeftWidth = LeftWidth;
    Value.cyTopHeight = TopHeight;
    Value.cxRightWidth = RightWidth;
    Value.cyBottomHeight = BottomHeight;
    return DwmExtendFrameIntoClientArea(
        WindowHandle,
        &Value);
}

/**
 * @brief Retrieves or specifies the system-drawn backdrop material of a
 *        window, including behind the non-client area.
 * @param WindowHandle The handle to the window for which the attribute value
 *                     is to be set.
 * @param Type Flags for specifying the system-drawn backdrop
 *                           material of a window, including behind the
 *                           non-client area.
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
HRESULT MileSetSystemBackdropAttribute(
    HWND WindowHandle,
    DwmSystemBackdropType Type)
{
    if (!::IsSupportSystemBackdrop())
    {
        return E_NOINTERFACE;
    }
    ::MileSetCaptionColorAttribute(WindowHandle, static_cast<COLORREF>(-1));
    const DWORD DwmWindowSystemBackdropTypeAttribute = 38;
    return ::DwmSetWindowAttribute(
        WindowHandle,
        DwmWindowSystemBackdropTypeAttribute,
        &Type,
        sizeof(DWORD));
}

/**
 * @brief 
 * @param Type 
 * @return If the function succeeds, it returns S_OK. Otherwise, it returns an
 *         HRESULT error code.
*/
HRESULT MileSetPreferredAppModeAttribute(
    UxThemePreferredAppModeType Type)
{
    static const auto SetPreferredAppMode =
        reinterpret_cast<HRESULT(CALLBACK*)(DWORD)>(::GetProcAddress(
            ::GetModuleHandleW(L"UxTheme.dll"),
            MAKEINTRESOURCEA(135)));
    return SetPreferredAppMode(static_cast<DWORD>(Type));
}

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
    using Windows::UI::Xaml::Hosting::DesktopWindowXamlSource;
    using Windows::UI::Xaml::Hosting::DesktopWindowXamlSourceTakeFocusRequestedEventArgs;
    using Windows::UI::Xaml::Media::VisualTreeHelper;
}

namespace
{
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

            ::MileSetUseImmersiveDarkModeAttribute(
                hWnd,
                (Content.ActualTheme() == winrt::ElementTheme::Dark
                    ? TRUE
                    : FALSE));

            ::MileSetPreferredAppModeAttribute(UxThemePreferredAppModeType::Auto);
            //::MileSetPreferredAppModeAttribute([&]()-> UxThemePreferredAppModeType
            //    {
            //        switch (Content.ActualTheme())
            //        {
            //        case winrt::ElementTheme::Default:
            //            return UxThemePreferredAppModeType::Auto;
            //        case winrt::ElementTheme::Light:
            //            return UxThemePreferredAppModeType::Light;
            //        case winrt::ElementTheme::Dark:
            //            return UxThemePreferredAppModeType::Dark;
            //        default:
            //            return UxThemePreferredAppModeType::Defalut;
            //        }
            //    });

            ::MileSetWindowFrameMargins(hWnd, -1, -1, -1, -1);

            if (FAILED(::MileSetSystemBackdropAttribute(
                hWnd,
                DwmSystemBackdropType::Mica)))
            {
                ::MileSetCaptionColorAttribute(
                    hWnd,
                    (Content.ActualTheme() == winrt::ElementTheme::Dark
                        ? RGB(32, 32, 32)
                        : RGB(243, 243, 243)));
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

                        ::MileSetUseImmersiveDarkModeAttribute(
                            hWnd,
                            (Content.ActualTheme() == winrt::ElementTheme::Dark
                                ? TRUE
                                : FALSE));

                        ::MileSetPreferredAppModeAttribute(UxThemePreferredAppModeType::Auto);
                        //::MileSetPreferredAppModeAttribute([&]()-> UxThemePreferredAppModeType
                        //    {
                        //        switch (Content.ActualTheme())
                        //        {
                        //        case winrt::ElementTheme::Default:
                        //            return UxThemePreferredAppModeType::Auto;
                        //        case winrt::ElementTheme::Light:
                        //            return UxThemePreferredAppModeType::Light;
                        //        case winrt::ElementTheme::Dark:
                        //            return UxThemePreferredAppModeType::Dark;
                        //        default:
                        //            return UxThemePreferredAppModeType::Defalut;
                        //        }
                        //    });

                        if (FAILED(::MileSetSystemBackdropAttribute(
                            hWnd,
                            DwmSystemBackdropType::Mica)))
                        {
                            ::MileSetCaptionColorAttribute(
                                hWnd,
                                (Content.ActualTheme() == winrt::ElementTheme::Dark
                                    ? RGB(32, 32, 32)
                                    : RGB(243, 243, 243)));
                        }
                    }
                }
            }

            break;
        }
        case WM_DESTROY:
        {
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

        winrt::Window::Current().as<IWindowPrivate>(
            ).TransparentBackground(true);

        // Prevent showing the dummy/empty/ghost DesktopWindowXamlSource window
        // in the task bar.
        // Reference: https://github.com/microsoft/terminal/issues/6507
        // Reference: https://github.com/microsoft/microsoft-ui-xaml/issues/6490
        // Fixes: https://github.com/M2Team/NanaZip/issues/225
        HWND CoreWindowHandle;
        winrt::check_hresult(
            winrt::CoreWindow::GetForCurrentThread().as<ICoreWindowInterop>(
                )->get_WindowHandle(&CoreWindowHandle));
        ::ShowWindow(CoreWindowHandle, SW_HIDE);

        WNDCLASSEXW WindowClass;
        WindowClass.cbSize = sizeof(WNDCLASSEXW);
        WindowClass.style = 0;
        WindowClass.lpfnWndProc = ::MileXamlContentWindowCallback;
        WindowClass.cbClsExtra = 0;
        WindowClass.cbWndExtra = 0;
        WindowClass.hInstance = nullptr;
        WindowClass.hIcon = nullptr;
        WindowClass.hCursor = ::LoadCursorW(nullptr, IDC_ARROW);
        WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        WindowClass.lpszMenuName = nullptr;
        WindowClass.lpszClassName = L"Mile.Xaml.ContentWindow";
        WindowClass.hIconSm = nullptr;

        winrt::check_bool(::RegisterClassExW(&WindowClass));
    }

    void Application::Close()
    {
        if (this->m_IsClosed)
        {
            return;
        }

        this->m_IsClosed = true;

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

    XamlControlsResources::XamlControlsResources()
    {
        this->Source(winrt::Uri(L"ms-appx:///Mile.Xaml/SunValleyStyles.xaml"));
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
