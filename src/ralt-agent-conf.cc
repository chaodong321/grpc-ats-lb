
#include <jansson.h>
#include "ralt-agent-conf.h"
#include "util-log.h"
#include "util-common.h"


#define DEFAULT_AGENT_IP "0.0.0.0"
#define DEFAULT_AGENT_PORT 50052

RaltAgentConf::RaltAgentConf()
{
}

RaltAgentConf::~RaltAgentConf()
{
}

RaltAgentConf& RaltAgentConf::GetInstance()
{
    static RaltAgentConf instance;
    return instance;
}

bool RaltAgentConf::LoadConf(string strConfPath)
{
    LOG_INFO("config path: %s", strConfPath.c_str());
    if(strConfPath.empty()){
        LOG_ERROR("config path is emtpy");
        return false;
    }

    json_t *j_obj = json_load_file(strConfPath.c_str(), 0, NULL);
    if(NULL == j_obj){
        LOG_ERROR("load json file(%s) failed.", strConfPath.c_str()); 
        return false;
    }

    json_t *j_ip_addr = json_object_get(j_obj, "AgentIp");
    if(NULL != j_ip_addr && NULL != json_string_value(j_ip_addr)){
        strIpAddr = string(json_string_value(j_ip_addr));
        LOG_INFO("ip addr: %s", strIpAddr.c_str());
    }else{
        strIpAddr = DEFAULT_AGENT_IP;
        LOG_INFO("default ip: %s", strIpAddr.c_str());
    }
    
    json_t *j_port = json_object_get(j_obj, "AgentPort");
    if(NULL != j_port && 0 != json_integer_value(j_port)){
        nPort = json_integer_value(j_port);
        LOG_INFO("port: %d", nPort);
    }else{
        nPort = DEFAULT_AGENT_PORT;
        LOG_INFO("default port: %d", nPort);
    }

    json_t *j_servers = json_object_get(j_obj, "Server");
    if(NULL != j_servers){
        unsigned int size = json_array_size(j_servers);
        LOG_INFO("server number: %u", size);

        unsigned int index = 0;
        for(index = 0; index < size; index++){
            RaltServer server;
            json_t *j_server = json_array_get(j_servers,index);
            if(NULL == j_server)
                continue;

            json_t *j_service_ip = json_object_get(j_server, "ServiceIp");
            if(NULL == j_service_ip || NULL == json_string_value(j_service_ip))
                continue;

            server.strIpAddr = string(json_string_value(j_service_ip));
            if(server.strIpAddr.empty())
                continue;

            json_t *j_service_port = json_object_get(j_server, "ServicePort");
            if(NULL == j_service_ip)
                continue;

            server.nPort = json_integer_value(j_service_port);
            if(server.nPort == 0)
                continue;

            LOG_INFO("server ip addr: %s", server.strIpAddr.c_str());
            LOG_INFO("server port: %d", server.nPort);
            vRaltServer.push_back(server);
        }
    }else{
        LOG_ERROR("no config server!");
        return false;
    }

    json_decref(j_obj);
    LOG_ERROR("load config success!");
    return true;
}

string RaltAgentConf::GetIpAddr()
{
    return strIpAddr;
}

unsigned int RaltAgentConf::GetPort()
{
    return nPort;
}

vector<RaltServer>& RaltAgentConf::GetServer()
{
    return vRaltServer;
}

