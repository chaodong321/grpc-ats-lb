#include "util-common.h"
#include "util-log.h"
#include "sys-info-impl.h"
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>  
#include <string.h>  
#include <sys/stat.h>  
#include <stdlib.h>
#include <jansson.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <memory>
#include <map>

#define ATS_ROOT_DIR "/opt/reyzar/can/"
#define ATS_LOG_DIR "/opt/reyzar/can/var/log/trafficserver/"



Status SysInfoImpl::getNameAndIpInfo (ServerContext* context, const GetNameAndIpInfoReq* request, GetNameAndIpInfoRsp* reply) 
{
    LOG_INFO("get host name and ip info");
    string strHostName;
    string strHostNameCmd = "hostname";
    if(!UtilCommon::ShellCmd(strHostNameCmd, strHostName)){
        LOG_ERROR("shell cmd failed: %s\n", strHostNameCmd.c_str());
        return Status::CANCELLED;
    }
    LOG_INFO("host name:%s", strHostName.c_str());
    reply->set_host_name(strHostName);

    string strIpInfo;
    string strIpInfoCmd = "ip address show";
    if(!UtilCommon::ShellCmd(strIpInfoCmd, strIpInfo)){
        LOG_ERROR("shell cmd failed: %s\n", strIpInfoCmd.c_str());
        return Status::CANCELLED;
    }
    LOG_INFO("ip info:%s", strIpInfo.c_str());
    reply->set_ip_info(strIpInfo);
    
    LOG_INFO("get host name successfully");
    return Status::OK;
}


Status SysInfoImpl::getDeviceInfo (ServerContext* context, const GetDeviceInfoReq* request, GetDeviceInfoRsp* reply)
{
    LOG_INFO("get device info");
    
    string strCpuModel;
    string strCpuModelCmd = "cat /proc/cpuinfo |grep model\\\\s*name |uniq -w1";
    if(!UtilCommon::ShellCmd(strCpuModelCmd, strCpuModel)){
        LOG_ERROR("shell cmd failed: %s\n", strCpuModelCmd.c_str());
        return Status::CANCELLED;
    }
    LOG_INFO("cpu model:%s", strCpuModel.c_str());
    reply->set_cpu_model(strCpuModel);

    string strCpuCores;
    string strCpuCoresCmd = "cat /proc/cpuinfo |grep cpu\\\\s*cores |uniq -w1";
    if(!UtilCommon::ShellCmd(strCpuCoresCmd, strCpuCores)){
        LOG_ERROR("shell cmd failed: %s\n", strCpuCoresCmd.c_str());
        return Status::CANCELLED;
    }
    LOG_INFO("cpu cores:%s", strCpuCores.c_str());
    reply->set_cpu_cores(strCpuCores);

    string strMemTotal;
    string strMemTotalCmd = "cat /proc/meminfo |grep MemTotal";
    if(!UtilCommon::ShellCmd(strMemTotalCmd, strMemTotal)){
        LOG_ERROR("shell cmd failed: %s\n", strMemTotalCmd.c_str());
        return Status::CANCELLED;
    }
    LOG_INFO("mem total:%s", strMemTotal.c_str());
    reply->set_mem_total(strMemTotal);

    string strEthCtrlInfo;
    string strEthCtrlInfoCmd = "lspci | grep Ethernet";
    if(!UtilCommon::ShellCmd(strEthCtrlInfoCmd, strEthCtrlInfo)){
        LOG_ERROR("shell cmd failed: %s\n", strEthCtrlInfoCmd.c_str());
        return Status::CANCELLED;
    }
    LOG_INFO("ethernet controler info:%s", strEthCtrlInfo.c_str()); 
    reply->set_eth_ctrl_info(strEthCtrlInfo);
    
    LOG_INFO("get device info successfully");
    return Status::OK;
}

