#include "ralt-agent-impl.h"
#include "util-pidfile.h"
#include "util-daemon.h"

#define RALT_AGENT_PID_FILENAME "/var/run/ralt-agent.pid"

int MayDaemonize()
{
    if (PidfileTestRunning(RALT_SERVICE_PID_FILENAME) != 0) {
		printf("ralt service is running!\n");
        return -1;
    }

	Daemonize();

    if (PidfileCreate(RALT_SERVICE_PID_FILENAME) != 0) {
		printf("create pid file error!\n");
		return -1;
    }

    return 0;
}


void RunServer() {
  std::string server_address("0.0.0.0:50052");
  RaltServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  server->Wait();
}

int main(int argc, char *argv[])
{
#if 0
	if(MayDaemonize() != 0){
		return 1;
	}
#endif

    RunServer();
    return 0;
}
