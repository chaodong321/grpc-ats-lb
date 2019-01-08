
#include <jansson.h>
#include "ralt-service-conf.h"
#include "util-log.h"
#include "util-common.h"


#define DEFAULT_SERVER_IP "0.0.0.0"
#define DEFAULT_SERVER_PORT 50052

RaltServiceConf::RaltServiceConf()
{
}

RaltServiceConf::~RaltServiceConf()
{
}

RaltServiceConf& RaltServiceConf::GetInstance()
{
    static RaltServiceConf instance;
    return instance;
}

bool RaltServiceConf::LoadConf(string strConfPath)
{
    LOG_INFO("config path: %s", strConfPath.c_str());
    if(strConfPath.empty()){
        LOG_ERROR("config path is emtpy");
        return false;
    }

    json_t *j_obj = json_load_file(strConfPath.c_str(),0,NULL);
    if(NULL == j_obj){
        LOG_ERROR("Error:load json file(%s) failed.", strConfPath.c_str()); 
        return false;
    }

    json_t *j_ip_addr = json_object_get(j_obj,"ServiceIp");
    if(NULL != j_ip_addr && NULL != json_string_value(j_ip_addr)){
        strIpAddr = json_string_value(j_ip_addr);
        LOG_INFO("ip addr: %s", strIpAddr.c_str());
    }else{
        strIpAddr = DEFAULT_SERVER_IP;
        LOG_INFO("default ip: %s", strIpAddr.c_str());
    }

    json_t *j_port = json_object_get(j_obj,"ServicePort");
    if(NULL != j_port){
        nPort = json_integer_value(j_port);
        LOG_INFO("port: %d", nPort);
    }else{
        nPort = DEFAULT_SERVER_PORT;
        LOG_INFO("default port: %d", nPort);
    }

    json_decref(j_obj);
    LOG_ERROR("load config success!");
    return true;
}

string RaltServiceConf::GetIpAddr()
{
    return strIpAddr;
}

unsigned int RaltServiceConf::GetPort()
{
    return nPort;
}