Status SysInfoImpl::getDeviceDetail (ServerContext* context, const GetDeviceDetailReq* request, GetDeviceDetailRsp* reply)
{
    LOG_INFO("get device detail");
    string strCpuUsage;
    string strCpuUsageCmd = "mpstat -P ALL |grep -v CPU | awk 'NF > 4' | awk '{for(i=1;i<NF;i++)if($i!~\"AM|PM\")printf(\"%s \",$i);print $NF}' |awk '{print $2,$NF}'";
    if(!UtilCommon::ShellCmd(strCpuUsageCmd, strCpuUsage)){
        LOG_ERROR("shell cmd failed: %s\n", strCpuUsageCmd.c_str());
        return Status::CANCELLED;
    }
    LOG_INFO("cpu usage:%s", strCpuUsage.c_str());
    reply->set_cpu_usage(strCpuUsage);

    string strMemUsage;
    string strMemUsageCmd = "free -m | grep -v total | grep -v cache | awk '{print $1,$2,$3,$4}'";
    if(!UtilCommon::ShellCmd(strMemUsageCmd, strMemUsage)){
        LOG_ERROR("shell cmd failed: %s\n", strMemUsageCmd.c_str());
        return Status::CANCELLED;
    }
    LOG_INFO("memory usage: %s", strMemUsage.c_str());
    reply->set_mem_usage(strMemUsage);

    string strCpuTemp;
    string strCpuTempCmd = "sensors | grep Core |awk '{print $2,$3}'";
    if(!UtilCommon::ShellCmd(strCpuTempCmd, strCpuTemp)){
        LOG_ERROR("shell cmd failed: %s\n", strCpuTempCmd.c_str());
        return Status::CANCELLED;
    }
    LOG_INFO("cpu temperature:%s", strCpuTemp.c_str());
    reply->set_cpu_temp(strCpuTemp);

    bool bDependOsVersion = request->is_depend_os_version();
    string strSysVer;
    string strSysVerCmd = "cat /etc/redhat-release|sed -r 's/.* ([0-9]+)\\..*/\\1/'";
    if(!UtilCommon::ShellCmd(strSysVerCmd, strSysVer)){
        LOG_ERROR("shell cmd error: %s", strSysVerCmd.c_str());
        return Status::CANCELLED;
    }
    string strNicInfoCmd, strNicInfo;
    if(bDependOsVersion && strSysVer.find("6") != string::npos){
        LOG_INFO("depend os, and os version: centos6");
        reply->set_os_version("centos6");
        strNicInfoCmd = "ifconfig -a |awk NF |grep -v collisions |grep -v Memory";
    }
    else if(bDependOsVersion && strSysVer.find("7") != string::npos){
        LOG_INFO("depend os, and os version: centos7");
        reply->set_os_version("centos7");
        strNicInfoCmd = "ifconfig";
    }
    else if( !bDependOsVersion && strSysVer.find("6") != string::npos){
        LOG_INFO("no depend os, and os version: centos6");
        reply->set_os_version("centos6");
        strNicInfoCmd = "ifconfig";
    }
    else if( !bDependOsVersion && strSysVer.find("7") != string::npos){
        LOG_INFO("no depend os, and os version: centos7");
        reply->set_os_version("centos7");
        strNicInfoCmd = "ifconfig";
    }
    else{
        LOG_WARN("the system version is not centos6 and centos7");
        return Status::CANCELLED;
    }
    if(!UtilCommon::ShellCmd(strNicInfoCmd, strNicInfo)){
        LOG_ERROR("shell cmd error: %s", strNicInfoCmd.c_str());
        return Status::CANCELLED;
    }
    reply->set_nic_info(strNicInfo);
    LOG_INFO("network info:%s", strNicInfo.c_str());
    
    LOG_INFO("get device detail successfully");
    return Status::OK;
}


Status SysInfoImpl::getCpuUsage (ServerContext* context, const GetCpuUsageReq* request, GetCpuUsageRsp* reply)
{
    LOG_INFO("get cpu usage");
    string strValue;
    string strCmd = "mpstat -P ALL |grep -v CPU | awk 'NF > 4' | awk '{for(i=1;i<NF;i++)if($i!~\"AM|PM\")printf(\"%s \",$i);print $NF}' |awk '{print $2,$NF}'";
    
    if(!UtilCommon::ShellCmd(strCmd, strValue)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return Status::CANCELLED;
    }

    LOG_INFO("cpu usage:%s", strValue.c_str());
    reply->set_cpu_usage(strValue);
    LOG_INFO("get cpu usage successfully");
    return Status::OK;
}

Status SysInfoImpl::getCpuTemp (ServerContext* context, const GetCpuTempReq* request, GetCpuTempRsp* reply)
{
    LOG_INFO("get cpu temperature");
    string strValue;
    string strCmd = "sensors | grep Core |awk '{print $2,$3}'";
    
    if(!UtilCommon::ShellCmd(strCmd, strValue)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return Status::CANCELLED;
    }

    LOG_INFO("cpu temperature:%s", strValue.c_str());
    reply->set_cpu_temp(strValue);
    LOG_INFO("get cpu temperature successfully");
    return Status::OK;
}

Status SysInfoImpl::getMemUsage (ServerContext* context, const GetMemUsageReq* request, GetMemUsageRsp* reply)
{
    LOG_INFO("get memory usage");
    string strValue;
    string strCmd = "free -m | grep -v total | grep -v cache | awk '{print $1,$2,$3,$4}'";
    
    if(!UtilCommon::ShellCmd(strCmd, strValue)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return Status::CANCELLED;
    }

    LOG_INFO("memory usage: %s", strValue.c_str());
    reply->set_mem_usage(strValue);
    LOG_INFO("get memory usage successfully");
    return Status::OK;
}

