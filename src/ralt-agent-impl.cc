#include "ralt-agent-impl.h"
#include "util-common.h"
#include "ralt-domain.h"
#include "misc-conf.h"
#include <stdlib.h>
#include <unistd.h>
#include <jansson.h>
#include <iostream>
#include <memory>
#include <map>

Status RaltAgentImpl::getCpuUsage (ServerContext* context, const GetCpuUsageReq* request, GetCpuUsageRsp* reply)
{
	return Status::OK;
}
	
Status RaltAgentImpl::getCpuTemp (ServerContext* context, const GetCpuTempReq* request, GetCpuTempRsp* reply)
{
	return Status::OK;
}
	
Status RaltAgentImpl::getMemUsage (ServerContext* context, const GetMemUsageReq* request, GetMemUsageRsp* reply)
{
	return Status::OK;
}
	
Status RaltAgentImpl::getNicInfo (ServerContext* context, const GetNicInfoReq* request, GetNicInfoRsp* reply)
{
	return Status::OK;
}

Status RaltAgentImpl::getStatsFieldValue (ServerContext* context, const StatsFieldName* request, StatsFieldValue* reply)
{
	

	return Status::OK;
}

Status RaltAgentImpl::getHomePageData (ServerContext* context, const HomePageReq* request, HomePageRsp* reply)
{
	
	
	return Status::OK;
}

Status RaltAgentImpl::showCacheData(ServerContext* context, const CacheLookUpReq* request, CacheResult* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::showFlowStatData(ServerContext* context, const FlowStatLookUpReq* request, FlowResult* reply)
{
	
	
	return Status::OK;
}

Status RaltAgentImpl::showLogInfoData(ServerContext* context, const LogInfoLookUpReq* request, LogResult* reply)
{
	
	
	return Status::OK;
}

Status RaltAgentImpl::getRecordConfig(ServerContext* context, const GetRecordCfgReq* request, GetRecordCfgRsp* reply)
{
	
	
	return Status::OK;
}

Status RaltAgentImpl::setRecordConfig(ServerContext* context, const SetRecordCfgReq* request, SetRecordCfgRsp* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::getAllDomain(ServerContext* context, const GetAllDomainReq* request, ServerWriter<Domain>* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::updateDomain(ServerContext* context, ServerReader<Domain>* request, UpdateDomainRsp* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::getDomain(ServerContext* context, const GetDomainReq* request, Domain* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::addDomain(ServerContext* context, const Domain* request, AddDomainRsp* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::deleteDomain(ServerContext* context, const DeleteDomainReq* request, DeleteDomainRsp* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::getMisc(ServerContext* context, const GetMiscReq* request, GetMiscRsp* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::modMisc(ServerContext* context, const Misc* request, ModMiscOpRsp* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::getRaltStatus(ServerContext* context, const RaltStatusReq* request, ServerWriter<RaltStatus>* reply)
{
	
	return Status::OK;
}

Status RaltAgentImpl::execCmd(ServerContext* context, const ExecCmdReq* request, ExecCmdRsp* reply)
{	
	

	return Status::OK;
}


