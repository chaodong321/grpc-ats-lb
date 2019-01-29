#include "util-common.h"
#include "util-log.h"
#include "ralt-service-impl.h"
#include "ralt-domain.h"
#include "misc-conf.h"
#include "ralt-rule.h"
#include "ralt-cache.h"
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
#define STORAGE_CONFIG_PATH "/opt/reyzar/can/etc/trafficserver/storage.config"

bool RaltServiceImpl::TrafficControlConfigGet(string strConfig, string &strConfigValue)
{
    bool bRet = false;
    string strCmd = string(TRAFFIC_CTL) + string(" config get ")
        + strConfig + string("|awk -F ': ' '{printf $2}'");
    string strCmdValue;
    LOG_INFO("cmd: %s", strCmd.c_str());
    if(UtilCommon::ShellCmd(strCmd, strCmdValue)){
        if(!strCmdValue.empty() && strCmdValue.find("failed") == string::npos){
            LOG_INFO("cmd value: %s", strCmdValue.c_str());
            strConfigValue = strCmdValue;
            bRet = true;
        }
        else{
            LOG_ERROR("error value: %s", strCmdValue.c_str());
        }
    }
    else{
        LOG_ERROR("shell cmd error: %s", strCmdValue.c_str());
    }
    return bRet;
}

bool RaltServiceImpl::TrafficControlConfigSet(string strConfig, string strConfigValue)
{
    bool bRet = false;
    string strCmdValue;
    string strCmd = string(TRAFFIC_CTL) + string(" config set ")
        + strConfig + string(" \"") + strConfigValue + string("\"");

    LOG_INFO("cmd: %s", strCmd.c_str());
    if(UtilCommon::ShellCmd(strCmd, strCmdValue)){
        if(strCmdValue.find("failed") == string::npos){
            LOG_INFO("cmd execute success");
            bRet = true;
        }
        else{
            LOG_ERROR("error value: %s", strCmdValue.c_str());
            bRet = false;
        }
    }
    else{
        LOG_ERROR("shell cmd error: %s", strCmdValue.c_str());
        bRet = false;
    }
    return bRet;
}


bool RaltServiceImpl::TrafficControlMetricGet(string strConfig, string &strMetricValue)
{
    bool bRet = false;
    string strCmd = string(TRAFFIC_CTL) + string(" metric get ")
        + strConfig + string("|awk '{printf $2}'");
    string strCmdValue;
    LOG_INFO("cmd: %s", strCmd.c_str());
    if(UtilCommon::ShellCmd(strCmd, strCmdValue)){
        if(!strCmdValue.empty() && strCmdValue.find("failed") == string::npos){
            LOG_INFO("cmd value: %s", strCmdValue.c_str());
            strMetricValue = strCmdValue;
            bRet = true;
        }
        else{
            LOG_ERROR("error value: %s", strCmdValue.c_str());
        }
    }
    else{
        LOG_ERROR("shell cmd error: %s", strCmdValue.c_str());
    }
    return bRet;
}


