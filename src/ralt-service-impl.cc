#include "util-common.h"
#include "util-log.h"
#include "ralt-service-impl.h"
#include "ralt-domain.h"
#include "misc-conf.h"
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
#define TRAFFIC_CTL "/opt/reyzar/can/bin/traffic_ctl"
#define RECORD_CONFIG_PATH "/opt/reyzar/can/etc/trafficserver/storage.config"

Status RaltServiceImpl::getRaltStats (ServerContext* context,
	const GetRaltStatsReq* request, GetRaltStatsRsp* reply)
{
	LOG_INFO("get ralt stats");
	
	string strValue;
	string strCmd =  "curl -v http://127.0.0.1:8080/_stats";
	if(!UtilCommon::ShellCmd(strCmd, strValue)){
		LOG_ERROR("shell cmd error: %s", strCmd.c_str());
		return Status::CANCELLED;
	}
	
	json_t *j_stats = json_loads(strValue.c_str(), 0, NULL);
	if(NULL == j_stats){
		LOG_ERROR("json_loads failed");
		return Status::CANCELLED;
	}
	
	json_t *j_global = json_object_get(j_stats,"global");
	if(NULL == j_global){
		LOG_ERROR("json_object_get failed");
		return Status::CANCELLED;
	}

	json_t *j_cache_bytes_used = json_object_get(j_global,"proxy.process.cache.bytes_used");
	if(NULL != j_cache_bytes_used)
		reply->set_cache_used_bytes(atoi(json_string_value(j_cache_bytes_used)));

	json_t *j_cache_bytes_total = json_object_get(j_global,"proxy.process.cache.bytes_total");
	if(NULL != j_cache_bytes_total)
		reply->set_cache_total_bytes(atoi(json_string_value(j_cache_bytes_total)));

	json_t *j_flow_completed_requests = json_object_get(j_global,"proxy.process.http.completed_requests");
	if(NULL != j_flow_completed_requests)
		reply->set_flow_completed_requests(atoi(json_string_value(j_flow_completed_requests)));
	
	json_t *j_flow_incoming_requests = json_object_get(j_global,"proxy.process.http.incoming_requests");
	if(NULL != j_flow_incoming_requests)
		reply->set_flow_incoming_requests(atoi(json_string_value(j_flow_incoming_requests)));
	
	json_t *j_flow_total_client_connections_ipv4 = json_object_get(j_global,"proxy.process.http.total_client_connections_ipv4");
	if(NULL != j_flow_total_client_connections_ipv4)
		reply->set_flow_total_client_connections_ipv4(atoi(json_string_value(j_flow_total_client_connections_ipv4)));
		
	json_t *j_flow_total_client_connections_ipv6 = json_object_get(j_global,"proxy.process.http.total_client_connections_ipv6");
	if(NULL != j_flow_total_client_connections_ipv6)
		reply->set_flow_total_client_connections_ipv6(atoi(json_string_value(j_flow_total_client_connections_ipv6)));

	json_t *j_flow_bandwidth_hit_ratio = json_object_get(j_global,"proxy.node.bandwidth_hit_ratio");
	if(NULL != j_flow_bandwidth_hit_ratio)
		reply->set_flow_bandwidth_hit_ratio(atof(json_string_value(j_flow_bandwidth_hit_ratio)));

	json_t *j_logs_space_used_mb = json_object_get(j_global,"proxy.process.log.log_files_space_used");
	if(NULL != j_logs_space_used_mb)
		reply->set_logs_space_used_mb(atoi(json_string_value(j_logs_space_used_mb))/1024);

	//max_space_mb_for_logs
	string strLogMaxSpaceCmd = string("/opt/reyzar/can/bin/traffic_ctl config get proxy.config.log.max_space_mb_for_logs|awk -F ': ' '{print $2}'");
	string strLogMaxSpace;
	if( ! UtilCommon::ShellCmd(strLogMaxSpaceCmd, strLogMaxSpace) ){
		LOG_ERROR("shell cmd error: %s", strLogMaxSpaceCmd.c_str());
		return Status::CANCELLED;
	}
	if(!strLogMaxSpace.empty()){
		reply->set_logs_space_total_mb(atoi(strLogMaxSpace.c_str()));
	}
	else{
		LOG_WARN("get logs space total mb failed");
	}

	LOG_INFO("cache_used_bytes: %llu", reply->cache_used_bytes());
	LOG_INFO("cache_total_bytes: %llu", reply->cache_total_bytes());
	LOG_INFO("logs_space_used_mb: %lu", reply->logs_space_used_mb());
	LOG_INFO("logs_space_total_mb: %lu", reply->logs_space_total_mb());
	LOG_INFO("flow_completed_requests: %lu", reply->flow_completed_requests());
	LOG_INFO("flow_incoming_requests: %lu", reply->flow_incoming_requests());
	LOG_INFO("flow_total_client_connections_ipv4: %lu", reply->flow_total_client_connections_ipv4());
	LOG_INFO("flow_total_client_connections_ipv6: %lu", reply->flow_total_client_connections_ipv6());
	LOG_INFO("flow_bandwidth_hit_ratio: %f", reply->flow_bandwidth_hit_ratio());
	
	LOG_INFO("get ralt stats successfully");
	return Status::OK;
}

