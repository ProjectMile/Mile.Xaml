#pragma once

#include "Application.g.h"
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <Windows.h>

namespace winrt::Mile::Xaml::implementation
{
    enum ExecutionMode
    {
        UWP = 0,
        Win32 = 1,
    };

    class Application : public ApplicationT<Application, Windows::UI::Xaml::Markup::IXamlMetadataProvider>
    {
    public:
        Application();
        Application(winrt::Windows::Foundation::Collections::IVector<winrt::Windows::UI::Xaml::Markup::IXamlMetadataProvider> providers);
        ~Application();

        void Initialize();
        void Close();

        winrt::Windows::Foundation::IClosable WindowsXamlManager() const;

        winrt::Windows::UI::Xaml::Markup::IXamlType GetXamlType(winrt::Windows::UI::Xaml::Interop::TypeName const& type);
        winrt::Windows::UI::Xaml::Markup::IXamlType GetXamlType(winrt::hstring const& fullName);
        winrt::com_array<winrt::Windows::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions();

        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::UI::Xaml::Markup::IXamlMetadataProvider> MetadataProviders();

        bool IsDisposed() const
        {
            return m_bIsClosed;
        }

    private:
        ExecutionMode m_executionMode = ExecutionMode::Win32;
        winrt::Windows::UI::Xaml::Hosting::WindowsXamlManager m_windowsXamlManager = nullptr;
        winrt::Windows::Foundation::Collections::IVector<winrt::Windows::UI::Xaml::Markup::IXamlMetadataProvider> m_providers = winrt::single_threaded_vector<Windows::UI::Xaml::Markup::IXamlMetadataProvider>();
        bool m_bIsClosed = false;
    };
}

namespace winrt::Mile::Xaml::factory_implementation
{
    class Application : public ApplicationT<Application, implementation::Application>
    {
    public:
        Application();
        ~Application();
    private:
        std::array<HMODULE, 2> m_preloadInstances;
    };
}
