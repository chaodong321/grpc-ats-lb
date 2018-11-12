#include "util-common.h"
#include "util-log.h"
#include "ralt-service-impl.h"
#include "ralt-domain.h"
#include "misc-conf.h"
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

Status RaltServiceImpl::getStatsFieldValue (ServerContext* context, const StatsFieldName* request, StatsFieldValue* reply)
{
	string strName = request->field_name();
	LOG_INFO("get stats field value by field name: %s", strName.c_str());
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

Status RaltServiceImpl::getHomePageData (ServerContext* context, const HomePageReq* request, HomePageRsp* reply)
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

Status RaltServiceImpl::showFlowStatData(ServerContext* context, const FlowStatLookUpReq* request, FlowResult* reply)
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

Status RaltServiceImpl::showLogInfoData(ServerContext* context, const LogInfoLookUpReq* request, LogResult* reply)
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

Status RaltServiceImpl::getRaltLogs(ServerContext* context, const GetRaltLogsReq* request, ServerWriter<RaltLogs>* reply)
{
	LOG_INFO("get ralt logs");
	string strFileListCmd, strFileList;
	//strFileListCmd = "ls -ltr /opt/reyzar/can/var/log/trafficserver/ | grep '.old' | grep -v 'error' | grep -v 'squid'";
	strFileListCmd = string("ls -ltr ") + string(ATS_LOG_DIR) + string(" | grep -v total |awk '{print $9}'");
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
		if( stat(strAbsolutePath.c_str(), &statbuf) < 0 || S_ISDIR(statbuf.st_mode) || statbuf.st_size <= 0){
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
	snprintf(format_time, sizeof(format_time)-1, "%d-%d-%d", timenow->tm_year+1900, timenow-> tm_mon+1,  timenow-> tm_mday);
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

Status RaltServiceImpl::getRecordConfig(ServerContext* context, const GetRecordCfgReq* request, GetRecordCfgRsp* reply)
{
	LOG_INFO("get record.config");
	//logging_enabled
	string strLoggingEnabledCmd = string("/opt/reyzar/can/bin/traffic_ctl config get proxy.config.log.logging_enabled|awk -F ': ' '{print $2}'");
	string strLoggingEnabled;
	if(UtilCommon::ShellCmd(strLoggingEnabledCmd, strLoggingEnabled)){
		if(!strLoggingEnabled.empty()){
			reply->set_logging_enabled(atoi(strLoggingEnabled.c_str()));
		}
	}
	
	//max_space_mb_for_logs
	string strLogMaxSpaceCmd = string("/opt/reyzar/can/bin/traffic_ctl config get proxy.config.log.max_space_mb_for_logs|awk -F ': ' '{print $2}'");
	string strLogMaxSpace;
	if(UtilCommon::ShellCmd(strLogMaxSpaceCmd, strLogMaxSpace)){
		if(!strLogMaxSpace.empty()){
			reply->set_max_space_mb_for_logs(atoi(strLogMaxSpace.c_str()));
		}
	}
	
	//rolling_enabled
	string strRollongEnabledCmd = string("/opt/reyzar/can/bin/traffic_ctl config get proxy.config.log.rolling_enabled|awk -F ': ' '{print $2}'");
	string strRollongEnabled;
	if(UtilCommon::ShellCmd(strRollongEnabledCmd, strRollongEnabled)){
		if(!strRollongEnabled.empty()){
			reply->set_rolling_enabled(atoi(strRollongEnabled.c_str()));
		}
	}

	//server_ports
	string strServerPortsCmd = string("/opt/reyzar/can/bin/traffic_ctl config get proxy.config.http.server_ports|awk -F ': ' '{print $2}'");
	string strServerPorts;
	if(UtilCommon::ShellCmd(strServerPortsCmd, strServerPorts)){
		if(!strServerPorts.empty()){
			reply->set_server_ports(strServerPorts.c_str());
		}
	}

	LOG_INFO("get record.config successfully");
	return Status::OK;
}

Status RaltServiceImpl::setRecordConfig(ServerContext* context, const SetRecordCfgReq* request, SetRecordCfgRsp* reply)
{
	LOG_INFO("set record.config");
	int key = request->key();
	string value = request->value();
	
	string strKey;
	switch(key){
		case RecordCfgType::enum_logging_enabled:
			strKey.assign("proxy.config.log.logging_enabled");
			LOG_INFO("key: proxy.config.log.logging_enabled");
			break;
		case RecordCfgType::enum_max_space_mb_for_logs:
			strKey.assign("proxy.config.log.max_space_mb_for_logs");
			LOG_INFO("key: proxy.config.log.max_space_mb_for_logs");
			break;
		case RecordCfgType::enum_rolling_enabled:
			strKey.assign("proxy.config.log.rolling_enabled");
			LOG_INFO("key: proxy.config.log.rolling_enabled");
			break;
		case RecordCfgType::enum_server_ports:
			strKey.assign("proxy.config.http.server_ports");
			LOG_INFO("key: proxy.config.http.server_ports");
			break;
		default:
			LOG_INFO("key: no such key");
			reply->set_result(1);
			return Status::CANCELLED;
	}

	LOG_INFO("value: %s", value.c_str());
	
	string strCmd = string("/opt/reyzar/can/bin/traffic_ctl config set ") + strKey + string(" ") + value;
	system(strCmd.c_str());
	reply->set_result(0);

	LOG_INFO("set record.config successfully");
	return Status::OK;
}

Status RaltServiceImpl::getAllDomain(ServerContext* context, const GetAllDomainReq* request, ServerWriter<Domain>* reply)
{
	LOG_INFO("get all domain");
	map<string, DomainValue>* domainMap = RaltDomain::GetInstance()->GetAllDomain();
	for(const auto it : *domainMap)
	{
		Domain domain_member;
		domain_member.set_domain_str(it.second.str_domain);
		domain_member.set_append_or_replace_str(it.second.str_append_or_replace);
		reply->Write(domain_member);
	}
	LOG_INFO("get all domain successfully");
	return Status::OK;
}

Status RaltServiceImpl::updateDomain(ServerContext* context, ServerReader<Domain>* request, UpdateDomainRsp* reply)
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

Status RaltServiceImpl::getDomain(ServerContext* context, const GetDomainReq* request, Domain* reply)
{
	LOG_INFO("get domain info");
	string strDomain = request->domain_str();
	LOG_INFO("domain name: %s", strDomain.c_str());
	DomainValue domain_value = RaltDomain::GetInstance()->GetDomain(strDomain);
	reply->set_type((raltservice::DomainType)domain_value.type);
	reply->set_domain_str(domain_value.str_domain);
	reply->set_append_or_replace_str(domain_value.str_append_or_replace);
	reply->set_port(domain_value.str_port);
	LOG_INFO("get domain info successfully");
	return Status::OK;
}

Status RaltServiceImpl::addDomain(ServerContext* context, const Domain* request, AddDomainRsp* reply)
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

Status RaltServiceImpl::deleteDomain(ServerContext* context, const DeleteDomainReq* request, DeleteDomainRsp* reply)
{
	LOG_INFO("delete domain");
	string strDomain = request->domain_str();
	RaltDomain::GetInstance()->DelDomain(strDomain);
	LOG_INFO("delete domain successfully");
	return Status::OK;
}

Status RaltServiceImpl::getMisc(ServerContext* context, const GetMiscReq* request, GetMiscRsp* reply)
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

Status RaltServiceImpl::getRaltStatus(ServerContext* context, const RaltStatusReq* request, ServerWriter<RaltStatus>* reply)
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

Status RaltServiceImpl::execCmd(ServerContext* context, const ExecCmdReq* request, ExecCmdRsp* reply)
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


