Name:           ralt-svr
Version:        1.0
Release:        1
Summary:        Reyzar Ralt Service
Vendor :        Reyzar

Group:          System Environment/Daemons
License:        All rights reserved
URL:            http://www.reyzar.net

Source:         ralt-svr.tar.gz

%description
Reyzar Ralt Service.

%prep
%setup -n %{name}

%install
mkdir -p ${RPM_BUILD_ROOT}/opt/reyzar/ralt-svr/
cp -f ralt-svr  ${RPM_BUILD_ROOT}/opt/reyzar/ralt-svr/
chmod +x ${RPM_BUILD_ROOT}/opt/reyzar/ralt-svr/ralt-svr
cp -f ralt-svr.json  ${RPM_BUILD_ROOT}/opt/reyzar/ralt-svr/
cp -f ralt-svr.service ${RPM_BUILD_ROOT}/opt/reyzar/ralt-svr/
chmod +x ${RPM_BUILD_ROOT}/opt/reyzar/ralt-svr/ralt-svr.service

%post
mkdir -p ${RPM_BUILD_ROOT}/etc/init.d
chmod +x ${RPM_BUILD_ROOT}/opt/reyzar/ralt-svr/ralt-svr
chmod +x ${RPM_BUILD_ROOT}/opt/reyzar/ralt-svr/ralt-svr.service
ln -s ${RPM_BUILD_ROOT}/opt/reyzar/ralt-svr/ralt-svr.service ${RPM_BUILD_ROOT}/etc/init.d/ralt-svr

echo chkconfig ralt-svr on...
chkconfig ralt-svr on

%preun
rm -f /opt/reyzar/ralt-svr/ralt-svr
rm -f /etc/init.d/ralt-svr

%files
/opt/reyzar/ralt-svr/ralt-svr
/opt/reyzar/ralt-svr/ralt-svr.json
/opt/reyzar/ralt-svr/ralt-svr.service