Status RaltServiceImpl::getStatsFieldValue (ServerContext* context,
	const StatsFieldName* request, StatsFieldValue* reply)
{
	LOG_INFO("get stats field value");
	
	string strName = request->field_name();
	LOG_INFO("field name: %s", strName.c_str());
	if(strName.empty()){
		LOG_ERROR("file name is empty");
		return Status::CANCELLED;
	}
	
	string strValue;
	string strCmd =  "curl -v http://127.0.0.1:8080/_stats";
	if(!UtilCommon::ShellCmd(strCmd, strValue)){
		LOG_ERROR("shell cmd error: %s", strCmd.c_str());
		return Status::CANCELLED;
	}
	
	json_t *j_stats = json_loads(strValue.c_str(), 0, NULL);
	if(NULL == j_stats){
		LOG_ERROR("json_loads failed");
		return Status::CANCELLED;
	}
	
	json_t *j_global = json_object_get(j_stats,"global");
	if(NULL == j_global){
		LOG_ERROR("json_object_get failed");
		return Status::CANCELLED;
	}

	json_t *j_value = json_object_get(j_global, strName.c_str());
	if(NULL != j_value){
		reply->set_value(json_string_value(j_value));
	}

	LOG_INFO("get stats field value by field name successfully");
	return Status::OK;
}

Status RaltServiceImpl::getHomePageData (ServerContext* context,
	const HomePageReq* request, HomePageRsp* reply)
{
	LOG_INFO("get home page data");
	string strValue;
	string strCmd =  "curl -v http://127.0.0.1:8080/_stats";
	if(!UtilCommon::ShellCmd(strCmd, strValue)){
		LOG_ERROR("shell cmd error: %s", strCmd.c_str());
		return Status::CANCELLED;
	}
	
	json_t *j_stats = json_loads(strValue.c_str(), 0, NULL);
	if(NULL == j_stats){
		LOG_ERROR("json_loads failed");
		return Status::CANCELLED;
	}
	
	json_t *j_global = json_object_get(j_stats,"global");
	if(NULL == j_global){
		LOG_ERROR("json_object_get failed");
		return Status::CANCELLED;
	}
	
	reply->set_domain_num(RaltDomain::GetInstance()->GetMemberNum());
	
	reply->set_cluster_device(3);
	
	json_t *j_app_transfor_bytes = json_object_get(j_global,"proxy.node.origin_server_total_bytes");
	if(NULL != j_app_transfor_bytes)
		reply->set_app_transfor_bytes(atoi(json_string_value(j_app_transfor_bytes)));
	
	json_t *j_app_transfor_connections = json_object_get(j_global,"proxy.process.http.incoming_responses");
	if(NULL != j_app_transfor_connections)
		reply->set_app_transfor_connections(atoi(json_string_value(j_app_transfor_connections)));
	
	json_t *j_cache_total_hits = json_object_get(j_global,"proxy.node.cache_total_hits");
	if(NULL != j_cache_total_hits)
		reply->set_cache_total_hits(atoi(json_string_value(j_cache_total_hits)));
	
	json_t *j_cache_bytes_total = json_object_get(j_global,"proxy.process.cache.bytes_total");
	if(NULL != j_cache_bytes_total)
		reply->set_cache_bytes_total(atoi(json_string_value(j_cache_bytes_total)));
	
	json_t *j_cache_bytes_used = json_object_get(j_global,"proxy.process.cache.bytes_used");
	if(NULL != j_cache_bytes_used)
		reply->set_cache_bytes_used(atoi(json_string_value(j_cache_bytes_used)));
	
	json_t *j_cache_num_docs = json_object_get(j_global,"proxy.node.cache.contents.num_docs");
	if(NULL != j_cache_num_docs)
		reply->set_cache_num_docs(atoi(json_string_value(j_cache_num_docs)));

	LOG_INFO("get home page data successfully");
	return Status::OK;
}

