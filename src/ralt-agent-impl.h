#ifndef __RALT_AGENT_IMPL_H_
#define __RALT_AGENT_IMPL_H_

#include "ralt-service.grpc.pb.h"
#include <string>
#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include <grpc++/grpc++.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerWriter;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientWriter;
using grpc::Channel;
using grpc::Status;

using raltservice::RaltService;

//home page
using raltservice::HomePageReq;
using raltservice::HomePageRsp;
//stats
using raltservice::GetRaltStatsReq;
using raltservice::GetRaltStatsRsp;
using raltservice::StatsFieldName;
using raltservice::StatsFieldValue;
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
using raltservice::UpdateDomainRsp;
using raltservice::AddDomainRsp;
using raltservice::DeleteDomainReq;
using raltservice::DeleteDomainRsp;
//miscellous
using raltservice::MiscKey;
using raltservice::MiscSwitch;
using raltservice::GetMiscReq;
using raltservice::GetMiscRsp;
using raltservice::Misc;
using raltservice::ModMiscOpRsp;
//ralt status
using raltservice::RaltStatusReq;
using raltservice::RaltStatus;
//exec cmd
using raltservice::CommandType;
using raltservice::ExecCmdReq;
using raltservice::ExecCmdRsp;

using namespace std;

class RaltAgentImpl final : public RaltService::Service {
public:	
	RaltAgentImpl(){}

	bool getStubByIp(string strIp);

	Status getRaltStats (ServerContext* server_context, const GetRaltStatsReq* request,
                  GetRaltStatsRsp* reply) override;

	Status getStatsFieldValue (ServerContext* server_context, const StatsFieldName* request,
                  StatsFieldValue* reply) override;
	
	Status getHomePageData (ServerContext* server_context, const HomePageReq* request,
                  HomePageRsp* reply) override;

	Status showCacheData(ServerContext* server_context, const CacheLookUpReq* request,
                  CacheResult* reply) override;

	Status showFlowStatData(ServerContext* server_context, const FlowStatLookUpReq* request,
                  FlowResult* reply) override;

	Status showLogInfoData(ServerContext* server_context, const LogInfoLookUpReq* request,
                  LogResult* reply) override;
	
	Status getRaltLogs(ServerContext* server_context, const GetRaltLogsReq* request,
                  ServerWriter<RaltLogs>* reply) override;

	Status getBasicConfig(ServerContext* server_context, const GetBasicConfigReq* request,
                  GetBasicConfigRsp* reply) override;

	Status setBasicConfig(ServerContext* server_context, const SetBasicConfigReq* request,
                  SetBasicConfigRsp* reply) override;

	Status getAllDomain(ServerContext* server_context, const GetAllDomainReq* request,
                  GetAllDomainRsp* reply) override;

	Status updateDomain(ServerContext* server_context, ServerReader<Domain>* request,
                  UpdateDomainRsp* reply) override;

	Status getDomain(ServerContext* server_context, const GetDomainReq* request,
                  GetDomainRsp* reply) override;

	Status addDomain(ServerContext* server_context, const Domain* request,
                  AddDomainRsp* reply) override;

	Status deleteDomain(ServerContext* server_context, const DeleteDomainReq* request,
                  DeleteDomainRsp* reply) override;

	Status getMisc(ServerContext* server_context, const GetMiscReq* request,
                  GetMiscRsp* reply) override;

	Status modMisc(ServerContext* server_context, const Misc* request,
                  ModMiscOpRsp* reply) override;

	Status getRaltStatus(ServerContext* server_context, const RaltStatusReq* request,
                  ServerWriter<RaltStatus>* reply) override;

	Status execCmd(ServerContext* server_context, const ExecCmdReq* request,
                  ExecCmdRsp* reply) override;

private:
	std::unique_ptr<RaltService::Stub> stub_ralt;
	//ClientContext client_context;
};

#endif