Status RaltServiceImpl::getRaltStats (ServerContext* context,
    const GetRaltStatsReq* request, GetRaltStatsRsp* reply)
{
    LOG_INFO("get ralt stats");

    string strStartTime;
    if(TrafficControlMetricGet("proxy.node.restarts.proxy.start_time", strStartTime)){
        LOG_INFO("logging enabled: %s", strStartTime.c_str());
        reply->set_start_time(atoi(strStartTime.c_str()));
    }

    time_t current_seconds = time(NULL);
    reply->set_end_time(current_seconds);


    string strIncomingRequests;
    if(TrafficControlMetricGet("proxy.process.http.incoming_requests", strIncomingRequests)){
        reply->set_flow_incoming_requests(atoi(strIncomingRequests.c_str()));
    }

    string strIncomingResponses;
    if(TrafficControlMetricGet("proxy.process.http.incoming_responses", strIncomingResponses)){
        reply->set_flow_incoming_responses(atoi(strIncomingResponses.c_str()));
    }

    string strTotalClientConnectionsIpv4;
    if(TrafficControlMetricGet("proxy.process.http.total_client_connections_ipv4", strTotalClientConnectionsIpv4)){
        reply->set_flow_total_client_connections_ipv4(atoi(strTotalClientConnectionsIpv4.c_str()));
    }

    string strTotalClientConnectionsIpv6;
    if(TrafficControlMetricGet("proxy.process.http.total_client_connections_ipv6", strTotalClientConnectionsIpv6)){
        reply->set_flow_total_client_connections_ipv6(atoi(strTotalClientConnectionsIpv6.c_str()));
    }

    string strTotalClientServerConnections;
    if(TrafficControlMetricGet("proxy.process.http.total_server_connections", strTotalClientServerConnections)){
        reply->set_flow_total_server_connections(atoi(strTotalClientServerConnections.c_str()));
    }

    string strCacheBytesUsed;
    if(TrafficControlMetricGet("proxy.process.cache.bytes_used", strCacheBytesUsed)){
        reply->set_cache_used_mb(atoi(strCacheBytesUsed.c_str())/(1024*1024));
    }

    string strCacheBytesTotal;
    if(TrafficControlMetricGet("proxy.process.cache.bytes_total", strCacheBytesTotal)){
        reply->set_cache_total_mb(atoi(strCacheBytesTotal.c_str())/(1024*1024));
    }

    string strCacheTotalHits;
    if(TrafficControlMetricGet("proxy.process.cache.cache_total_hits", strCacheTotalHits)){
        reply->set_cache_total_hits(atoi(strCacheTotalHits.c_str()));
    }

    string strCacheHitRatio;
    if(TrafficControlMetricGet("proxy.node.cache_hit_ratio", strCacheHitRatio)){
        reply->set_cache_total_hits(atof(strCacheHitRatio.c_str()));
    }

    string strHostDbTotalHits;
    if(TrafficControlMetricGet("proxy.node.hostdb.total_hits", strHostDbTotalHits)){
        reply->set_hostdb_total_hits(atoi(strHostDbTotalHits.c_str()));
    }

    string strHostDbHitRatio;
    if(TrafficControlMetricGet("proxy.node.hostdb.hit_ratio", strHostDbHitRatio)){
        reply->set_hostdb_hit_ratio(atof(strHostDbHitRatio.c_str()));
    }

    string strLogFilesSpaceUsed;
    if(TrafficControlMetricGet("proxy.process.log.log_files_space_used", strLogFilesSpaceUsed)){
        reply->set_logs_space_used_mb(atoi(strLogFilesSpaceUsed.c_str())/(1024*1024));
    }

    string strLogFilesSpaceMax;
    if(TrafficControlConfigGet("proxy.config.log.max_space_mb_for_logs", strLogFilesSpaceMax)){
        reply->set_logs_space_total_mb(atoi(strLogFilesSpaceMax.c_str()));
    }

    LOG_INFO("start time: %u", reply->start_time());
    LOG_INFO("end time: %lu", reply->end_time());
    
    LOG_INFO("logs_space_used_mb: %lu", reply->logs_space_used_mb());
    LOG_INFO("logs_space_total_mb: %lu", reply->logs_space_total_mb());
    
    LOG_INFO("flow_incoming_requests: %lu", reply->flow_incoming_requests());
    LOG_INFO("flow_incoming_responses: %lu", reply->flow_incoming_responses());
    LOG_INFO("flow_total_client_connections_ipv4: %lu", reply->flow_total_client_connections_ipv4());
    LOG_INFO("flow_total_client_connections_ipv6: %lu", reply->flow_total_client_connections_ipv6());
    LOG_INFO("flow_total_server_connections: %lu", reply->flow_total_server_connections());

    LOG_INFO("cache_used_mb: %llu", reply->cache_used_mb());
    LOG_INFO("cache_total_mb: %llu", reply->cache_total_mb());
    LOG_INFO("cache_total_hits: %lu", reply->cache_total_hits());
    LOG_INFO("cache_hit_ratio: %f", reply->cache_hit_ratio());

    LOG_INFO("hostdb_total_hits: %lu", reply->hostdb_total_hits());
    LOG_INFO("cache_hit_ratio: %f", reply->cache_hit_ratio());

    LOG_INFO("get ralt stats successfully");
    return Status::OK;
}