Status RaltServiceImpl::showCacheData(ServerContext* context, const CacheLookUpReq* request, CacheResult* reply)
{
	LOG_INFO("get cache data");
	string strValue;
	string strCmd =  "curl -v http://127.0.0.1:8080/_stats";

	if(!UtilCommon::ShellCmd(strCmd, strValue)){
		LOG_ERROR("shell cmd error: %s", strCmd.c_str());
		return Status::CANCELLED;
	}

	json_t *j_stats = json_loads(strValue.c_str(), 0, NULL);
	if(NULL == j_stats){
		LOG_ERROR("json_loads failed");
		return Status::CANCELLED;
	}
	
	json_t *j_global = json_object_get(j_stats,"global");
	if(NULL == j_global){
		LOG_ERROR("json_object_get failed");
		return Status::CANCELLED;
	}

	json_t *j_cache_total_hits = json_object_get(j_global,"proxy.node.cache_total_hits");
	if(NULL != j_cache_total_hits)
		reply->set_cache_total_hits(atoi(json_string_value(j_cache_total_hits)));

	json_t *j_bytes_total = json_object_get(j_global,"proxy.process.cache.bytes_total");
	if(NULL != j_bytes_total)
		reply->set_bytes_total(atoi(json_string_value(j_bytes_total)));

	json_t *j_bytes_used = json_object_get(j_global,"proxy.process.cache.bytes_used");
	if(NULL != j_bytes_used)
		reply->set_bytes_used(atoi(json_string_value(j_bytes_used)));

	json_t *j_num_docs = json_object_get(j_global,"proxy.node.cache.contents.num_docs");
	if(NULL != j_num_docs)
		reply->set_num_docs(atoi(json_string_value(j_num_docs)));
	
	json_t *j_cache_hit_ratio = json_object_get(j_global,"proxy.node.cache_hit_ratio");
	if(NULL != j_cache_hit_ratio)
		reply->set_cache_hit_ratio(atof(json_string_value(j_cache_hit_ratio)));

	LOG_INFO("get cache data successfully");
	return Status::OK;
}

Status RaltServiceImpl::showFlowStatData(ServerContext* context,
	const FlowStatLookUpReq* request, FlowResult* reply)
{
	LOG_INFO("get flow data");
	string strValue;
	string strCmd =  "curl -v http://127.0.0.1:8080/_stats";
	
	if(!UtilCommon::ShellCmd(strCmd, strValue)){
		LOG_ERROR("shell cmd error: %s", strCmd.c_str());
		return Status::CANCELLED;
	}
	
	json_t *j_stats = json_loads(strValue.c_str(), 0, NULL);
	if(NULL == j_stats){
		LOG_ERROR("json_loads failed");
		return Status::CANCELLED;
	}
	
	json_t *j_global = json_object_get(j_stats,"global");
	if(NULL == j_global){
		LOG_ERROR("json_object_get failed");
		return Status::CANCELLED;
	}

	json_t *j_user_agent_current_connections_count = json_object_get(j_global,"proxy.node.http.user_agent_current_connections_count");
	if(NULL != j_user_agent_current_connections_count)
		reply->set_user_agent_current_connections_count(atoi(json_string_value(j_user_agent_current_connections_count)));
	
	json_t *j_origin_server_current_connections_count = json_object_get(j_global,"proxy.node.http.origin_server_current_connections_count");
	if(NULL != j_origin_server_current_connections_count)
		reply->set_origin_server_current_connections_count(atoi(json_string_value(j_origin_server_current_connections_count)));
	
	json_t *j_completed_requests = json_object_get(j_global,"proxy.process.http.completed_requests");
	if(NULL != j_completed_requests)
		reply->set_completed_requests(atoi(json_string_value(j_completed_requests)));
	
	json_t *j_total_incoming_connections = json_object_get(j_global,"proxy.process.http.total_incoming_connections");
	if(NULL != j_total_incoming_connections)
		reply->set_total_incoming_connections(atoi(json_string_value(j_total_incoming_connections)));
	
	json_t *j_incoming_requests = json_object_get(j_global,"proxy.process.http.incoming_requests");
	if(NULL != j_incoming_requests)
		reply->set_incoming_requests(atoi(json_string_value(j_incoming_requests)));
	
	json_t *j_outgoing_requests = json_object_get(j_global,"proxy.process.http.outgoing_requests");
	if(NULL != j_outgoing_requests)
		reply->set_outgoing_requests(atoi(json_string_value(j_outgoing_requests)));
	
	json_t *j_incoming_responses = json_object_get(j_global,"proxy.process.http.incoming_responses");
	if(NULL != j_incoming_responses)
		reply->set_incoming_responses(atoi(json_string_value(j_incoming_responses)));
	
	json_t *j_total_client_connections = json_object_get(j_global,"proxy.process.http.total_client_connections");
	if(NULL != j_total_client_connections)
		reply->set_total_client_connections(atoi(json_string_value(j_total_client_connections)));
	
	json_t *j_total_client_connections_ipv4 = json_object_get(j_global,"proxy.process.http.total_client_connections_ipv4");
	if(NULL != j_total_client_connections_ipv4)
		reply->set_total_client_connections_ipv4(atoi(json_string_value(j_total_client_connections_ipv4)));
		
	json_t *j_total_client_connections_ipv6 = json_object_get(j_global,"proxy.process.http.total_client_connections_ipv6");
	if(NULL != j_total_client_connections_ipv6)
		reply->set_total_client_connections_ipv6(atoi(json_string_value(j_total_client_connections_ipv6)));
	
	json_t *j_total_server_connections = json_object_get(j_global,"proxy.process.http.total_server_connections");
	if(NULL != j_total_server_connections)
		reply->set_total_server_connections(atoi(json_string_value(j_total_server_connections)));
	
	json_t *j_user_agent_total_bytes = json_object_get(j_global,"proxy.node.user_agent_total_bytes");
	if(NULL != j_user_agent_total_bytes)
		reply->set_user_agent_total_bytes(atoi(json_string_value(j_user_agent_total_bytes)));
	
	json_t *j_origin_server_total_bytes = json_object_get(j_global,"proxy.node.origin_server_total_bytes");
	if(NULL != j_origin_server_total_bytes)
		reply->set_origin_server_total_bytes(atoi(json_string_value(j_origin_server_total_bytes)));

	json_t *j_bandwidth_hit_ratio = json_object_get(j_global,"proxy.node.bandwidth_hit_ratio");
	if(NULL != j_bandwidth_hit_ratio)
		reply->set_bandwidth_hit_ratio(atof(json_string_value(j_bandwidth_hit_ratio)));

	LOG_INFO("get flow data successfully");
	return Status::OK;
}

