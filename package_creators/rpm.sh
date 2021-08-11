if [ ${1} != "" ]
then
    path=${1}
else
    path=${PWD}
fi

sudo dnf install -y rpmdevtools rpmlint cmake gtk3-devel

rpmdev-wipetree
rpmdev-setuptree
cp ${path}/package_creators/fastfingers.spec ~/rpmbuild/SPECS
cp -rv ${path}/* ~/rpmbuild/BUILD/
