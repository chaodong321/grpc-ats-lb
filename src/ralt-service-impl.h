#ifndef __RALT_SERVICE_IMPL_H_
#define __RALT_SERVICE_IMPL_H_

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
using grpc::Status;

using raltservice::RaltService;

//system info
using raltservice::GetCpuUsageReq;
using raltservice::GetCpuUsageRsp;
using raltservice::GetCpuTempReq;
using raltservice::GetCpuTempRsp;
using raltservice::GetMemUsageReq;
using raltservice::GetMemUsageRsp;
using raltservice::GetNicInfoReq;
using raltservice::GetNicInfoRsp;
//home page
using raltservice::HomePageReq;
using raltservice::HomePageRsp;
//stats
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
//record
using raltservice::RecordCfgType;
using raltservice::GetRecordCfgReq;
using raltservice::GetRecordCfgRsp;
using raltservice::SetRecordCfgReq;
using raltservice::SetRecordCfgRsp;
//domain
using raltservice::DomainType;
using raltservice::Domain;
using raltservice::GetAllDomainReq;
using raltservice::GetDomainReq;
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

class RaltServiceImpl final : public RaltService::Service {
public:
	//system info
	Status getCpuUsage (ServerContext* context, const GetCpuUsageReq* request,
                  GetCpuUsageRsp* reply) override;
	
	Status getCpuTemp (ServerContext* context, const GetCpuTempReq* request,
                  GetCpuTempRsp* reply) override;
	
	Status getMemUsage (ServerContext* context, const GetMemUsageReq* request,
                  GetMemUsageRsp* reply) override;
	
	Status getNicInfo (ServerContext* context, const GetNicInfoReq* request,
                  GetNicInfoRsp* reply) override;
	
	//stats
	Status getStatsFieldValue (ServerContext* context, const StatsFieldName* request,
                  StatsFieldValue* reply) override;
	
	Status getHomePageData (ServerContext* context, const HomePageReq* request,
                  HomePageRsp* reply) override;

	Status showCacheData(ServerContext* context, const CacheLookUpReq* request,
                  CacheResult* reply) override;

	Status showFlowStatData(ServerContext* context, const FlowStatLookUpReq* request,
                  FlowResult* reply) override;

	Status showLogInfoData(ServerContext* context, const LogInfoLookUpReq* request,
                  LogResult* reply) override;
	
	Status getRaltLogs(ServerContext* context, const GetRaltLogsReq* request,
                  ServerWriter<RaltLogs>* reply) override;

	//configure
	Status getRecordConfig(ServerContext* context, const GetRecordCfgReq* request,
                  GetRecordCfgRsp* reply) override;

	Status setRecordConfig(ServerContext* context, const SetRecordCfgReq* request,
                  SetRecordCfgRsp* reply) override;

	Status getAllDomain(ServerContext* context, const GetAllDomainReq* request,
                  ServerWriter<Domain>* reply) override;

	Status updateDomain(ServerContext* context, ServerReader<Domain>* request,
                     UpdateDomainRsp* reply) override;

	Status getDomain(ServerContext* context, const GetDomainReq* request,
                  Domain* reply) override;

	Status addDomain(ServerContext* context, const Domain* request,
                  AddDomainRsp* reply) override;

	Status deleteDomain(ServerContext* context, const DeleteDomainReq* request,
                  DeleteDomainRsp* reply) override;

	Status getMisc(ServerContext* context, const GetMiscReq* request,
                  GetMiscRsp* reply) override;

	Status modMisc(ServerContext* context, const Misc* request,
                  ModMiscOpRsp* reply) override;

	//command
	Status getRaltStatus(ServerContext* context, const RaltStatusReq* request,
                  ServerWriter<RaltStatus>* reply) override;

	Status execCmd(ServerContext* context, const ExecCmdReq* request,
                  ExecCmdRsp* reply) override;

};

#endif
