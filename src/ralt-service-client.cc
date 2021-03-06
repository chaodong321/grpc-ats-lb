#include "ralt-service.grpc.pb.h"
#include "sys-info.grpc.pb.h"
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <string>
#include <jansson.h>
#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <grpc++/grpc++.h>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientWriter;
using grpc::Status;

using sysinfo::SysInfo;
//system info
using sysinfo::GetNameAndIpInfoReq;
using sysinfo::GetNameAndIpInfoRsp;
using sysinfo::GetDeviceInfoReq;
using sysinfo::GetDeviceInfoRsp;
using sysinfo::GetDeviceDetailReq;
using sysinfo::GetDeviceDetailRsp;

using sysinfo::GetCpuUsageReq;
using sysinfo::GetCpuUsageRsp;
using sysinfo::GetCpuTempReq;
using sysinfo::GetCpuTempRsp;
using sysinfo::GetMemUsageReq;
using sysinfo::GetMemUsageRsp;
using sysinfo::GetNicInfoReq;
using sysinfo::GetNicInfoRsp;
using sysinfo::GetHostNameReq;
using sysinfo::GetHostNameRsp;
using sysinfo::GetIpInfoReq;
using sysinfo::GetIpInfoRsp;
using sysinfo::GetCpuModelReq;
using sysinfo::GetCpuModelRsp;
using sysinfo::GetCpuCoresReq;
using sysinfo::GetCpuCoresRsp;
using sysinfo::GetMemTotalReq;
using sysinfo::GetMemTotalRsp;
using sysinfo::GetEthCtrlInfoReq;
using sysinfo::GetEthCtrlInfoRsp;

//ralt
using raltservice::RaltService;

//home page
using raltservice::HomePageReq;
using raltservice::HomePageRsp;
//stats
using raltservice::GetRaltStatsReq;
using raltservice::GetRaltStatsRsp;
using raltservice::GetStatsFieldReq;
using raltservice::GetStatsFieldRsp;
using raltservice::CacheLookUpReq;
using raltservice::CacheResult;
using raltservice::FlowStatLookUpReq;
using raltservice::FlowResult;
using raltservice::LogInfoLookUpReq;
using raltservice::LogResult;
//ralt log
using raltservice::GetRaltLogsReq;
using raltservice::RaltLogs;
//basic config
using raltservice::BasicConfig;
using raltservice::GetBasicConfigReq;
using raltservice::GetBasicConfigRsp;
using raltservice::SetBasicConfigReq;
using raltservice::SetBasicConfigRsp;
//domain
using raltservice::DomainType;
using raltservice::Domain;
using raltservice::GetAllDomainReq;
using raltservice::GetAllDomainRsp;
using raltservice::GetDomainReq;
using raltservice::GetDomainRsp;
using raltservice::UpdateDomainReq;
using raltservice::UpdateDomainRsp;
using raltservice::AddDomainReq;
using raltservice::AddDomainRsp;
using raltservice::DeleteDomainReq;
using raltservice::DeleteDomainRsp;
//miscellous
using raltservice::MiscKey;
using raltservice::MiscSwitch;
using raltservice::Misc;
using raltservice::GetMiscReq;
using raltservice::GetMiscRsp;
using raltservice::ModMiscOpReq;
using raltservice::ModMiscOpRsp;
//ralt rule
using raltservice::RuleType;
using raltservice::Rule;
using raltservice::GetRuleReq;
using raltservice::GetRuleRsp;
using raltservice::UpdateRuleReq;
using raltservice::UpdateRuleRsp;
//cache operate
using raltservice::GetCacheUrlReq;
using raltservice::GetCacheUrlRsp;
using raltservice::IsUrlInCacheReq;
using raltservice::IsUrlInCacheRsp;
//ralt status
using raltservice::RaltStatusReq;
using raltservice::RaltStatus;
//exec cmd
using raltservice::CommandType;
using raltservice::ExecCmdReq;
using raltservice::ExecCmdRsp;

class RaltServiceClient {
    public:
        RaltServiceClient(std::shared_ptr<Channel> channel)
            : stub_ralt(RaltService::NewStub(channel)), stub_sys(SysInfo::NewStub(channel)) {}

