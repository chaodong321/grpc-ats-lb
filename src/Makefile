.PHONY: all clean

CC = g++ -g
CXXFLAGS := -std=c++11
LDFLAGS := -L. 
LIBS := -lgrpc++ -lprotobuf -ljansson -lpthread -lgrpc++_reflection
SERVER := ralt-svr
CLIENT := ralt-svr-client

COMM_SRCS = ralt-service.grpc.pb.o ralt-service.pb.o util-log.o util-common.o util-pidfile.o util-daemon.o

SERVER_SRCS =  ralt-service-impl.o ralt-domain.o misc-conf.o ralt-service-server.o

CLIENT_SRCS = ralt-service-client.o

all: $(SERVER) $(CLIENT)

$(SERVER): $(COMM_SRCS) $(SERVER_SRCS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

$(CLIENT):$(COMM_SRCS) $(CLIENT_SRCS)
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -o $@ $^

%.o:%.cc
	$(CC) $(CXXFLAGS) $(LDFLAGS) $(LIBS) -c $< -o $@

proto:
	protoc -I ../proto --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` ../proto/ralt-service.proto
	protoc -I ../proto --cpp_out=. ../proto/ralt-service.proto

clean:
	rm $(SERVER) $(CLIENT) $(COMM_SRCS) $(SERVER_SRCS) $(CLIENT_SRCS) -f