if [ ${1} != "" ]
then
    path=${1}
else
    path=${PWD}
fi

rpmdev-wipetree
rpmdev-setuptree
cp ${path}/package_creators/fastfingers.spec ~/rpmbuild/SPECS
cp -rv ${path}/* ~/rpmbuild/SOURCES/
rpmbuild -bb ~/rpmbuild/SPECS/fastfingers.spec

sudo rpm -e fastfingers && sudo rpm -ivh ~/rpmbuild/RPMS/x86_64/fastfingers-0.7.2-1.el8.x86_64.rpm