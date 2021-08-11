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
    ls -la
    mkdir -pv %{buildroot}/usr/bin
    mkdir -pv %{buildroot}/usr/share/fastfingers/applications
    mkdir -pv %{buildroot}/usr/share/icons
    mkdir -pv %{buildroot}/usr/share/applications
    mkdir -pv %{buildroot}/usr/share/glib-2.0/schemas

    cp -v src/build/fastfingers %{buildroot}/usr/bin
    cp -v src/build/fastfingers-cheatsheet %{buildroot}/usr/bin
    chmod -vR u=rwx,g=rx,o=rx %{buildroot}/usr/bin

    cp -v src/build/org.ccextractor.FastFingers.desktop %{buildroot}/usr/share/applications/
    cp -v src/build/org.ccextractor.Cheatsheet.desktop %{buildroot}/usr/share/applications/
    chmod -vR u=rwx,g=rx,o=rx %{buildroot}/usr/share/applications

    cp -vR data/icons/* %{buildroot}/usr/share/icons
    chmod -vR u=rwx,g=rx,o=rx %{buildroot}/usr/share/icons

    cp -vp data/applications/* %{buildroot}/usr/share/fastfingers/applications
    chmod -R u=rw,g=rw,o=rw %{buildroot}/usr/share/fastfingers/applications/

    cp -v data/org.ccextractor.FastFingers.gschema.xml %{buildroot}/usr/share/glib-2.0/schemas
    chmod -v u=rw,g=r,o=r %{buildroot}/usr/share/glib-2.0/schemas/org.ccextractor.FastFingers.gschema.xml

    chmod -Rv u=rwx,g=rwx,o=rwx %{buildroot}

%files
%license LICENSE