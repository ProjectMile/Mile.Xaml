# Mile.Xaml Release Notes

**Mile.Xaml 2.0 Preview 2 (2.0.573.0)**

- Use C functions instead of Windows Runtime classes to interoperate Mile.Xaml.
  - MileXamlGetCoreWindowHandle
  - MileXamlGetTransparentBackgroundAttribute
  - MileXamlSetTransparentBackgroundAttribute
  - MileXamlGetPreferredDarkModeAttribute
  - MileXamlSetPreferredDarkModeAttribute
  - MileXamlThreadInitialize
  - MileXamlThreadUninitialize
  - MileXamlGlobalInitialize
  - MileXamlGlobalUninitialize
- Improve the .NET Framework support.
- Fix keyboard navigation with Shift-Tab. (Contributed by driver1998.)
- Make buildTransitive mode only applies to the C/C++ projects. (Contributed by
  AndromedaMelody.)
- Add native0.0 target framework setting. (Suggested by AndromedaMelody.)
- Remove the unnecessary precompiled header file for optimizing the size and
  reducing the disk space usage when compiling.
- Add ContentDialog resizing support. (Inspired by Windows Terminal.)
- Forward WM_SETTINGCHANGE to CoreWindow compatibility window for improve the
  runtime light and dark mode switch support for XAML Islands. (Noticed by
  dongle-the-gadget.)
- Add source code of Mile.Xaml.Styles.SunValley and update the generated
  Mile.Xaml.Styles.SunValley.xaml.. (Contributed by AndromedaMelody.)
- Add Mile.Xaml.MaintainerTools solution and Mile.Xaml.ControlStyleXamlGenerator
  project.
- Update Mile.Project.Windows.
- Adjust project configurations.
- Simplify build script logic.
- Update license.

**Mile.Xaml 1.2 (1.2.495.0)**

- Update Mile.Windows.Helpers to 1.0.11.
- Use solid background color instead of extending DWM frame into client area in
  Windows 10 for solving background color behavior issue when user enabling the
  "Show accent color on title bars and window borders" option in the Settings
  app.
- Update the minimum system requirement to Windows 10 Version 2004 (Build 19041)
  or later.
- Update to Git submodule version of Mile.Project.Windows.
- Remove MileXamlApplication and MileXamlApplicationPackage project because it
  will be added to the Mile.Xaml.Samples repository.
- Adjust NuGet package layout. (Suggested by AndromedaMelody.)
- Add target support for .NET Framework 4.8 or later. (Contributed by 
  AndromedaMelody.)
- Adjust project configurations.

**Mile.Xaml 1.1 (1.1.434.0)**

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
- Fix the window with empty content due to CoreWindow is not exist issue.

**Mile.Xaml 1.0 (1.0.361.0)**

- Integrate all features from Microsoft.Toolkit.Win32.UI.XamlApplication.
- Provide Mile.Xaml.ContentWindow window class for simplifying the development.
- Provide partial XAML Designer and Hot Reload support.
- Use [VC-LTL 5.x](https://github.com/Chuyu-Team/VC-LTL5) toolchain to make the
  binary size smaller.
- Provide NuGet package.
