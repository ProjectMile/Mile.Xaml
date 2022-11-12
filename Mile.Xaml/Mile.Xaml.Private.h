/*
 * PROJECT:   Mouri Internal Library Essentials
 * FILE:      Mile.Xaml.Private.h
 * PURPOSE:   Private Definition for the Mile.Xaml
 *
 * LICENSE:   The MIT License
 *
 * DEVELOPER: Mouri_Naruto (Mouri_Naruto AT Outlook.com)
 */

#pragma once

#include "Application.g.h"
#include "XamlControlsResources.g.h"

#include "Mile.Xaml.h"

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>

namespace winrt
{
    using Windows::Foundation::Collections::IVector;
    using Windows::Foundation::IClosable;
    using Windows::UI::Xaml::Interop::TypeName;
    using Windows::UI::Xaml::Hosting::WindowsXamlManager;
    using Windows::UI::Xaml::Markup::IXamlMetadataProvider;
    using Windows::UI::Xaml::Markup::IXamlType;
    using Windows::UI::Xaml::Markup::XmlnsDefinition;
    using XmlnsDefinitions = winrt::com_array<winrt::XmlnsDefinition>;
    using XamlMetadataProviders = winrt::IVector<winrt::IXamlMetadataProvider>;
}

namespace winrt::Mile::Xaml::implementation
{
    class Application : public ApplicationT<
        Application,
        winrt::IXamlMetadataProvider>
    {
    public:

        Application();
        Application(winrt::XamlMetadataProviders const& Providers);
        ~Application();

        void Initialize();

        void Close();

        bool TransparentBackground();
        void TransparentBackground(bool const& value);

        bool PreferredDarkModeIfAvailable();
        void PreferredDarkModeIfAvailable(bool const& value);

        winrt::IXamlType GetXamlType(winrt::TypeName const& type);
        winrt::IXamlType GetXamlType(winrt::hstring const& fullName);
        winrt::XmlnsDefinitions GetXmlnsDefinitions();

        winrt::XamlMetadataProviders MetadataProviders();

    private:

        winrt::WindowsXamlManager m_WindowsXamlManager = nullptr;
        winrt::XamlMetadataProviders m_Providers =
            winrt::single_threaded_vector<winrt::IXamlMetadataProvider>();
        bool m_IsClosed = false;
    };

    class XamlControlsResources : public XamlControlsResourcesT<
        XamlControlsResources,
        winrt::composable>
    {
    public:

        XamlControlsResources();
    };
}

namespace winrt::Mile::Xaml::factory_implementation
{
    class Application : public ApplicationT<
        Application,
        implementation::Application>
    {
    public:

        Application();
        ~Application();

    private:

        std::vector<HMODULE> m_PreloadModules;
    };

    class XamlControlsResources : public XamlControlsResourcesT<
        XamlControlsResources,
        implementation::XamlControlsResources>
    {
    };
}