Status RaltServiceImpl::showLogInfoData(ServerContext* context,
	const LogInfoLookUpReq* request, LogResult* reply)
{
	LOG_INFO("get log info");
	string strValue;
	string strCmd =  "curl -v http://127.0.0.1:8080/_stats";
	
	if(!UtilCommon::ShellCmd(strCmd, strValue)){
		LOG_ERROR("shell cmd error: %s", strCmd.c_str());
		return Status::CANCELLED;
	}
	
	json_t *j_stats = json_loads(strValue.c_str(), 0, NULL);
	if(NULL == j_stats){
		LOG_ERROR("json_loads failed");
		return Status::CANCELLED;
	}
	
	json_t *j_global = json_object_get(j_stats,"global");
	if(NULL == j_global){
		LOG_ERROR("json_object_get failed");
		return Status::CANCELLED;
	}

	json_t *j_log_files_space_used = json_object_get(j_global,"proxy.process.log.log_files_space_used");
	if(NULL != j_log_files_space_used)
		reply->set_log_files_space_mb_used(atoi(json_string_value(j_log_files_space_used))/1024);

	//max_space_mb_for_logs
	string strLogMaxSpaceCmd = string("/opt/reyzar/can/bin/traffic_ctl config get proxy.config.log.max_space_mb_for_logs|awk -F ': ' '{print $2}'");
	string strLogMaxSpace;
	if(UtilCommon::ShellCmd(strLogMaxSpaceCmd, strLogMaxSpace)){
		if(!strLogMaxSpace.empty()){
			reply->set_max_space_mb_for_logs(atoi(strLogMaxSpace.c_str()));
		}
	}

	LOG_INFO("get log info successfully");
	return Status::OK;
}

