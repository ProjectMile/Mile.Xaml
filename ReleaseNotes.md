# Mile.Xaml Release Notes

**Mile.Xaml 1.1 (1.1.433.0)**

- Port Windows 11 XAML control styles from Windows UI Library 2.8.1. 
  (Contributed by AndromedaMelody.)
- Implement Mile.Xaml.XamlControlsResources class.
- Fix throwing exception issue when destroying the Window.
- Adjust the WindowsTargetPlatformMinVersion to 10.0.18362.0 for reducing the
  compilation warnings.
- Prevent showing the dummy/empty/ghost DesktopWindowXamlSource window in the
  task bar. (Thanks to Nothing9495.)
- Add immersive Mica effect support for Mile.Xaml.ContentWindow window class.
  (Contributed by AndromedaMelody.)
- Add Win32 context menu dark mode support for Mile.Xaml. (Contributed by 
  AndromedaMelody.)
- Add TransparentBackground property for Mile.Xaml.Application class.
- Add PreferredDarkModeIfAvailable property for Mile.Xaml.Application class.
- Adjust and simplify the Mile.Xaml implementation.

**Mile.Xaml 1.0 (1.0.361.0)**

- Integrate all features from Microsoft.Toolkit.Win32.UI.XamlApplication.
- Provide Mile.Xaml.ContentWindow window class for simplifying the development.
- Provide partial XAML Designer and Hot Reload support.
- Use [VC-LTL 5.x](https://github.com/Chuyu-Team/VC-LTL5) toolchain to make the
  binary size smaller.
- Provide NuGet package.
