#!/bin/bash
echo "===================================="
echo "Cleaning Systemwide Installed Files"
echo "===================================="
echo "1#START - Removing binaries"
rm -v /usr/local/bin/fastfingers
rm -v /usr/local/bin/fastfingers-cheatsheet
rm -v /usr/bin/fastfingers
rm -v /usr/bin/fastfingers-cheatsheet
echo "1#END - Removing binaries"
echo "===================================="
echo "2#START - Removing data files"
rm -rf -v /usr/share/fastfingers/
echo "2#END - Removing data files"
echo "===================================="
echo "3#START - Removing desktop files"
rm -v /usr/share/applications/org.ccextractor.FastFingers.desktop 
rm -v /usr/share/applications/org.ccextractor.Cheatsheet.desktop
echo "3#END - Removing desktop files"
echo "===================================="
echo "4#START - Removing GSettings file"
glib_prefix=`pkg-config glib-2.0 --variable prefix`
schemas_dir="$glib_prefix/share/glib-2.0/schemas"
rm -v $schemas_dir/org.ccextractor.FastFingers.gschema.xml
echo "4#END - Removing GSettings file"
echo "===================================="
echo "5#START - Removing icons"
for px in 16 32 48 64 128
do
    rm -v /usr/share/icons/hicolor/{$px}x{$px}/apps/fastfingers.png
    rm -v /usr/share/icons/hicolor/{$px}x{$px}/apps/fastfingers-cheatsheet.png
done
gtk-update-icon-cache -t -f /usr/share/icons/hicolor/
for px in 16 22 24 32 48 256
do
    rm -v /usr/share/icons/HighContrast/${px}x${px}/apps/fastfingers.png
    rm -v /usr/share/icons/HighContrast/${px}x${px}/apps/fastfingers-cheatsheet.png
    gtk-update-icon-cache -t -f /usr/share/icons/HighContrast/
done
echo "5#END - Removing icons"
echo "===================================="