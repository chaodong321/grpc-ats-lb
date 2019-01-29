.PHONY: all clean

CC = g++
CXXFLAGS := -g -Wall -std=c++11
LDFLAGS := -L. 
LIBS := -lgrpc++ -lgrpc++_reflection -lprotobuf -ljansson -lpthread

SERVER := ralt-svr
CLIENT := ralt-client
AGENT := ralt-agent

COMM_SRCS = sys-info.grpc.pb.o sys-info.pb.o ralt-service.grpc.pb.o ralt-service.pb.o util-log.o util-common.o util-pidfile.o util-daemon.o

SERVER_SRCS = sys-info-impl.o ralt-service-impl.o ralt-domain.o ralt-rule.o ralt-cache.o misc-conf.o ralt-service-conf.o ralt-service-server.o

CLIENT_SRCS = ralt-service-client.o

AGENT_SRCS = sys-agent-impl.o ralt-agent-impl.o ralt-agent-conf.o ralt-agent.o

all: $(SERVER) $(CLIENT) $(AGENT)

$(SERVER): $(COMM_SRCS) $(SERVER_SRCS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

$(CLIENT):$(COMM_SRCS) $(CLIENT_SRCS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

$(AGENT): $(COMM_SRCS) $(AGENT_SRCS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

%.o:%.cc
	$(CC) $(CXXFLAGS) -c $< -o $@

proto:
	protoc -I ../proto --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../proto/sys-info.proto
	protoc -I ../proto --cpp_out=.  ../proto/sys-info.proto
	protoc -I ../proto --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../proto/ralt-service.proto
	protoc -I ../proto --cpp_out=.  ../proto/ralt-service.proto

clean:
	rm $(COMM_SRCS) $(SERVER_SRCS) $(SERVER) $(CLIENT_SRCS) $(CLIENT) $(AGENT_SRCS) $(AGENT) -f

