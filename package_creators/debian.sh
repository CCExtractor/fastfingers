#!/bin/bash
echo "===================================="
echo "Creating Debian Package"
echo "===================================="

if [ ${1} != "" ]
then
    path=${1}
else
    path=${PWD}
fi

mkdir -p -v ${path}/packages/debian/fastfingers/DEBIAN

version=`cat ${path}/.version`

echo "\
Package: fastfingers
Version: ${version}
Maintainer: Ali Eren Yogurtcu
Architecture: all
Description: FastFingers is an training application \
that helps its users to remove the habit of using a mouse, \
by helping them to memorize the shortcuts they want to \
learn by practicing." > ${path}/packages/debian/fastfingers/DEBIAN/control

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
