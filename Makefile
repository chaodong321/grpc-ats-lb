.PHONY: build clean install-svr install-agent uninstall-svr uninstall-agent rpm-svr

RPMBUILD_DIR = $(shell pwd)/rpmbuild
RALT_SVR_ROOT=/opt/reyzar/ralt-svr
RALT_AGENT_ROOT=/opt/reyzar/ralt-agent
INITD_ROOT=/etc/init.d

build:
	cd src/ && make proto && make

clean:
	cd src/ && make clean

install-svr:build
	mkdir -p $(RALT_SVR_ROOT)
	cp -f src/ralt-svr $(RALT_SVR_ROOT)
	cp -f conf/ralt-svr.json $(RALT_SVR_ROOT)
	cp -f service/ralt-svr.service $(RALT_SVR_ROOT)
	chmod +x $(RALT_SVR_ROOT)/ralt-svr
	chmod +x $(RALT_SVR_ROOT)/ralt-svr.service
	mkdir $(INITD_ROOT) -p
	ln -sf $(RALT_SVR_ROOT)/ralt-svr.service $(INITD_ROOT)/ralt-svr
	chkconfig ralt-svr on

install-agent:build
	mkdir -p $(RALT_AGENT_ROOT)
	cp -f src/ralt-agent $(RALT_AGENT_ROOT)
	cp -f conf/ralt-agent.json $(RALT_AGENT_ROOT)
	cp -f service/ralt-agent.service $(RALT_AGENT_ROOT)
	chmod +x $(RALT_AGENT_ROOT)/ralt-agent
	chmod +x $(RALT_AGENT_ROOT)/ralt-agent.service
	mkdir -p $(INITD_ROOT)
	ln -sf $(RALT_AGENT_ROOT)/ralt-agent.service $(INITD_ROOT)/ralt-agent
	chkconfig ralt-agent on

uninstall-svr:
	service ralt-svr stop
	rm -rf $(RALT_SVR_ROOT)/ralt-svr
	chkconfig ralt-svr off
	rm $(INITD_ROOT)/ralt-svr

uninstall-agent:
	service ralt-agent stop
	rm -rf $(RALT_AGENT_ROOT)/ralt-agent
	chkconfig ralt-agent off
	rm $(INITD_ROOT)/ralt-agent

rpm-svr: build
	touch ~/.rpmmacros
	mv ~/.rpmmacros ~/.rpmmacros.bak
	echo '' > ~/.rpmmacros
	echo "%_topdir $(RPMBUILD_DIR)" >> ~/.rpmmacros
	echo "%__os_install_post %{nil}" >> ~/.rpmmacros
	mkdir -p $(RPMBUILD_DIR)/BUILD/ralt-svr
	cp -f src/ralt-svr $(RPMBUILD_DIR)/BUILD/ralt-svr
	cp -f conf/ralt-svr.json $(RPMBUILD_DIR)/BUILD/ralt-svr
	cp -f service/ralt-svr.service $(RPMBUILD_DIR)/BUILD/ralt-svr
	tar -C $(RPMBUILD_DIR)/BUILD/ -zcvf $(RPMBUILD_DIR)/SOURCES/ralt-svr.tar.gz ralt-svr
	rpmbuild -bb $(RPMBUILD_DIR)/SPECS/ralt-svr.spec
	mv $(RPMBUILD_DIR)/RPMS/x86_64/*.rpm .
	
rpm-agent: build
	touch ~/.rpmmacros
	mv ~/.rpmmacros ~/.rpmmacros.bak
	echo '' > ~/.rpmmacros
	echo "%_topdir $(RPMBUILD_DIR)" >> ~/.rpmmacros
	echo "%__os_install_post %{nil}" >> ~/.rpmmacros
	mkdir -p $(RPMBUILD_DIR)/BUILD/ralt-agent
	cp -f src/ralt-agent $(RPMBUILD_DIR)/BUILD/ralt-agent
	cp -f conf/ralt-agent.json $(RPMBUILD_DIR)/BUILD/ralt-agent
	cp -f service/ralt-agent.service $(RPMBUILD_DIR)/BUILD/ralt-agent
	tar -C $(RPMBUILD_DIR)/BUILD/ -zcvf $(RPMBUILD_DIR)/SOURCES/ralt-agent.tar.gz ralt-agent
	rpmbuild -bb $(RPMBUILD_DIR)/SPECS/ralt-agent.spec
	mv $(RPMBUILD_DIR)/RPMS/x86_64/*.rpm .