Status RaltServiceImpl::getStatsField (ServerContext* context,
    const GetStatsFieldReq* request, GetStatsFieldRsp* reply)
{
    LOG_INFO("get stats field value");
    
    string strName = request->field_name();
    LOG_INFO("field name: %s", strName.c_str());
    string strValue;
    if(TrafficControlMetricGet(strName, strValue)){
        reply->set_value(strValue);
    }

    LOG_INFO("get stats field value by field name successfully");
    return Status::OK;
}

Status RaltServiceImpl::getHomePageData (ServerContext* context,
    const HomePageReq* request, HomePageRsp* reply)
{
    LOG_INFO("get home page data");
    
    reply->set_domain_num(RaltDomain::GetInstance()->GetMemberNum());

    string strAppTransforBytes;
    if(TrafficControlMetricGet("proxy.node.origin_server_total_bytes", strAppTransforBytes)){
        reply->set_app_transfor_bytes(atoi(strAppTransforBytes.c_str()));
    }
    
    string strAppTransforConnections;
    if(TrafficControlMetricGet("proxy.process.http.total_server_connections", strAppTransforConnections)){
        reply->set_app_transfor_connections(atoi(strAppTransforConnections.c_str()));
    }

    string strCacheTotalHits;
    if(TrafficControlMetricGet("proxy.node.cache_total_hits", strCacheTotalHits)){
        reply->set_cache_total_hits(atoi(strCacheTotalHits.c_str()));
    }

    string strCacheBytesTotal;
    if(TrafficControlMetricGet("proxy.process.cache.bytes_total", strCacheBytesTotal)){
        reply->set_cache_bytes_total(atoi(strCacheBytesTotal.c_str()));
    }

    string strCacheBytesUsed;
    if(TrafficControlMetricGet("proxy.process.cache.bytes_used", strCacheBytesUsed)){
        reply->set_cache_bytes_used(atoi(strCacheBytesUsed.c_str()));
    }

    string strCacheNumDocs;
    if(TrafficControlMetricGet("proxy.node.cache.contents.num_docs", strCacheNumDocs)){
        reply->set_cache_num_docs(atoi(strCacheNumDocs.c_str()));
    }

    LOG_INFO("get home page data successfully");
    return Status::OK;
}

Status RaltServiceImpl::showCacheData(ServerContext* context, const CacheLookUpReq* request, CacheResult* reply)
{
    LOG_INFO("get cache data");

    string strCacheTotalHits;
    if(TrafficControlMetricGet("proxy.node.cache_total_hits", strCacheTotalHits)){
        reply->set_cache_total_hits(atoi(strCacheTotalHits.c_str()));
    }

    string strCacheBytesTotal;
    if(TrafficControlMetricGet("proxy.process.cache.bytes_total", strCacheBytesTotal)){
        reply->set_bytes_total(atoi(strCacheBytesTotal.c_str()));
    }

    string strCacheBytesUsed;
    if(TrafficControlMetricGet("proxy.process.cache.bytes_used", strCacheBytesUsed)){
        reply->set_bytes_used(atoi(strCacheBytesUsed.c_str()));
    }

    string strCacheNumDocs;
    if(TrafficControlMetricGet("proxy.node.cache.contents.num_docs", strCacheNumDocs)){
        reply->set_num_docs(atoi(strCacheNumDocs.c_str()));
    }

    string strCacheHitRatio;
    if(TrafficControlMetricGet("proxy.node.cache_hit_ratio", strCacheHitRatio)){
        reply->set_cache_hit_ratio(atof(strCacheHitRatio.c_str()));
    }

    LOG_INFO("get cache data successfully");
    return Status::OK;
}