Status RaltServiceImpl::getRaltLogs(ServerContext* context,
	const GetRaltLogsReq* request, ServerWriter<RaltLogs>* reply)
{
	LOG_INFO("get ralt logs");
	string strFileListCmd, strFileList;
	//strFileListCmd = "ls -ltr /opt/reyzar/can/var/log/trafficserver/ | grep '.old' | grep -v 'error' | grep -v 'squid'";
	strFileListCmd = string("ls -ltr ") + string(ATS_LOG_DIR) 
		+ string(" | grep -v total |awk '{print $9}'");
	if(!UtilCommon::ShellCmd(strFileListCmd, strFileList)){
		LOG_ERROR("shell cmd error: %s", strFileListCmd.c_str());
		return Status::CANCELLED;
	}

	vector<string> vLogFiles, vDefaultFile, vAccessFile;
	while(!strFileList.empty())
	{
		string strLine;
		UtilCommon::GetLine(strFileList,strLine);
		UtilCommon::Trim(strLine);

		if(strLine.empty())
		{
			continue;
		}

		string strAbsolutePath = string(ATS_LOG_DIR) + strLine;

		struct stat statbuf;
		if( stat(strAbsolutePath.c_str(), &statbuf) < 0 
			|| S_ISDIR(statbuf.st_mode) || statbuf.st_size <= 0){
			continue;
		}

		if( strLine.rfind(".old") != string::npos && strLine.rfind(".old") == (strLine.length()-4) 
				&& strLine.find("error") == string::npos && strLine.find("squid") == string::npos ) {

			vDefaultFile.push_back(strAbsolutePath);
			LOG_DEBUG("default log file: %s", strAbsolutePath.c_str());
		}
		else if (strLine.find("access_") == 0){
			vAccessFile.push_back(strAbsolutePath);
			LOG_DEBUG("access log file: %s", strAbsolutePath.c_str());
		}
	}

	
	if(!vDefaultFile.empty()){
		vLogFiles.assign(vDefaultFile.begin(), vDefaultFile.end());
	}
	else if(!vAccessFile.empty()){
		vLogFiles.assign(vAccessFile.begin(), vAccessFile.end());
	}

	if(vLogFiles.empty()){
		LOG_INFO("no log files to read");
		return Status::OK;
	}

	time_t now;
	struct tm *timenow;
	time(&now);
	timenow = localtime(&now);
	char format_time[32];
	snprintf(format_time, sizeof(format_time)-1, "%d-%d-%d",
		timenow->tm_year+1900, timenow-> tm_mon+1,  timenow-> tm_mday);
	string strDirName = string(ATS_LOG_DIR) + string("bak/") + string(format_time);
	string strCreateDirCmd = string("mkdir -p ") + strDirName;
	system(strCreateDirCmd.c_str());

	for(vector<string>::iterator it = vLogFiles.begin(); it != vLogFiles.end(); it++){
		ifstream in(it->c_str());
	    istreambuf_iterator<char> begin(in);
	    istreambuf_iterator<char> end;
	    string strFileContent(begin, end);

		LOG_DEBUG("file content: %s", strFileContent.c_str());

		RaltLogs logs;
		logs.set_logs(strFileContent);
		reply->Write(logs);

		string strMvCmd = string("mv ") + *it + " " + strDirName;
		system(strMvCmd.c_str());
	}

	LOG_INFO("get ralt logs successfully");
	return Status::OK;

/*
	const char *proc_run_dir = get_current_dir_name();
	
	const char *dir = "/opt/reyzar/can/var/log/trafficserver/";
	DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
	if ( (dp = opendir(dir)) == NULL ) {  
        fprintf(stderr, "Can`t open directory %s\n", dir);  
		return Status::OK;
    }
	
	if(chdir(dir) != 0){
		return Status::OK;
	}

	time_t now;
	struct tm *timenow;
	time(&now);
	timenow = localtime(&now);
	char format_time[32];
	snprintf(format_time, "%d-%d-%d", timenow->tm_year+1900, timenow-> tm_mon+1,  timenow-> tm_mday);
	
	string strDirName = string("bak/") + string(format_time);
	bool is_create_log_dir = false;
	while ( (entry = readdir(dp)) != NULL ) {
		if( stat(entry->d_name, &statbuf) < 0 || S_ISDIR(statbuf.st_mode) || statbuf.st_size <= 0){
			continue;
		}

		string strFileName(entry->d_name);
		if( strFileName.rfind(".old") == (strFileName.length()-sizeof(".old")) 
				&& strFileName.find("error") != 0 && strFileName.find("squid") != 0 ) {

			
		}
		else if (strFileName.find("access_") == 0){
			
		}
		else{
			continue;
		}

		if( !is_create_log_dir ){
			is_create_log_dir = true;
			if(mkdir("bak/", 0644) != 0){
				chdir(proc_run_dir);
				return Status::OK;
			}
		}

		ifstream in(entry->d_name);
	    istreambuf_iterator<char> begin(in);
	    istreambuf_iterator<char> end;
	    string strFileContent(begin, end);
	    in >> strFileContent;
		
		RaltLogs logs;
		logs.set_logs(strFileContent);
		reply->Write(logs);
		
		if( rename(entry->d_name, "bak/") != 0 ){
			break;
		}
	}
	chdir(proc_run_dir);
	return Status::OK;
	*/
}

