Name:           fastfingers
Version:        0.7.2
Release:        1%{?dist}
Summary:        FastFingers is an shortcut training application.

License:        GPL-2.0
URL:            https://github.com/CCExtractor/fastfingers

BuildRequires:  gcc cmake gtk3-devel make pkgconfig
Requires:       gtk3

%description
FastFingers is an training application \
that helps its users to remove the habit of using a mouse, \
by helping them to memorize the shortcuts they want to \
learn by practicing.

%build
    cmake -S %{_sourcedir}/src -B %{_builddir} -DCMAKE_BUILD_TYPE=Release
    cmake --build %{_builddir} --config Release

%install
    rm -rf %{buildroot}
    mkdir -pv %{buildroot}/usr/bin
    mkdir -pv %{buildroot}/usr/share/fastfingers/applications
    mkdir -pv %{buildroot}/usr/share/icons
    mkdir -pv %{buildroot}/usr/share/applications
    mkdir -pv %{buildroot}/usr/share/glib-2.0/schemas

    cp -v %{_builddir}/fastfingers %{buildroot}/usr/bin
    cp -v %{_builddir}/fastfingers-cheatsheet %{buildroot}/usr/bin
    cp -v %{_builddir}/org.ccextractor.FastFingers.desktop %{buildroot}/usr/share/applications/
    cp -v %{_builddir}/org.ccextractor.Cheatsheet.desktop %{buildroot}/usr/share/applications/
    cp -vR %{_sourcedir}/data/icons/* %{buildroot}/usr/share/icons
    cp -v %{_sourcedir}/data/org.ccextractor.FastFingers.gschema.xml %{buildroot}/usr/share/glib-2.0/schemas

    install -vpm 777 %{_sourcedir}/data/applications/* %{buildroot}/usr/share/fastfingers/applications

%files
   /usr/bin/fastfingers
   /usr/bin/fastfingers-cheatsheet
   /usr/share/applications/org.ccextractor.Cheatsheet.desktop
   /usr/share/applications/org.ccextractor.FastFingers.desktop
   /usr/share/fastfingers/applications/appdata.json
   /usr/share/fastfingers/applications/firefox.json
   /usr/share/fastfingers/applications/gimp.json
   /usr/share/fastfingers/applications/slack.json
   /usr/share/fastfingers/applications/thunderbird.json
   /usr/share/fastfingers/applications/vlc.json
   /usr/share/fastfingers/applications/vscode.json
   /usr/share/glib-2.0/schemas/org.ccextractor.FastFingers.gschema.xml
   /usr/share/icons/HighContrast/16x16/apps/fastfingers-cheatsheet.png
   /usr/share/icons/HighContrast/16x16/apps/fastfingers.png
   /usr/share/icons/HighContrast/22x22/apps/fastfingers-cheatsheet.png
   /usr/share/icons/HighContrast/22x22/apps/fastfingers.png
   /usr/share/icons/HighContrast/24x24/apps/fastfingers-cheatsheet.png
   /usr/share/icons/HighContrast/24x24/apps/fastfingers.png
   /usr/share/icons/HighContrast/256x256/apps/fastfingers-cheatsheet.png
   /usr/share/icons/HighContrast/256x256/apps/fastfingers.png
   /usr/share/icons/HighContrast/32x32/apps/fastfingers-cheatsheet.png
   /usr/share/icons/HighContrast/32x32/apps/fastfingers.png
   /usr/share/icons/HighContrast/48x48/apps/fastfingers-cheatsheet.png
   /usr/share/icons/HighContrast/48x48/apps/fastfingers.png
   /usr/share/icons/hicolor/128x128/apps/fastfingers-cheatsheet.png
   /usr/share/icons/hicolor/128x128/apps/fastfingers.png
   /usr/share/icons/hicolor/16x16/apps/fastfingers-cheatsheet.png
   /usr/share/icons/hicolor/16x16/apps/fastfingers.png
   /usr/share/icons/hicolor/32x32/apps/fastfingers-cheatsheet.png
   /usr/share/icons/hicolor/32x32/apps/fastfingers.png
   /usr/share/icons/hicolor/48x48/apps/fastfingers-cheatsheet.png
   /usr/share/icons/hicolor/48x48/apps/fastfingers.png
   /usr/share/icons/hicolor/64x64/apps/fastfingers-cheatsheet.png
   /usr/share/icons/hicolor/64x64/apps/fastfingers.png
