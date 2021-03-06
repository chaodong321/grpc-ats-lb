#include <unistd.h>
#include "sys-agent-impl.h"
#include "ralt-agent-impl.h"
#include "ralt-agent-conf.h"
#include "util-pidfile.h"
#include "util-daemon.h"
#include "util-log.h"

#define RALT_AGENT_CONF_PATH "/opt/reyzar/ralt-agent/ralt-agent.json"
#define RALT_AGENT_PID_FILENAME "/var/run/ralt-agent.pid"
#define LOG_FILE_NAME "/opt/reyzar/ralt-agent/log/ralt-agent.log"
#define LOG_BUF_SIZE 2048
#define LOG_SPLIT_SIZE 200000
#define LOG_MAX_QUEUE_SIZE 0

int MayDaemonize()
{
    if (PidfileTestRunning(RALT_AGENT_PID_FILENAME) != 0) {
        printf("ralt service is running!\n");
        return -1;
    }

    //Daemonize();

    if (PidfileCreate(RALT_AGENT_PID_FILENAME) != 0) {
        printf("create pid file error!\n");
        return -1;
    }

    return 0;
}

void RunServer() {

    string strAgent = RaltAgentConf::GetInstance().GetIpAddr() + ":"
        + to_string(RaltAgentConf::GetInstance().GetPort());
    
    SysAgentImpl sys_agent;
    RaltAgentImpl ralt_agent;
    
    ServerBuilder builder;
    builder.AddListeningPort(strAgent, grpc::InsecureServerCredentials());
    builder.RegisterService(&sys_agent);
    builder.RegisterService(&ralt_agent);
    unique_ptr<Server> server(builder.BuildAndStart());

    LOG_INFO("ralt agent listening on: %s", strAgent.c_str());

    server->Wait();
}

int main(int argc, char *argv[])
{
    if(MayDaemonize() != 0){
        return 0;
    }

    LOG_INIT(LOG_FILE_NAME, LOG_BUF_SIZE, LOG_SPLIT_SIZE, LOG_MAX_QUEUE_SIZE);

    if(!RaltAgentConf::GetInstance().LoadConf(RALT_AGENT_CONF_PATH)){
        return 0;
    }

    RunServer();

    LOG_INFO("ralt agent exit");
   return 0;
}