Status RaltServiceImpl::getBasicConfig(ServerContext* context,
	const GetBasicConfigReq* request, GetBasicConfigRsp* reply)
{

	LOG_INFO("get basic config");
	//logging_enabled
	string strLoggingEnabledCmd = string(TRAFFIC_CTL)
		+ string(" config get proxy.config.log.logging_enabled|awk -F ': ' '{printf $2}'");
	string strLoggingEnabled;
	if(UtilCommon::ShellCmd(strLoggingEnabledCmd, strLoggingEnabled)){
		if(!strLoggingEnabled.empty()){
			LOG_INFO("logging enabled: %s", strLoggingEnabled.c_str());
			reply->set_logging_enabled(atoi(strLoggingEnabled.c_str()));
		}
		else{
			LOG_ERROR("get logging enabled error: %s", strLoggingEnabledCmd.c_str());
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strLoggingEnabledCmd.c_str());
		return Status::CANCELLED;
	}
	
	//max_space_mb_for_logs
	string strLogMaxSpaceCmd = string(TRAFFIC_CTL)
		+ string(" config get proxy.config.log.max_space_mb_for_logs|awk -F ': ' '{printf $2}'");
	string strLogMaxSpace;
	if(UtilCommon::ShellCmd(strLogMaxSpaceCmd, strLogMaxSpace)){
		if(!strLogMaxSpace.empty()){
			LOG_INFO("log max space: %s", strLogMaxSpace.c_str());
			reply->set_max_space_mb_for_logs(atoi(strLogMaxSpace.c_str()));
		}
		else{
			LOG_ERROR("get log max space error: %s", strLogMaxSpace.c_str());
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strLogMaxSpaceCmd.c_str());
		return Status::CANCELLED;
	}
	
	//rolling_enabled
	string strRollingEnabledCmd = string(TRAFFIC_CTL) 
		+ string(" config get proxy.config.log.rolling_enabled|awk -F ': ' '{printf $2}'");
	string strRollingEnabled;
	if(UtilCommon::ShellCmd(strRollingEnabledCmd, strRollingEnabled)){
		if(!strRollingEnabled.empty()){
			LOG_INFO("roll enabled: %s", strRollingEnabled.c_str());
			reply->set_rolling_enabled(atoi(strRollingEnabled.c_str()));
		}
		else{
			LOG_ERROR("get roll enabled error: %s", strRollingEnabled.c_str());
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strRollingEnabledCmd.c_str());
		return Status::CANCELLED;
	}

	//server_ports
	string strServerPortsCmd = string(TRAFFIC_CTL)
		+ string(" config get proxy.config.http.server_ports|awk -F ': ' '{printf $2}'");
	string strServerPorts;
	if(UtilCommon::ShellCmd(strServerPortsCmd, strServerPorts)){
		if(!strServerPorts.empty()){
			LOG_INFO("server ports: %s", strServerPorts.c_str());
			reply->set_server_ports(strServerPorts.c_str());
		}
		else{
			LOG_ERROR("get server ports error: %s", strServerPorts.c_str());
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strServerPortsCmd.c_str());
		return Status::CANCELLED;
	}

	//storage cache size
	string strStorageCacheCmd = string("cat ") + string(RECORD_CONFIG_PATH)
		+ string(" | grep -v '#' | awk '{printf $2}'");
	string strStorageCache;
	if(UtilCommon::ShellCmd(strStorageCacheCmd, strStorageCache)){
		if(!strStorageCache.empty()){
			LOG_INFO("storage cache size: %s", strStorageCache.c_str());
			unsigned int nStorageSize;
			sscanf(strStorageCache.c_str(), "%d[1-9]", &nStorageSize);
			reply->set_storage_cache_size(nStorageSize);
		}
		else{
			LOG_ERROR("get storage cache size error: %s", strStorageCache.c_str());
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strStorageCacheCmd.c_str());
		return Status::CANCELLED;
	}

	LOG_INFO("get basic config successfully");
	return Status::OK;
}

Status RaltServiceImpl::setBasicConfig(ServerContext* context,
	const SetBasicConfigReq* request, SetBasicConfigRsp* reply)
{
	LOG_INFO("set basic config");
	LOG_INFO("logging_enabled: %u", request->logging_enabled());
	LOG_INFO("max_space_mb_for_logs: %u", request->max_space_mb_for_logs());
	LOG_INFO("rolling_enabled: %u", request->rolling_enabled());
	LOG_INFO("server_ports: %s", request->server_ports().c_str());
	LOG_INFO("storage_cache_size: %dM", request->storage_cache_size());

	string strRet;
	string strLoggingEnabledCmd = string(TRAFFIC_CTL)
		+ string(" config set proxy.config.log.logging_enabled ")
		+ to_string(request->logging_enabled());
	if(UtilCommon::ShellCmd(strLoggingEnabledCmd, strRet)){
		if(strRet.find("failed") == string::npos){
			LOG_INFO("set logging enabled successfully");
		}
		else{
			LOG_ERROR("%s", strRet.c_str());
			reply->set_result(1);
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strLoggingEnabledCmd.c_str());
		return Status::CANCELLED;
	}

	string strMaxSpaceMbForLogsCmd = string(TRAFFIC_CTL)
		+ string(" config set proxy.config.log.max_space_mb_for_logs ")
		+ to_string(request->max_space_mb_for_logs());
	
	if(UtilCommon::ShellCmd(strMaxSpaceMbForLogsCmd, strRet)){
		if(strRet.find("failed") == string::npos){
			LOG_INFO("set max space mb for logs successfully");
		}
		else{
			LOG_ERROR("%s", strRet.c_str());
			reply->set_result(1);
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strMaxSpaceMbForLogsCmd.c_str());
		reply->set_result(1);
		return Status::CANCELLED;
	}

	string strRollingEnabledCmd = string(TRAFFIC_CTL)
		+ string(" config set proxy.config.log.rolling_enabled ")
		+ to_string(request->rolling_enabled());
	
	if(UtilCommon::ShellCmd(strRollingEnabledCmd, strRet)){
		if(strRet.find("failed") == string::npos){
			LOG_INFO("set log rolling enabled successfully");
		}
		else{
			LOG_ERROR("%s", strRet.c_str());
			reply->set_result(1);
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strRollingEnabledCmd.c_str());
		reply->set_result(1);
		return Status::CANCELLED;
	}

	string strServerPortCmd = string(TRAFFIC_CTL) + string(" config set proxy.config.http.server_ports '") + request->server_ports() + string("'");
	if(UtilCommon::ShellCmd(strServerPortCmd, strRet)){
		if(strRet.find("failed") == string::npos){
			LOG_INFO("set server port successfully");
		}
		else{
			LOG_ERROR("%s", strRet.c_str());
			reply->set_result(1);
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strServerPortCmd.c_str());
		reply->set_result(1);
		return Status::CANCELLED;
	}

	string strDelStorageCmd = string("sed -i '/#/!d' ") + (RECORD_CONFIG_PATH);
	if(UtilCommon::ShellCmd(strDelStorageCmd, strRet)){
		if(!strRet.empty()){
			LOG_ERROR("%s", strRet.c_str());
			reply->set_result(1);
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strDelStorageCmd.c_str());
		reply->set_result(1);
		return Status::CANCELLED;
	}
	string strAppendStorageCmd = string("sed -i '$a var/trafficserver ") +
	     to_string(request->storage_cache_size())+string("M' ") + (RECORD_CONFIG_PATH);
	if(UtilCommon::ShellCmd(strAppendStorageCmd, strRet)){
		if(!strRet.empty()){
			LOG_ERROR("%s", strRet.c_str());
			reply->set_result(1);
			return Status::CANCELLED;
		}
	}
	else{
		LOG_ERROR("shell cmd error: %s", strAppendStorageCmd.c_str());
		reply->set_result(1);
		return Status::CANCELLED;
	}

	reply->set_result(0);
	LOG_INFO("set basic config successfully");
	return Status::OK;
}

