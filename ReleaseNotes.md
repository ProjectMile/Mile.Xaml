# Mile.Xaml Release Notes

**Mile.Xaml 2.1 Preview 1 (2.1.651.0)**

- Add Windows 11 XAML control styles for several XAML controls from Windows UI
  Library 2.8.4. (Contributed by Gaoyifei1011.)
  - CalendarDatePicker and CalendarView
  - ColorPicker and ColorSpectrum
  - DatePicker and DateTimePickerFlyout
  - FlipView and FlipViewItem
  - GridViewItem
  - InkToolbar
  - MediaTransportControls
  - PersonPicture
  - RatingControl
  - TimePicker and DateTimePickerFlyout
  - TreeView and TreeViewItem
- Update email address for source code.
- Update Mile.Project.Windows.
- Adjust project configurations. 

**Mile.Xaml 2.0 Update 2 (2.0.617.0)**

- Improve the C/C++ projects support.
  - Add MileXamlContentWindowDefaultCallback API.
  - Add MileXamlSetXamlContentForContentWindow API.

**Mile.Xaml 2.0 Update 1 (2.0.616.0)**

- Improve the C/C++ projects support.
  - Fix behavior issue for MileXamlThreadInitialize API.
  - Fix ContentDialogs resizing handling issue when using maximize and restore.
  - Update Mile.Windows.Helpers to 1.0.15 for improving the immersive UI
	experience before Windows 11.

**Mile.Xaml 2.0 (2.0.611.0)**

- Use C functions instead of Windows Runtime classes to interoperate Mile.Xaml
  from C/C++ projects and make use source code form to integrate directly.
  - MileXamlGetCoreWindowHandle
  - MileXamlGetTransparentBackgroundAttribute
  - MileXamlSetTransparentBackgroundAttribute
  - MileXamlGetPreferredDarkModeAttribute
  - MileXamlSetPreferredDarkModeAttribute
  - MileXamlThreadInitialize
  - MileXamlThreadUninitialize
  - MileXamlGlobalInitialize
  - MileXamlGlobalUninitialize
- Improve the .NET Framework support via adding the pure managed version of
  Mile.Xaml called Mile.Xaml.Managed which supports .NET Any CPU target.
- Remove the unfinished .NET Native target support.
- Fix keyboard navigation with Shift-Tab. (Contributed by driver1998.)
- Fix missing break statement when handling WM_DPICHANGED. (Contributed by
  kernelbin.)
- Remove buildTransitive mode for all projects. (Partially contributed by
  AndromedaMelody.)
- Add native0.0 target framework setting. (Suggested by AndromedaMelody.)
- Remove the unnecessary precompiled header file for optimizing the size and
  reducing the disk space usage when compiling.
- Add ContentDialog resizing support. (Inspired by Windows Terminal.)
- Forward WM_SETTINGCHANGE to CoreWindow compatibility window for improve the
  runtime light and dark mode switch support for XAML Islands. (Noticed by
  dongle-the-gadget.)
- Add MileXamlNoSunValleyXamlStyle MSBuild option support.
- Add source code of Mile.Xaml.Styles.SunValley and update the generated
  Mile.Xaml.Styles.SunValley.xaml. (Contributed by AndromedaMelody.)
- Split XAML control styles binary generation to Mile.Xaml.Styles project.
- Add maintainer tools projects.
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
