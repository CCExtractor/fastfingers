#!/bin/bash
echo "===================================="
echo "Creating Arch Package"
echo "===================================="

if [ ${1} != "" ]
then
    path=${1}
else
    path=${PWD}
fi

mkdir -p -v ${path}/packages/arch

echo "\
# Maintainer: Ali Eren Yogurtcu <aeren.yogurtcu@gmail.com>
pkgname=fastfingers
pkgver=0.7.2
pkgrel=1
pkgdesc="FastFingers is an training application \
that helps its users to remove the habit of using a mouse, \
by helping them to memorize the shortcuts they want to \
learn by practicing."
arch=('any')
url="https://github.com/CCExtractor/fastfingers"
license=('GPL')
depends=('gtk3')
source=("https://github.com/CCExtractor/fastfingers/archive/refs/tags/v$pkgver.tar.gz")

prepare() {
        cmake -S "$pkgname-$pkgver/src" -B "$pkgname-$pkgver/src/build" -DCMAKE_BUILD_TYPE=Release
}

build() {
        cmake --build "$pkgname-$pkgver/src/build" --config Release
}

package () {
    mkdir -pv ${pkgdir}/usr/bin
    mkdir -pv ${pkgdir}/usr/share/fastfingers/applications
    mkdir -pv ${pkgdir}/usr/share/icons
    mkdir -pv ${pkgdir}/usr/share/applications
    mkdir -pv ${pkgdir}/usr/share/glib-2.0/schemas

    cp -v $pkgname-$pkgver/src/build/fastfingers ${pkgdir}/usr/bin
    cp -v $pkgname-$pkgver/src/build/fastfingers-cheatsheet ${pkgdir}/usr/bin
    chmod -vR u=rwx,g=rx,o=rx ${pkgdir}/usr/bin

    cp -v $pkgname-$pkgver/src/build/org.ccextractor.FastFingers.desktop ${pkgdir}/usr/share/applications/
    cp -v $pkgname-$pkgver/src/build/org.ccextractor.Cheatsheet.desktop ${pkgdir}/usr/share/applications/
    chmod -vR u=rwx,g=rx,o=rx ${pkgdir}/usr/share/applications

    cp -vR $pkgname-$pkgver/data/icons/* ${pkgdir}/usr/share/icons
    chmod -vR u=rwx,g=rx,o=rx ${pkgdir}/usr/share/icons

    cp -vp $pkgname-$pkgver/data/applications/* ${pkgdir}/usr/share/fastfingers/applications
    chmod -R u=rw,g=rw,o=rw ${pkgdir}/usr/share/fastfingers/applications/

    cp -v $pkgname-$pkgver/data/org.ccextractor.FastFingers.gschema.xml ${pkgdir}/usr/share/glib-2.0/schemas
    chmod -v u=rw,g=r,o=r ${pkgdir}/usr/share/glib-2.0/schemas/org.ccextractor.FastFingers.gschema.xml

    chmod -Rv u=rwx,g=rwx,o=rwx ${pkgdir}
}

post_install () {
    chmod -vR u=rw,g=rw,o=rw /usr/share/fastfingers/applications
}
md5sums=('c4d5f68ada39261756314d0be9d84ab5')
" > ${path}/packages/debian/fastfingers/DEBIAN/control

echo "#!/bin/bash
gtk-update-icon-cache -t -f /usr/share/icons/HighContrast/
gtk-update-icon-cache -t -f /usr/share/icons/hicolor/" > ${path}/packages/debian/fastfingers/DEBIAN/postinst

mkdir -p -v ${path}/packages/debian/fastfingers/usr/bin
cp -v ${path}/src/build/fastfingers ${path}/packages/debian/fastfingers/usr/bin
cp -v ${path}/src/build/fastfingers-cheatsheet ${path}/packages/debian/fastfingers/usr/bin
chmod -v u=rwx,g=rx,o=rx ${path}/packages/debian/fastfingers/usr/bin/*

mkdir -p -v ${path}/packages/debian/fastfingers/usr/share/fastfingers/applications
cp -R -v ${path}/data/applications ${path}/packages/debian/fastfingers/usr/share/fastfingers/applications/
chmod -v u=rw,g=rw,o=rw ${path}/packages/debian/fastfingers/usr/share/fastfingers/applications/*

mkdir -p -v ${path}/packages/debian/fastfingers/usr/share/applications/
cp -v ${path}/src/build/org.ccextractor.FastFingers.desktop ${path}/packages/debian/fastfingers/usr/share/applications/
cp -v ${path}/src/build/org.ccextractor.Cheatsheet.desktop ${path}/packages/debian/fastfingers/usr/share/applications/
chmod -v u=rw,g=r,o=r ${path}/packages/debian/fastfingers/usr/share/applications/*

cp -R -v ${path}/data/icons ${path}/packages/debian/fastfingers/usr/share/
chmod -v u=rw,g=r,o=r ${path}/packages/debian/fastfingers/usr/share/*

mkdir -p -v ${path}/packages/debian/fastfingers/usr/share/glib-2.0/schemas
cp -v ${path}/data/org.ccextractor.FastFingers.gschema.xml ${path}/packages/debian/fastfingers/usr/share/glib-2.0/schemas
chmod -v u=rw,g=r,o=r ${path}/packages/debian/fastfingers/usr/share/glib-2.0/schemas/org.ccextractor.FastFingers.gschema.xml

chmod -R -v u=rwx,g=rx,o=rx ${path}/packages/debian/fastfingers/DEBIAN/

dpkg-deb --build ${path}/packages/debian/fastfingers ${path}/packages/debian

rm -rfv ${path}/packages/debian/fastfingers/