Status RaltServiceImpl::getAllDomain(ServerContext* context, const GetAllDomainReq* request,
	GetAllDomainRsp* reply)
{
	LOG_INFO("get all domain");
	unsigned int nPageDomainSum = request->page_size();
	unsigned int nPageNum = request->page_number();
	LOG_INFO("page domain sum: %u", nPageDomainSum);
	LOG_INFO("page num: %u", nPageNum);
	map<string, DomainValue> domainMap = RaltDomain::GetInstance()->GetAllDomain();
	
	unsigned int nStart = (nPageNum-1>=0?(nPageNum-1):1) * nPageDomainSum + 1;
	unsigned int nEnd = nPageNum * nPageDomainSum;
	unsigned int nDomainNum = 0;

	LOG_INFO("nStart: %u", nStart);
	LOG_INFO("nEnd: %u", nEnd);
	for(const auto it : domainMap)
	{
		nDomainNum++;
		LOG_INFO("nDomainNum: %u", nDomainNum);
		LOG_INFO("str_domain: %s", it.second.str_domain.c_str());
		if(nDomainNum >= nStart && nDomainNum <= nEnd){
			LOG_INFO("add domain to reply: %s", it.second.str_domain.c_str());
			Domain *domain_member = reply->add_domain();
			domain_member->set_type((raltservice::DomainType)it.second.type);
			domain_member->set_domain_str(it.second.str_domain);
			domain_member->set_append_or_replace_str(it.second.str_append_or_replace);
			domain_member->set_port(it.second.str_port);
		}
	}
	reply->set_domain_total(domainMap.size());
	LOG_INFO("domain total: %u", domainMap.size());
	
	LOG_INFO("get all domain successfully");
	return Status::OK;
}

Status RaltServiceImpl::updateDomain(ServerContext* context, ServerReader<Domain>* request,
	UpdateDomainRsp* reply)
{
	LOG_INFO("update all domain");
	map<string, DomainValue> domainMap;
	Domain element;
	while (request->Read(&element)) {
		string strMmber = element.domain_str();
		DomainValue value;
		value.type = element.type();
		value.str_domain = strMmber;
		value.str_append_or_replace = element.append_or_replace_str();
		value.str_port = element.port();

		domainMap[strMmber] = value;
    }
	RaltDomain::GetInstance()->UpdateDomain(&domainMap);
	reply->set_result(0);
	LOG_INFO("update all domain successfully");
	return Status::OK;
}

Status RaltServiceImpl::getDomain(ServerContext* context, const GetDomainReq* request,
	 GetDomainRsp* reply)
{
	LOG_INFO("get domain info");
	unsigned int nPageDomainSum = request->page_size();
	unsigned int nPageNum = request->page_number();
	LOG_INFO("page domain sum: %u", nPageDomainSum);
	LOG_INFO("page num: %u", nPageNum);
	string strDomain = request->domain_str();
	string strTransDomain = request->transformed_domain();
	LOG_INFO("search domain: %s", strDomain.c_str());
	LOG_INFO("search transformed domain: %s", strTransDomain.c_str());
	map<string, DomainValue> domainMap;
	RaltDomain::GetInstance()->GetDomain(strDomain, strTransDomain, domainMap);

	unsigned int nStart = (nPageNum-1>=0?(nPageNum-1):1) * nPageDomainSum + 1;
	unsigned int nEnd = nPageNum * nPageDomainSum;
	unsigned int nDomainNum = 0;

	LOG_INFO("nStart: %u", nStart);
	LOG_INFO("nEnd: %u", nEnd);
	for(const auto it : domainMap)
	{
		nDomainNum++;
		LOG_INFO("nDomainNum: %u", nDomainNum);
		LOG_INFO("str_domain: %s", it.second.str_domain.c_str());
		if(nDomainNum >= nStart && nDomainNum <= nEnd){
			LOG_INFO("add domain to reply: %s", it.second.str_domain.c_str());
			Domain *domain_member = reply->add_domain();
			domain_member->set_type((raltservice::DomainType)it.second.type);
			domain_member->set_domain_str(it.second.str_domain);
			domain_member->set_append_or_replace_str(it.second.str_append_or_replace);
			domain_member->set_port(it.second.str_port);
		}
	}
	reply->set_domain_total(domainMap.size());
	LOG_INFO("domain total: %u", domainMap.size());
	LOG_INFO("get domain info successfully");
	return Status::OK;
}