Status SysInfoImpl::getNicInfo (ServerContext* context, const GetNicInfoReq* request, GetNicInfoRsp* reply)
{
    LOG_INFO("get network card info");
    bool bDependOsVersion = request->is_depend_os_version();
    string strSysVer;
    string strSysVerCmd = "cat /etc/redhat-release|sed -r 's/.* ([0-9]+)\\..*/\\1/'";
    if(!UtilCommon::ShellCmd(strSysVerCmd, strSysVer)){
        LOG_ERROR("shell cmd error: %s", strSysVerCmd.c_str());
        return Status::CANCELLED;
    }

    string strNicInfoCmd, strNicInfo;
    if(bDependOsVersion && strSysVer.find("6") != string::npos){
        LOG_INFO("depend os, and os version: centos6");
        reply->set_os_version("centos6");
        strNicInfoCmd = "ifconfig -a |awk NF |grep -v collisions |grep -v Memory";
    }
    else if(bDependOsVersion && strSysVer.find("7") != string::npos){
        LOG_INFO("depend os, and os version: centos7");
        reply->set_os_version("centos7");
        strNicInfoCmd = "ifconfig";
    }
    else if( !bDependOsVersion && strSysVer.find("6") != string::npos){
        LOG_INFO("no depend os, and os version: centos6");
        reply->set_os_version("centos6");
        strNicInfoCmd = "ifconfig";
    }
    else if( !bDependOsVersion && strSysVer.find("7") != string::npos){
        LOG_INFO("no depend os, and os version: centos7");
        reply->set_os_version("centos7");
        strNicInfoCmd = "ifconfig";
    }
    else{
        LOG_WARN("the system version is not centos6 and centos7");
        return Status::CANCELLED;
    }

    if(!UtilCommon::ShellCmd(strNicInfoCmd, strNicInfo)){
        LOG_ERROR("shell cmd error: %s", strNicInfoCmd.c_str());
        return Status::CANCELLED;
    }
    
    reply->set_nic_info(strNicInfo);

    LOG_INFO("get network card info successfully");
    return Status::OK;
}

Status SysInfoImpl::getHostName (ServerContext* context, const GetHostNameReq* request, GetHostNameRsp* reply) 
{
    LOG_INFO("get host name");
    string strValue;
    string strCmd = "hostname";
    
    if(!UtilCommon::ShellCmd(strCmd, strValue)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return Status::CANCELLED;
    }

    LOG_INFO("host name:%s", strValue.c_str());
    reply->set_host_name(strValue);
    LOG_INFO("get host name successfully");
    return Status::OK;
}

Status SysInfoImpl::getIpInfo (ServerContext* context, const GetIpInfoReq* request, GetIpInfoRsp* reply) 
{
    LOG_INFO("get ip info");
    string strValue;
    string strCmd = "ip address show";
    
    if(!UtilCommon::ShellCmd(strCmd, strValue)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return Status::CANCELLED;
    }

    LOG_INFO("ip info:%s", strValue.c_str());
    reply->set_ip_info(strValue);
    LOG_INFO("get ip info successfully");
    return Status::OK;
}

Status SysInfoImpl::getCpuModel (ServerContext* context, const GetCpuModelReq* request, GetCpuModelRsp* reply) 
{
    LOG_INFO("get cpu model");
    string strValue;
    string strCmd = "cat /proc/cpuinfo |grep model\\\\s*name |uniq -w1";
    
    if(!UtilCommon::ShellCmd(strCmd, strValue)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return Status::CANCELLED;
    }

    LOG_INFO("cpu model:%s", strValue.c_str());
    reply->set_cpu_model(strValue);
    LOG_INFO("get cpu model successfully");
    return Status::OK;
}
    
Status SysInfoImpl::getCpuCores (ServerContext* context, const GetCpuCoresReq* request, GetCpuCoresRsp* reply)
{
    LOG_INFO("get cpu cores");
    string strValue;
    string strCmd = "cat /proc/cpuinfo |grep cpu\\\\s*cores |uniq -w1";
    
    if(!UtilCommon::ShellCmd(strCmd, strValue)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return Status::CANCELLED;
    }

    LOG_INFO("cpu cores:%s", strValue.c_str());
    reply->set_cpu_cores(strValue);
    LOG_INFO("get cpu cores successfully");
    return Status::OK;
}

Status SysInfoImpl::getMemTotal (ServerContext* context, const GetMemTotalReq* request, GetMemTotalRsp* reply) 
{
    LOG_INFO("get cpu total");
    string strValue;
    string strCmd = "cat /proc/meminfo |grep MemTotal";
    
    if(!UtilCommon::ShellCmd(strCmd, strValue)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return Status::CANCELLED;
    }

    LOG_INFO("cpu total:%s", strValue.c_str());
    reply->set_mem_total(strValue);
    LOG_INFO("get cpu total successfully");
    return Status::OK;
}

Status SysInfoImpl::getEthCtrlInfo (ServerContext* context, const GetEthCtrlInfoReq* request, GetEthCtrlInfoRsp* reply) 
{
    LOG_INFO("get ethernet controler info");
    string strValue;
    string strCmd = "lspci | grep Ethernet";
    
    if(!UtilCommon::ShellCmd(strCmd, strValue)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return Status::CANCELLED;
    }

    LOG_INFO("ethernet controler info:%s", strValue.c_str());
    reply->set_eth_ctrl_info(strValue);
    LOG_INFO("get ethernet controler info successfully");
    return Status::OK;
}

