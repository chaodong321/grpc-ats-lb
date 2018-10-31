.PHONY: build clean install uninstall

INSTALL_ROOT = /opt/reyzar/ralt-svr
SYSTEM_ROOT = /usr/lib/systemd/system

build:
	cd src/ && make proto && make
	
clean:
	cd src/ && make clean

install-svr:build
	mkdir -p $(INSTALL_ROOT)
	cp src/ralt-svr $(INSTALL_ROOT)
	cp service/ralt-svr.service $(SYSTEM_ROOT) -f

install-agent:build
	mkdir -p $(INSTALL_ROOT)
	cp src/ralt-agent $(INSTALL_ROOT)
	cp service/ralt-agent.service $(SYSTEM_ROOT) -f

uninstall-svr:
	service ralt-svr stop
	rm $(INSTALL_ROOT) -rf
	rm $(SYSTEM_ROOT)/ralt-svr.service

uninstall-agent:
	service ralt-agent stop
	rm $(INSTALL_ROOT) -rf
	rm $(SYSTEM_ROOT)/ralt-agent.service