Status RaltServiceImpl::showFlowStatData(ServerContext* context,
    const FlowStatLookUpReq* request, FlowResult* reply)
{
    LOG_INFO("get flow data");

    string strUserAgentCurrentConnectionsCount;
    if(TrafficControlMetricGet("proxy.node.http.user_agent_current_connections_count", strUserAgentCurrentConnectionsCount)){
        reply->set_user_agent_current_connections_count(atoi(strUserAgentCurrentConnectionsCount.c_str()));
    }

    string strServerAgentCurrentConnectionsCount;
    if(TrafficControlMetricGet("proxy.node.http.origin_server_current_connections_count", strServerAgentCurrentConnectionsCount)){
        reply->set_origin_server_current_connections_count(atoi(strServerAgentCurrentConnectionsCount.c_str()));
    }

    string strCompletedRequest;
    if(TrafficControlMetricGet("proxy.process.http.completed_requests", strCompletedRequest)){
        reply->set_completed_requests(atoi(strCompletedRequest.c_str()));
    }

    string strTotalIncomintConnections;
    if(TrafficControlMetricGet("proxy.process.http.total_incoming_connections", strTotalIncomintConnections)){
        reply->set_total_incoming_connections(atoi(strTotalIncomintConnections.c_str()));
    }

    string strIncomingRequests;
    if(TrafficControlMetricGet("proxy.process.http.incoming_requests", strIncomingRequests)){
        reply->set_incoming_requests(atoi(strIncomingRequests.c_str()));
    }

    string strIncomingResponses;
    if(TrafficControlMetricGet("proxy.process.http.incoming_responses", strIncomingResponses)){
        reply->set_incoming_responses(atoi(strIncomingResponses.c_str()));
    }

    string strTotalClientConnections;
    if(TrafficControlMetricGet("proxy.process.http.total_client_connections", strTotalClientConnections)){
        reply->set_total_client_connections(atoi(strTotalClientConnections.c_str()));
    }

    string strTotalClientConnectionsIpv4;
    if(TrafficControlMetricGet("proxy.process.http.total_client_connections_ipv4", strTotalClientConnectionsIpv4)){
        reply->set_total_client_connections_ipv4(atoi(strTotalClientConnectionsIpv4.c_str()));
    }

    string strTotalClientConnectionsIpv6;
    if(TrafficControlMetricGet("proxy.process.http.total_client_connections_ipv6", strTotalClientConnectionsIpv6)){
        reply->set_total_client_connections_ipv6(atoi(strTotalClientConnectionsIpv6.c_str()));
    }

    string strTotalServerConnections;
    if(TrafficControlMetricGet("proxy.process.http.total_server_connections", strTotalServerConnections)){
        reply->set_total_server_connections(atoi(strTotalServerConnections.c_str()));
    }

    string strUserAgentTotalBytes;
    if(TrafficControlMetricGet("proxy.node.user_agent_total_bytes", strUserAgentTotalBytes)){
        reply->set_user_agent_total_bytes(atoi(strUserAgentTotalBytes.c_str()));
    }

    string strBandWidthHitRatio;
    if(TrafficControlMetricGet("proxy.node.bandwidth_hit_ratio", strBandWidthHitRatio)){
        reply->set_bandwidth_hit_ratio(atof(strBandWidthHitRatio.c_str()));
    }

    LOG_INFO("get flow data successfully");
    return Status::OK;
}

