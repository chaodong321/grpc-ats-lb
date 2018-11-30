Name:           ralt-agent
Version:        1.0
Release:        1
Summary:        Reyzar Ralt Agent
Vendor :        Reyzar

Group:          System Environment/Daemons
License:        All rights reserved
URL:            http://www.reyzar.net

Source:         ralt-agent.tar.gz

%description
Reyzar Ralt Agent.

%prep
%setup -n %{name}

%install
mkdir -p ${RPM_BUILD_ROOT}/opt/reyzar/ralt-agent/
cp -f ralt-agent  ${RPM_BUILD_ROOT}/opt/reyzar/ralt-agent/
chmod +x ${RPM_BUILD_ROOT}/opt/reyzar/ralt-agent/ralt-agent
cp -f ralt-agent.json  ${RPM_BUILD_ROOT}/opt/reyzar/ralt-agent/
cp -f ralt-agent.service ${RPM_BUILD_ROOT}/opt/reyzar/ralt-agent/
chmod +x ${RPM_BUILD_ROOT}/opt/reyzar/ralt-agent/ralt-agent.service

%post
mkdir -p ${RPM_BUILD_ROOT}/etc/init.d
chmod +x ${RPM_BUILD_ROOT}/opt/reyzar/ralt-agent/ralt-agent
chmod +x ${RPM_BUILD_ROOT}/opt/reyzar/ralt-agent/ralt-agent.service
ln -s ${RPM_BUILD_ROOT}/opt/reyzar/ralt-agent/ralt-agent.service ${RPM_BUILD_ROOT}/etc/init.d/ralt-agent

echo chkconfig ralt-agent on...
chkconfig ralt-agent on

%preun
rm -f /opt/reyzar/ralt-agent/ralt-agent
rm -f /etc/init.d/ralt-agent

%files
/opt/reyzar/ralt-agent/ralt-agent
/opt/reyzar/ralt-agent/ralt-agent.json
/opt/reyzar/ralt-agent/ralt-agent.service