        void getNameAndIpInfo(){
            ClientContext context;

            // The actual RPC.
            GetNameAndIpInfoReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetNameAndIpInfoRsp reply;
            Status status = stub_sys->getNameAndIpInfo(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "host name: " << std::endl << reply.host_name() << std::endl;
                std::cout << "ip info: " << std::endl << reply.ip_info() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getDeviceInfo(){
            ClientContext context;
                
            // The actual RPC.
            GetDeviceInfoReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetDeviceInfoRsp reply;
            Status status = stub_sys->getDeviceInfo(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "cpu model: " << std::endl << reply.cpu_model() << std::endl;
                std::cout << "cpu cores: " << std::endl << reply.cpu_cores() << std::endl;
                std::cout << "mem total: " << std::endl << reply.mem_total() << std::endl;
                std::cout << "ethernet controler: " << std::endl << reply.eth_ctrl_info() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getDeviceDetail(bool bDependOsVersion){
            ClientContext context;
                
            // The actual RPC.
            GetDeviceDetailReq request;
            request.set_ip_addr("10.2.1.240");
            request.set_is_depend_os_version(bDependOsVersion);
            // Container for the data we expect from the server.
            GetDeviceDetailRsp reply;
            Status status = stub_sys->getDeviceDetail(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "cpu usage: " << std::endl << reply.cpu_usage() << std::endl;
                std::cout << "memory usage: " << std::endl << reply.mem_usage() << std::endl;
                std::cout << "cpu temperature: " << std::endl << reply.cpu_temp() << std::endl;
                std::cout << "os version: " << std::endl << reply.os_version() << std::endl;
                std::cout << "network info: " << std::endl << reply.nic_info() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }
        
        void getCpuUsage(){
            ClientContext context;
                
            // The actual RPC.
            GetCpuUsageReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetCpuUsageRsp reply;
            Status status = stub_sys->getCpuUsage(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "cpu usage: " << std::endl << reply.cpu_usage() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getCpuTemp(){
            ClientContext context;
                
            // The actual RPC.
            GetCpuTempReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetCpuTempRsp reply;
            Status status = stub_sys->getCpuTemp(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "cpu temp: " << std::endl << reply.cpu_temp() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getMemUsage(){
            ClientContext context;
                
            // The actual RPC.
            GetMemUsageReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetMemUsageRsp reply;
            Status status = stub_sys->getMemUsage(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "mem usage: " << std::endl << reply.mem_usage() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getNicInfo(bool bDependOsVersion){
            ClientContext context;
            
            // The actual RPC.
            GetNicInfoReq request;
            request.set_ip_addr("10.2.1.240");
            request.set_is_depend_os_version(bDependOsVersion);
            // Container for the data we expect from the server.
            GetNicInfoRsp reply;
            Status status = stub_sys->getNicInfo(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "nic info: " << std::endl << reply.nic_info() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getHostName(){
            ClientContext context;
                
            // The actual RPC.
            GetHostNameReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetHostNameRsp reply;
            Status status = stub_sys->getHostName(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "host name: " << std::endl << reply.host_name() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getIpInfo(){
            ClientContext context;
                
            // The actual RPC.
            GetIpInfoReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetIpInfoRsp reply;
            Status status = stub_sys->getIpInfo(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "ip info: " << std::endl << reply.ip_info() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getCpuModel(){
            ClientContext context;
                
            // The actual RPC.
            GetCpuModelReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetCpuModelRsp reply;
            Status status = stub_sys->getCpuModel(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "cpu model: " << std::endl << reply.cpu_model() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getCpuCores(){
            ClientContext context;
                
            // The actual RPC.
            GetCpuCoresReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetCpuCoresRsp reply;
            Status status = stub_sys->getCpuCores(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "cpu cores: " << std::endl << reply.cpu_cores() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getMemTotal(){
            ClientContext context;
                
            // The actual RPC.
            GetMemTotalReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetMemTotalRsp reply;
            Status status = stub_sys->getMemTotal(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "mem total: " << std::endl << reply.mem_total() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getEthCtrlInfo(){
            ClientContext context;
                
            // The actual RPC.
            GetEthCtrlInfoReq request;
            request.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            GetEthCtrlInfoRsp reply;
            Status status = stub_sys->getEthCtrlInfo(&context, request, &reply);
            // Act upon its cacheStatus.
            if (status.ok()) {
                std::cout << "mem total: " << std::endl << reply.eth_ctrl_info() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getRaltStats(){
            ClientContext context;
            GetRaltStatsReq req;
            req.set_ip_addr("10.2.1.240");
            GetRaltStatsRsp rsp;
            
            Status status = stub_ralt->getRaltStats(&context, req, &rsp);
            if (status.ok()) {
                std::cout << "start_time: " << rsp.start_time() << std::endl;
                std::cout << "end_time: " << rsp.end_time() << std::endl;
                std::cout << "logs_space_used_mb: " << rsp.logs_space_used_mb() << std::endl;
                std::cout << "logs_space_total_mb: " << rsp.logs_space_total_mb() << std::endl;
                std::cout << "flow_incoming_requests: " << rsp.flow_incoming_requests() << std::endl;
                std::cout << "flow_incoming_responses: " << rsp.flow_incoming_responses() << std::endl;
                std::cout << "flow_total_client_connections_ipv4: " << rsp.flow_total_client_connections_ipv4() << std::endl;
                std::cout << "flow_total_client_connections_ipv6: " << rsp.flow_total_client_connections_ipv6() << std::endl;
                std::cout << "flow_total_server_connections: " << rsp.flow_total_server_connections() << std::endl;
                std::cout << "cache_used_mb: " << rsp.cache_used_mb() << std::endl;
                std::cout << "cache_total_mb: " << rsp.cache_total_mb() << std::endl;
                std::cout << "cache_total_hits: " << rsp.cache_total_hits() << std::endl;
                std::cout << "cache_hit_ratio: " << rsp.cache_hit_ratio() << std::endl;
                std::cout << "hostdb_total_hits: " << rsp.hostdb_total_hits() << std::endl;
                std::cout << "hostdb_hit_ratio: " << rsp.hostdb_hit_ratio() << std::endl;
            }
            else{
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void ShowFieldValue(std::string strFiledName){
            ClientContext context;
            GetStatsFieldReq req;
            req.set_ip_addr("10.2.1.240");
            req.set_field_name(strFiledName);
            GetStatsFieldRsp rsp;
            
            Status status = stub_ralt->getStatsField(&context, req, &rsp);
            if (status.ok()) {
                std::cout << "file value: " << rsp.value() << std::endl;
            }
            else{
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }
            
        void showHomePageData(){
            ClientContext context;
            HomePageReq req;
            HomePageRsp rsp;
            
            Status status = stub_ralt->getHomePageData(&context, req, &rsp);
            if (status.ok()) {
                std::cout << "home page:" << std::endl;
                std::cout << "domain_num: " << rsp.domain_num() << std::endl;
                std::cout << "cluster_device: " << rsp.cluster_device() << std::endl;
                std::cout << "app_transfor_bytes: " << rsp.app_transfor_bytes() << std::endl;
                std::cout << "app_transfor_connections: " << rsp.app_transfor_connections() << std::endl;
                std::cout << "cache_total_hits: " << rsp.cache_total_hits() << std::endl;
                std::cout << "cache_bytes_total: " << rsp.cache_bytes_total() << std::endl;
                std::cout << "cache_bytes_used: " << rsp.cache_bytes_used() << std::endl;
                std::cout << "cache_num_docs: " << rsp.cache_num_docs() << std::endl;
            }
            else{
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void showCache() {

            ClientContext context;
                
            // The actual RPC.
            CacheLookUpReq cacheRequest;
            cacheRequest.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            CacheResult cacheReply;
            Status cacheStatus = stub_ralt->showCacheData(&context, cacheRequest, &cacheReply);
            // Act upon its cacheStatus.
            if (cacheStatus.ok()) {
                std::cout << "Cache data:" << std::endl;
                std::cout << "cache_total_hits: " << cacheReply.cache_total_hits() << std::endl;
                std::cout << "bytes_total: " << cacheReply.bytes_total() << std::endl;
                std::cout << "bytes_used: " << cacheReply.bytes_used() << std::endl;
                std::cout << "num_docs: " << cacheReply.num_docs() << std::endl;
                std::cout << "cache_hit_ratio: " << cacheReply.cache_hit_ratio() << std::endl;
            } else {
                std::cout << cacheStatus.error_code() << ": " << cacheStatus.error_message() << std::endl;
            }
        }
        
        void showFlowStats() {
            ClientContext context;
            
            // The actual RPC.
            FlowStatLookUpReq flowRequest;
            flowRequest.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            FlowResult flowReply;
            Status flowStatus = stub_ralt->showFlowStatData(&context, flowRequest, &flowReply);
            // Act upon its flowStatus.
            if (flowStatus.ok()) {
                std::cout << "Flow stats data:" << std::endl;
                std::cout << "user_agent_current_connections_count: " << flowReply.user_agent_current_connections_count() << std::endl;
                std::cout << "origin_server_current_connections_count: " << flowReply.origin_server_current_connections_count() << std::endl;
                std::cout << "completed_requests: " << flowReply.completed_requests() << std::endl;
                std::cout << "total_incoming_connections: " << flowReply.total_incoming_connections() << std::endl;
                std::cout << "incoming_requests: " << flowReply.incoming_requests() << std::endl;
                std::cout << "outgoing_requests: " << flowReply.outgoing_requests() << std::endl;
                std::cout << "incoming_responses: " << flowReply.incoming_responses() << std::endl;
                std::cout << "total_client_connections: " << flowReply.total_client_connections() << std::endl;
                std::cout << "total_client_connections_ipv4: " << flowReply.total_client_connections_ipv4() << std::endl;
                std::cout << "total_client_connections_ipv6: " << flowReply.total_client_connections_ipv6() << std::endl;
                std::cout << "total_server_connections: " << flowReply.total_server_connections() << std::endl;
                std::cout << "bandwidth_hit_ratio: " << flowReply.bandwidth_hit_ratio() << std::endl;
            } else {
                std::cout << flowStatus.error_code() << ": " << flowStatus.error_message() << std::endl;
            }
        }
        
        void showLogInfo() {
            ClientContext context;
            
            // The actual RPC.
            LogInfoLookUpReq logRequest;
            logRequest.set_ip_addr("10.2.1.240");
            // Container for the data we expect from the server.
            LogResult logReply;
            Status logStatus = stub_ralt->showLogInfoData(&context, logRequest, &logReply);
            // Act upon its logStatus.
            if (logStatus.ok()) {
                std::cout << "Cache data:" << std::endl;
                std::cout << "log_files_space_mb_used: " << logReply.log_files_space_mb_used() << std::endl;
                std::cout << "max_space_mb_for_logs: " << logReply.max_space_mb_for_logs() << std::endl;
            } else {
                std::cout << logStatus.error_code() << ": " << logStatus.error_message() << std::endl;
            }
        }

        void getRaltLogs(){
            ClientContext context;
            GetRaltLogsReq request;
            request.set_ip_addr("10.2.1.240");
            std::unique_ptr<ClientReader<RaltLogs> > reader(stub_ralt->getRaltLogs(&context, request));
            RaltLogs logs;
            while (reader->Read(&logs)) {
                std::cout << "raltlogs:" << std::endl << logs.logs() << std::endl ;
            }
            Status status = reader->Finish();
            if (status.ok()) {
                std::cout << "getRaltLogs rpc succeeded." << std::endl;
            } else {
                std::cout << "getRaltLogs rpc failed." << std::endl;
            }
        }

        void getBasicConfig(){
            ClientContext context;
            GetBasicConfigReq request;
            request.set_ip_addr("10.2.1.240");
            GetBasicConfigRsp reply;
            Status status =  stub_ralt->getBasicConfig(&context, request, &reply);
            if (status.ok()) {
                std::cout << "logging_enabled:" << reply.basic_config().logging_enabled() << std::endl;
                std::cout << "max_space_mb_for_logs:" << reply.basic_config().max_space_mb_for_logs() << std::endl;
                std::cout << "rolling_enabled:" << reply.basic_config().rolling_enabled() << std::endl;
                std::cout << "server_ports:" << reply.basic_config().server_ports() << std::endl;
                std::cout << "storage_cache_size:" << reply.basic_config().storage_cache_size() << std::endl;
                std::cout << "http_cache_enabled:" << reply.basic_config().http_cache_enabled() << std::endl;
                std::cout << "connections_throttle:" << reply.basic_config().connections_throttle() << std::endl;
                std::cout << "ip_resolve:" << reply.basic_config().ip_resolve() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void setBasicConfig(){
            ClientContext context;
            SetBasicConfigReq request;
            request.set_ip_addr("10.2.1.240");
            BasicConfig *basic_config = request.mutable_basic_config();
            basic_config->set_logging_enabled(1);
            basic_config->set_max_space_mb_for_logs(1);
            basic_config->set_rolling_enabled(1);
            basic_config->set_server_ports("80:ipv6 8080:ipv6 443:ipv6");
            basic_config->set_storage_cache_size(500);
            basic_config->set_http_cache_enabled(0);
            basic_config->set_connections_throttle(70000);
            basic_config->set_ip_resolve("ipv4;ipv6;none");
            SetBasicConfigRsp reply;
            std::cout << "start set basic config" << std::endl;
            Status status =  stub_ralt->setBasicConfig(&context, request, &reply);
            std::cout << "end set basic config" << std::endl;
            if (status.ok()) {
                printf("modify success\n");
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getAllDomain(unsigned int page_size, unsigned int page_num){
            ClientContext context;
            GetAllDomainReq request;
            request.set_ip_addr("10.2.1.240");
            GetAllDomainRsp reply;
            request.set_page_size(page_size);
            request.set_page_number(page_num);
            Status status =  stub_ralt->getAllDomain(&context, request, &reply);
            if (status.ok()) {
                std::cout << "getAllDomain rpc succeeded." << std::endl;
                std::cout << "domain_sum: " << reply.domain_total() << std::endl;
                ::google::protobuf::RepeatedPtrField<Domain> domain=reply.domain();
                for (const auto& domain_member : domain){
                    std::cout << "type:" << domain_member.type() << std::endl;
                    std::cout << "member:" << domain_member.domain_str() << std::endl;
                    std::cout << "transform:" << domain_member.append_or_replace_str() << std::endl;
                    std::cout << "port:" << domain_member.port() << std::endl;
                }
            } else {
                std::cout << "getAllDomain rpc failed." << std::endl;
            }
        }

        void updateDomain(){
            
            ClientContext context;
            UpdateDomainRsp reply;
            UpdateDomainReq request;
            request.set_ip_addr("10.2.1.240");
            Domain *domain_member = request.add_domain();
            domain_member->set_type(DomainType::enum_member_domain);
            domain_member->set_domain_str("www.reyzar.com");
            domain_member->set_append_or_replace_str("ipv6.reyzar.com");
            domain_member->set_port("80,8080");
            Domain *domain_member2 = request.add_domain();
            domain_member2->set_type(DomainType::enum_member_domain);
            domain_member2->set_domain_str("www.reyzar.com");
            domain_member2->set_append_or_replace_str("ipv6.reyzar.com");
            domain_member2->set_port("80,8080");
            Domain *sub_domain1 = request.add_domain();
            sub_domain1->set_type(DomainType::enum_subs_domain);
            sub_domain1->set_domain_str("http:\\/\\/");
            sub_domain1->set_append_or_replace_str("http://");
            Status status =  stub_ralt->updateDomain(&context, request, &reply);
            if (status.ok()) {
              std::cout << "updateDomain rpc succeeded." << std::endl;
            } else {
              std::cout << "updateDomain rpc failed." << std::endl;
            }
        }

        void getDomain(const std::string &strDomain, const std::string &strTransDomain, unsigned int page_size, unsigned int page_num){
            ClientContext context;
            GetDomainReq request;
            request.set_ip_addr("10.2.1.240");
            GetDomainRsp reply;
            request.set_domain_str(strDomain);
            request.set_transformed_domain(strTransDomain);
            request.set_page_size(page_size);
            request.set_page_number(page_num);
            Status status =  stub_ralt->getDomain(&context, request, &reply);
            if (status.ok()) {
                std::cout << "getDomain rpc succeeded." << std::endl;
                std::cout << "domain_sum: " << reply.domain_total() << std::endl;
                ::google::protobuf::RepeatedPtrField<Domain> domain=reply.domain();
                for (const auto& domain_member : domain){
                    std::cout << "type:" << domain_member.type() << std::endl;
                    std::cout << "member:" << domain_member.domain_str() << std::endl;
                    std::cout << "transform:" << domain_member.append_or_replace_str() << std::endl;
                    std::cout << "port:" << domain_member.port() << std::endl;
                }
            } else {
                std::cout << "getDomain rpc failed." << std::endl;
            }
        }

        void addDomain(){
            ClientContext context;
            AddDomainReq request;
            request.set_ip_addr("10.2.1.240");
            Domain domain;
            domain.set_type(DomainType::enum_member_domain);
            domain.set_domain_str("www.reyzar.com");
            domain.set_append_or_replace_str("ipv6.reyzar.com");
            domain.set_port("[8080]");
            request.set_allocated_domain(&domain);
            AddDomainRsp reply;
            Status status =  stub_ralt->addDomain(&context, request, &reply);
            if (status.ok()) {
                std::cout << "addDomain rpc success." << std::endl;
            } else {
                std::cout << "addDomain rpc failed." << std::endl;
            }
        }

        void delDomain(){
            ClientContext context;
            DeleteDomainReq request;
            request.set_ip_addr("10.2.1.240");
            request.set_domain_str("www.reyzar.com");
            DeleteDomainRsp reply;
            Status status =  stub_ralt->deleteDomain(&context, request, &reply);
            if (status.ok()) {
                std::cout << "delDomain rpc success." << std::endl;
            } else {
                std::cout << "delDomain rpc failed." << std::endl;
            }
        }
        
        void getMiscData(){
            ClientContext context;
            GetMiscReq request;
            request.set_ip_addr("10.2.1.240");
            GetMiscRsp reply;
            Status status =  stub_ralt->getMisc(&context, request, &reply);
            if (status.ok()) {
                std::cout << "ralt_filter_type_default: " << reply.ralt_filter_type_default() << std::endl;
                //std::cout << "ralt_referer_default: " << reply.ralt_referer_default() << std::endl;
                //std::cout << "ralt_utf16_to_utf8_default: " << reply.ralt_utf16_to_utf8_default() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void modMiscData(){
            ClientContext context;
            ModMiscOpReq request;
            request.set_ip_addr("10.2.1.240");
            Misc misc;
            misc.set_key(MiscKey::enum_ralt_filter_type_default);
            misc.set_value(MiscSwitch::switch_on);
            request.set_allocated_misc(&misc);
            ModMiscOpRsp reply;
            Status status =  stub_ralt->modMisc(&context, request, &reply);

            if (status.ok()) {
                printf("modify misc data success\n");
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getRule(){
            ClientContext context;
            GetRuleReq request;
            request.set_ip_addr("10.2.1.240");
            GetRuleRsp reply;

            Status status =  stub_ralt->getRule(&context, request, &reply);
            if (status.ok()) {
                std::cout << "getRule rpc succeeded." << std::endl;
                ::google::protobuf::RepeatedPtrField<Rule> rules=reply.rule();
                for (const auto& rule : rules){
                    std::cout << rule.type() << "\t" << rule.search() << "\t" << rule.replace() << "\t" << rule.append() << "\t" << rule.ralt_domain()  << std::endl;
                }
            } else {
                std::cout << "getRule rpc failed." << std::endl;
            }
        }

        void updateRule(){
            ClientContext context;
            UpdateRuleRsp reply;
            UpdateRuleReq request;
            request.set_ip_addr("10.2.1.240");
            Rule *rule1 = request.add_rule();
            rule1->set_type(RuleType::enum_top_level_domain);
            rule1->set_search(".com");
            rule1->set_replace(".ex1");
            rule1->set_append("$");
            rule1->set_ralt_domain("1,2,3");
            
            request.set_ip_addr("10.2.1.240");
            Rule *rule2 = request.add_rule();
            rule2->set_type(RuleType::enum_ipv4_addr);
            rule2->set_search("\"1");
            rule2->set_replace("*");
            rule2->set_append("$");
            Status status =  stub_ralt->updateRule(&context, request, &reply);
            if (status.ok()) {
              std::cout << "updateRule rpc succeeded." << std::endl;
            } else {
              std::cout << "updateRule rpc failed." << std::endl;
            }
        }

        void getCacheUrl(){
            ClientContext context;
            GetCacheUrlReq request;
            request.set_ip_addr("10.2.1.240");
            GetCacheUrlRsp reply;
            Status status =  stub_ralt->getCacheUrl(&context, request, &reply);
            if (status.ok()) {
                std::cout << "cache: " << reply.all_url() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void isUrlInCache(){
            ClientContext context;
            IsUrlInCacheReq request;
            request.set_ip_addr("10.2.1.240");
            request.set_url("http://www.reyzar.com/js/front/main.js");
            IsUrlInCacheRsp reply;
            Status status =  stub_ralt->isUrlInCache(&context, request, &reply);
            if (status.ok()) {
                std::cout << "result: " << reply.result() << std::endl;
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

        void getRaltStatus(){
            ClientContext context;
            RaltStatusReq request;
            request.set_ip_addr("10.2.1.240");
            std::unique_ptr<ClientReader<RaltStatus> > reader(stub_ralt->getRaltStatus(&context, request));
            
            RaltStatus ralt_status;
            while (reader->Read(&ralt_status)) {
                std::cout << "ipAddr:" << ralt_status.ip_addr() << std::endl;
                std::cout << "status:" << ralt_status.status() << std::endl;
            }
            Status status = reader->Finish();
            if (status.ok()) {
                std::cout << "getRaltStatus rpc succeeded." << std::endl;
            } else {
                std::cout << "getRaltStatus rpc failed." << std::endl;
            }
        }

        void execCmd(){
            ClientContext context;
            ExecCmdReq request;
            request.set_ip_addr("10.2.1.240");
            ExecCmdRsp reply;
            request.set_ip_addr("10.2.21.143");
            request.set_cmd(CommandType::ralt_stop);
            Status status =  stub_ralt->execCmd(&context, request, &reply);

            if (status.ok()) {
                printf("exec success\n");
            } else {
                std::cout << status.error_code() << ": " << status.error_message() << std::endl;
            }
        }

    private:
        std::unique_ptr<RaltService::Stub> stub_ralt;
        std::unique_ptr<SysInfo::Stub> stub_sys;
};

int main(int argc, char** argv) {
    RaltServiceClient client(grpc::CreateChannel("10.2.1.235:50052", grpc::InsecureChannelCredentials()));

    //client.getNameAndIpInfo();
    //client.getDeviceInfo();
    //client.getDeviceDetail(true);
    //client.getCpuUsage();
    //client.getCpuTemp();
    //client.getMemUsage();
    //client.getNicInfo(true);
    //client.getHostName();
    //client.getIpInfo();
    //client.getCpuModel();
    //client.getCpuCores();
    //client.getMemTotal();
    //client.getEthCtrlInfo();

    //client.getRaltStats();
    //client.ShowFieldValue("proxy.process.cache.bytes_total");
    //client.showHomePageData();
    //client.showCache();
    //client.showFlowStats();
    //client.showLogInfo();
    //client.getRaltLogs();

    //client.getBasicConfig();
    //client.setBasicConfig();
    
    //client.getAllDomain(2,2);
    //client.updateDomain();
    //client.getDomain("www.reyzar.com", "", 2, 1);
    //client.delDomain("www.reyzar.com");
    //client.addDomain();
    
    //client.getMiscData();
    //client.modMiscData();
    //client.getMiscData();

    client.getRule();
    //client.updateRule();
    //client.getCacheUrl();
    //client.isUrlInCache();

    //client.getRaltStatus();
    //client.execCmd();
    return 0;
}