Status RaltServiceImpl::showLogInfoData(ServerContext* context,
    const LogInfoLookUpReq* request, LogResult* reply)
{
    LOG_INFO("get log info");

    string strLogFilesSpaceUsed;
    if(TrafficControlMetricGet("proxy.process.log.log_files_space_used", strLogFilesSpaceUsed)){
        reply->set_log_files_space_mb_used(atoi(strLogFilesSpaceUsed.c_str())/(1024*1024));
    }

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
}

Status RaltServiceImpl::getBasicConfig(ServerContext* context,
    const GetBasicConfigReq* request, GetBasicConfigRsp* reply)
{

    LOG_INFO("get basic config");
    BasicConfig *basic_config = reply->mutable_basic_config();
    //logging_enabled
    string strLoggingEnabled;
    if(TrafficControlConfigGet("proxy.config.log.logging_enabled", strLoggingEnabled)){
        LOG_INFO("logging enabled: %s", strLoggingEnabled.c_str());
        basic_config->set_logging_enabled(atoi(strLoggingEnabled.c_str()));
    }
    else{
        return Status::CANCELLED;
    }

    string strLogMaxSpace;
    if(TrafficControlConfigGet("proxy.config.log.max_space_mb_for_logs", strLogMaxSpace)){
        LOG_INFO("log max space: %s", strLogMaxSpace.c_str());
        basic_config->set_max_space_mb_for_logs(atoi(strLogMaxSpace.c_str()));
    }
    else{
        return Status::CANCELLED;
    }

    string strRollingEnabled;
    if(TrafficControlConfigGet("proxy.config.log.rolling_enabled", strRollingEnabled)){
        LOG_INFO("roll enabled: %s", strRollingEnabled.c_str());
        basic_config->set_rolling_enabled(atoi(strRollingEnabled.c_str()));
    }
    else{
        return Status::CANCELLED;
    }

    string strServerPorts;
    if(TrafficControlConfigGet("proxy.config.http.server_ports", strServerPorts)){
        LOG_INFO("server ports: %s", strServerPorts.c_str());
        basic_config->set_server_ports(strServerPorts.c_str());
    }
    else{
        return Status::CANCELLED;
    }

    //storage cache size
    string strStorageCacheCmd = string("cat ") + string(STORAGE_CONFIG_PATH)
        + string(" | grep -v '#' | awk '{printf $2}'");
    string strStorageCache;
    if(UtilCommon::ShellCmd(strStorageCacheCmd, strStorageCache)){
        if(!strStorageCache.empty()){
            LOG_INFO("storage cache size: %s", strStorageCache.c_str());
            unsigned int nStorageSize;
            sscanf(strStorageCache.c_str(), "%d[1-9]", &nStorageSize);
            basic_config->set_storage_cache_size(nStorageSize);
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


    string strHttpCacheEnabled;
    if(TrafficControlConfigGet("proxy.config.http.cache.http", strHttpCacheEnabled)){
        LOG_INFO("http cache enabled: %s", strHttpCacheEnabled.c_str());
        basic_config->set_http_cache_enabled(atoi(strHttpCacheEnabled.c_str()));
    }
    else{
        return Status::CANCELLED;
    }

    string strConnectionsThrottle;
    if(TrafficControlConfigGet("proxy.config.net.connections_throttle", strConnectionsThrottle)){
        LOG_INFO("connections throttle: %s", strConnectionsThrottle.c_str());
        basic_config->set_connections_throttle(atoi(strConnectionsThrottle.c_str()));
    }
    else{
        return Status::CANCELLED;
    }

    string strIpResolve;
    if(TrafficControlConfigGet("proxy.config.hostdb.ip_resolve", strIpResolve)){
        LOG_INFO("ip resolve: %s", strIpResolve.c_str());
        basic_config->set_ip_resolve(strIpResolve);
    }
    else{
        return Status::CANCELLED;
    }

    LOG_INFO("get basic config successfully");
    return Status::OK;
}

Status RaltServiceImpl::setBasicConfig(ServerContext* context,
    const SetBasicConfigReq* request, SetBasicConfigRsp* reply)
{
    const BasicConfig &basic_config = request->basic_config();
    LOG_INFO("set basic config");
    LOG_INFO("logging_enabled: %u", basic_config.logging_enabled());
    LOG_INFO("max_space_mb_for_logs: %u", basic_config.max_space_mb_for_logs());
    LOG_INFO("rolling_enabled: %u", basic_config.rolling_enabled());
    LOG_INFO("server_ports: %s", basic_config.server_ports().c_str());
    LOG_INFO("storage_cache_size: %dM", basic_config.storage_cache_size());

    string strLoggingEnabled = to_string(basic_config.logging_enabled());
    if(TrafficControlConfigSet("proxy.config.log.logging_enabled", strLoggingEnabled)){
        LOG_INFO("set logging enabled successfully");
    }
    else{
        reply->set_result(1);
        return Status::CANCELLED;
    }

    string strMaxSpaceMbForLogs = to_string(basic_config.max_space_mb_for_logs());
    if(TrafficControlConfigSet("proxy.config.log.max_space_mb_for_logs", strMaxSpaceMbForLogs)){
        LOG_INFO("set max space mb for logs successfully");
    }
    else{
        reply->set_result(1);
        return Status::CANCELLED;
    }

    string strRollingEnabled = to_string(basic_config.rolling_enabled());
    if(TrafficControlConfigSet("proxy.config.log.rolling_enabled", strRollingEnabled)){
        LOG_INFO("set log rolling enabled successfully");
    }
    else{
        reply->set_result(1);
        return Status::CANCELLED;
    }


    string strServerPorts = basic_config.server_ports();
    if(TrafficControlConfigSet("proxy.config.http.server_ports", strServerPorts)){
        LOG_INFO("set server port successfully");
    }
    else{
        reply->set_result(1);
        return Status::CANCELLED;
    }

    string strHttpCacheEnabled = to_string(basic_config.http_cache_enabled());
    if(TrafficControlConfigSet("proxy.config.http.cache.http", strHttpCacheEnabled)){
        LOG_INFO("set http cache enabled successfully");
    }
    else{
        reply->set_result(1);
        return Status::CANCELLED;
    }

    string strConnectionsThrottle = to_string(basic_config.connections_throttle());
    if(TrafficControlConfigSet("proxy.config.net.connections_throttle", strConnectionsThrottle)){
        LOG_INFO("set connections throttle successfully");
    }
    else{
        reply->set_result(1);
        return Status::CANCELLED;
    }

    string strIpResolve = basic_config.ip_resolve();
    if(TrafficControlConfigSet("proxy.config.hostdb.ip_resolve", strIpResolve)){
        LOG_INFO("set ip resolve successfully");
    }
    else{
        reply->set_result(1);
        return Status::CANCELLED;
    }

    string strRet;
    string strDelStorageCmd = string("sed -i '/#/!d' ") + string(STORAGE_CONFIG_PATH);
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
         to_string(basic_config.storage_cache_size())+string("M' ") + (STORAGE_CONFIG_PATH);
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

Status RaltServiceImpl::updateDomain(ServerContext* context, const UpdateDomainReq* request,
    UpdateDomainRsp* reply)
{
    LOG_INFO("update all domain");
    map<string, DomainValue> domainMap;
    ::google::protobuf::RepeatedPtrField<Domain> domain=request->domain();
     for (const auto& domain_member : domain){
        DomainValue value;
        value.type = domain_member.type();
        value.str_domain = domain_member.domain_str();
        value.str_append_or_replace = domain_member.append_or_replace_str();
        value.str_port = domain_member.port();
        domainMap[value.str_domain] = value;
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

Status RaltServiceImpl::addDomain(ServerContext* context, const AddDomainReq* request,
    AddDomainRsp* reply)
{
    LOG_INFO("add domain");
    DomainValue domain;
    domain.type = request->domain().type();
    domain.str_domain = request->domain().domain_str();
    domain.str_append_or_replace = request->domain().append_or_replace_str();
    domain.str_port = request->domain().port();
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

Status RaltServiceImpl::modMisc(ServerContext* context, const ModMiscOpReq* request,
    ModMiscOpRsp* reply)
{
    LOG_INFO("modify misc");
    MiscConf::GetInstance()->ModMisc((MISC_TYPE)request->misc().key(), (MISC_VALUE)request->misc().value());
    reply->set_result(0);
    LOG_INFO("modify misc successfully");
    return Status::OK;
}

Status RaltServiceImpl::getRule(ServerContext* context, const GetRuleReq* request,
    GetRuleRsp* reply)
{
    LOG_INFO("get rule");
    multimap<RULE_TYPE, RuleValue> ruleMap = RaltRule::GetInstance()->GetRule();

    for(const auto it : ruleMap)
    {
        string strRelateDomain;
        for(const auto relate_domain_it : it.second.vRelateDomain){
            if(strRelateDomain.empty()){
                strRelateDomain = to_string(relate_domain_it);
            }
            else{
                strRelateDomain = strRelateDomain + "," + to_string(relate_domain_it);
            }
        }

        Rule *rule = reply->add_rule();
        rule->set_type((raltservice::RuleType)it.second.type);
        rule->set_search(it.second.strSearch);
        rule->set_replace(it.second.strReplace);
        rule->set_append(it.second.strAppend);
        rule->set_ralt_domain(strRelateDomain);

        LOG_INFO("%u\t%s\t%s\t%s\t%s", 
            it.second.type, it.second.strSearch.c_str(), it.second.strReplace.c_str(), 
            it.second.strAppend.c_str(), strRelateDomain.c_str());
    }
    LOG_INFO("get rule successfully");
    return Status::OK;
}

Status RaltServiceImpl::updateRule(ServerContext* context, const UpdateRuleReq* request,
   UpdateRuleRsp* reply)
{
    LOG_INFO("update rule");
    multimap<RULE_TYPE, RuleValue> ruleMap;
    ::google::protobuf::RepeatedPtrField<Rule> rules=request->rule();
     for (const auto& rule : rules){
        RuleValue value;
        value.type = (RULE_TYPE)rule.type();
        value.strSearch = rule.search();
        value.strReplace = rule.replace();
        value.strAppend = rule.append();
        string strRelateDomain = rule.ralt_domain();
        vector<char> vSepration;
        vSepration.push_back(',');
        vSepration.push_back(' ');
        value.vRelateDomain = RaltRule::GetInstance()->GetMinElement(vSepration, strRelateDomain);
        pair<RULE_TYPE, RuleValue> rule_pair(value.type, value);
        ruleMap.insert(rule_pair);
    }
    RaltRule::GetInstance()->UpdateRule(ruleMap);
    reply->set_result(0);
    LOG_INFO("update all domain successfully");
    return Status::OK;
}

Status RaltServiceImpl::getCacheUrl(ServerContext* context, const GetCacheUrlReq* request,
   GetCacheUrlRsp* reply)
{
    LOG_INFO("get cache url");
    string strRegex = "*";
    string strAllUrl = RaltCache::GetInstance()->GetRegexCacheUrl(strRegex);
    reply->set_all_url(strAllUrl.c_str());
    LOG_INFO("%s", strAllUrl.c_str());
    LOG_INFO("get cache url successfully");
    return Status::OK;
}

Status RaltServiceImpl::isUrlInCache(ServerContext* context, const IsUrlInCacheReq* request,
    IsUrlInCacheRsp* reply)
{
    LOG_INFO("is cache url");
    if(RaltCache::GetInstance()->IsUrlInCache(request->url())){
        reply->set_result(0);
        LOG_INFO("url in cache");
    }
    else{
        reply->set_result(1);
        LOG_INFO("url not in cache");
    }
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


