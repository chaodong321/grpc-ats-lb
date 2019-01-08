
#include <unistd.h>
#include "ralt-service-impl.h"
#include "ralt-service-conf.h"
#include "sys-info-impl.h"
#include "util-pidfile.h"
#include "util-daemon.h"
#include "util-log.h"
#include "ralt-service-conf.h"


#define RALT_SERVICE_CONF_PATH "/opt/reyzar/ralt-svr/ralt-svr.json"
#define RALT_SERVICE_PID_FILENAME "/var/run/ralt-svr.pid"
#define LOG_FILE_NAME "/opt/reyzar/ralt-svr/log/ralt-service.log"
#define LOG_BUF_SIZE 2048
#define LOG_SPLIT_SIZE 200000
#define LOG_MAX_QUEUE_SIZE 0


int MayDaemonize()
{
    if (PidfileTestRunning(RALT_SERVICE_PID_FILENAME) != 0) {
        printf("ralt service is running!\n");
        return -1;
    }

    //Daemonize();

    if (PidfileCreate(RALT_SERVICE_PID_FILENAME) != 0) {
        printf("create pid file error!\n");
        return -1;
    }

    return 0;
}


void RunServer() {

    string strService = RaltServiceConf::GetInstance().GetIpAddr() + ":"
        + to_string(RaltServiceConf::GetInstance().GetPort());
    
    RaltServiceImpl ralt_service;
    SysInfoImpl sys_info_service;

    ServerBuilder builder;
    builder.AddListeningPort(strService, grpc::InsecureServerCredentials());
    builder.RegisterService(&ralt_service);
    builder.RegisterService(&sys_info_service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    LOG_INFO("ralt server listening on: %s", strService.c_str());

    server->Wait();
}

int main(int argc, char *argv[])
{
    if(MayDaemonize() != 0){
        return 0;
    }
    
    LOG_INIT(LOG_FILE_NAME, LOG_BUF_SIZE, LOG_SPLIT_SIZE, LOG_MAX_QUEUE_SIZE);

    if(!RaltServiceConf::GetInstance().LoadConf(RALT_SERVICE_CONF_PATH)){
        return 0;
    }

    RunServer();

    LOG_INFO("ralt service exit");
    return 0;
}
