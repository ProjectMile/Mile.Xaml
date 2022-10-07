#include "pch.h"

#include "App.h"
#include "App.g.cpp"
#include "MainPage.h"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace MileXamlApplication;
using namespace MileXamlApplication::implementation;

namespace winrt::MileXamlApplication::implementation
{
    App::App()
    {
        Initialize();
    }
}