Status RaltServiceImpl::addDomain(ServerContext* context, const Domain* request,
	AddDomainRsp* reply)
{
	LOG_INFO("add domain");
	DomainValue domain;
	domain.type = request->type();
	domain.str_domain = request->domain_str();
	domain.str_append_or_replace = request->append_or_replace_str();
	domain.str_port = request->port();
	RaltDomain::GetInstance()->AddDomain(&domain);
	LOG_INFO("add domain successfully");
	return Status::OK;
}

Status RaltServiceImpl::deleteDomain(ServerContext* context, const DeleteDomainReq* request,
	DeleteDomainRsp* reply)
{
	LOG_INFO("delete domain");
	string strDomain = request->domain_str();
	RaltDomain::GetInstance()->DelDomain(strDomain);
	LOG_INFO("delete domain successfully");
	return Status::OK;
}

Status RaltServiceImpl::getMisc(ServerContext* context, const GetMiscReq* request,
	GetMiscRsp* reply)
{
	LOG_INFO("get misc");
	MiscData data;
	MiscConf::GetInstance()->GetMiscData(&data);
	if(data.ralt_filter_type_default){
		reply->set_ralt_filter_type_default(MiscSwitch::switch_on);
	}
	else{
		reply->set_ralt_filter_type_default(MiscSwitch::switch_off);
	}
	LOG_INFO("get misc successfully");
	return Status::OK;
}

Status RaltServiceImpl::modMisc(ServerContext* context, const Misc* request, ModMiscOpRsp* reply)
{
	LOG_INFO("modify misc");
	MiscConf::GetInstance()->ModMisc((MISC_TYPE)request->key(), (MISC_VALUE)request->value());
	reply->set_result(0);
	LOG_INFO("modify misc successfully");
	return Status::OK;
}

Status RaltServiceImpl::getRaltStatus(ServerContext* context, const RaltStatusReq* request,
	ServerWriter<RaltStatus>* reply)
{
	LOG_INFO("get ralt status");
	RaltStatus status;
	string strCmd = "ps -ef |grep traffic_server |grep -v grep";
	string strRet;
	if(UtilCommon::ShellCmd(strCmd, strRet)){
		if(!strRet.empty()){
			status.set_status(0);
			reply->Write(status);
			LOG_INFO("get ralt status successfully");
			return Status::OK;
		}
	}
	status.set_status(1);
	reply->Write(status);
	LOG_INFO("get ralt status failed");
	return Status::OK;
}

Status RaltServiceImpl::execCmd(ServerContext* context, const ExecCmdReq* request,
	ExecCmdRsp* reply)
{
	LOG_INFO("execute cmd");
	int cmdType = request->cmd();
	switch(cmdType){
		case CommandType::ralt_start:
			system("service can start");
			LOG_INFO("cmd: service can start");
			reply->set_result(0);
			break;
		case CommandType::ralt_stop:
			system("service can stop");
			LOG_INFO("cmd: service can stop");
			reply->set_result(0);
			break;
		case CommandType::ralt_restart:
			system("service can restart");
			LOG_INFO("cmd: service can restart");
			reply->set_result(0);
			break;
		case CommandType::cache_clear:
			system("service can stop");
			LOG_INFO("cmd: service can stop");
			system("/opt/reyzar/can/bin/traffic_server -Cclear");
			LOG_INFO("cmd: /opt/reyzar/can/bin/traffic_server -Cclear");
			system("service can start");
			LOG_INFO("cmd: service can start");
			reply->set_result(0);
			break;
		case CommandType::reload_config:
			system("/opt/reyzar/can/bin/traffic_ctl config reload");
			LOG_INFO("cmd: /opt/reyzar/can/bin/traffic_ctl config reload");
			reply->set_result(0);
			break;
		default:
			reply->set_result(1);
			LOG_INFO("no such operation");
			return Status::CANCELLED;
	}

	LOG_INFO("execute cmd successfully");
	return Status::OK;
}


