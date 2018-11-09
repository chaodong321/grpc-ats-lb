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

Status SysInfoImpl::getCpuUsage (ServerContext* context, const GetCpuUsageReq* request, GetCpuUsageRsp* reply)
{
	LOG_INFO("get cpu usage");
	string strValue;
	string strCmd = "mpstat -P ALL |grep -v CPU | awk 'NF > 4'|awk '{print $3,$12}'";
	
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
		reply->set_os_version("centos6");
		strNicInfoCmd = "ifconfig -a |awk NF |grep -v collisions |grep -v Memory";
	}
	else if(bDependOsVersion && strSysVer.find("7") != string::npos){
		reply->set_os_version("centos7");
		strNicInfoCmd = "ifconfig";
	}
	else if( !bDependOsVersion && strSysVer.find("6") != string::npos){
		reply->set_os_version("centos6");
		strNicInfoCmd = "ifconfig -a |awk NF |grep -v collisions |grep -v Memory";
	}
	else if( !bDependOsVersion && strSysVer.find("7") != string::npos){
		reply->set_os_version("centos7");
		strNicInfoCmd = "ifconfig -a |awk NF |grep -v collisions |grep -v Memory";
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

