.PHONY: build clean install-svr install-agent uninstall-svr uninstall-agent

RALT_SVR_ROOT = /opt/reyzar/ralt-svr
RALT_AGENT_ROOT = /opt/reyzar/ralt-agent
SYSTEM_ROOT = /usr/lib/systemd/system

build:
	cd src/ && make proto && make
	
clean:
	cd src/ && make clean

install-svr:build
	mkdir -p $(RALT_SVR_ROOT)
	cp src/ralt-svr $(RALT_SVR_ROOT)
	cp service/ralt-svr.service $(SYSTEM_ROOT) -f

install-agent:build
	mkdir -p $(RALT_AGENT_ROOT)
	cp src/ralt-agent $(RALT_AGENT_ROOT)
	cp service/ralt-agent.service $(SYSTEM_ROOT) -f

uninstall-svr:
	service ralt-svr stop
	rm $(RALT_SVR_ROOT)/ralt-svr -rf
	rm $(SYSTEM_ROOT)/ralt-svr.service

uninstall-agent:
	service ralt-agent stop
	rm $(RALT_AGENT_ROOT)/ralt-agent -rf
	rm $(SYSTEM_ROOT)/ralt-agent.service