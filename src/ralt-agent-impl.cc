#include "ralt-agent-impl.h"
#include "util-log.h"
#include "util-common.h"

bool RaltAgentImpl::getStubByIp(string strIp)
{
	LOG_INFO("ip addr: %s", strIp.c_str());
	if(strIp.empty()){
		LOG_INFO("get all ip message");
		//deal with on server now
		strIp = "10.2.1.114";
	}

	if(!UtilCommon::IsIp(strIp.c_str())){
		LOG_ERROR("the format of ip is error, ip: %s", strIp.c_str());
		return false;
	}
	else{
		LOG_INFO("create stub");
		string strIpAndPort = strIp + string(":50053");
		shared_ptr<Channel> channel(grpc::CreateChannel(strIpAndPort.c_str(), grpc::InsecureChannelCredentials()));
		stub_ralt = RaltService::NewStub(channel);
		return true;
	}
}

Status RaltAgentImpl::getRaltStats (ServerContext* server_context, const GetRaltStatsReq* request,
    GetRaltStatsRsp* reply)
{
	LOG_INFO("get ralt stats");
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub_ralt->getRaltStats(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get ralt stats successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get ralt stats failed");
	}
	return status;
}

Status RaltAgentImpl::getStatsFieldValue (ServerContext* server_context, const StatsFieldName* request,
    StatsFieldValue* reply)
{
	LOG_INFO("get stats field value");	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
	Status status = stub_ralt->getStatsFieldValue(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get stats field value successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get stats field value failed");
	}
	return status;
}

Status RaltAgentImpl::getHomePageData (ServerContext* server_context, const HomePageReq* request,
    HomePageRsp* reply)
{
	ClientContext client_context;
    Status status = stub_ralt->getHomePageData(&client_context, *request, reply);
	return status;
}

Status RaltAgentImpl::showCacheData(ServerContext* server_context, const CacheLookUpReq* request,
    CacheResult* reply)
{
	LOG_INFO("get cache data");	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub_ralt->showCacheData(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get cache data successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get cache data failed");
	}
	return status;
}

Status RaltAgentImpl::showFlowStatData(ServerContext* server_context, const FlowStatLookUpReq* request,
    FlowResult* reply)
{
	LOG_INFO("get flow data");	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub_ralt->showFlowStatData(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get flow data successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get flow data failed");
	}
	return status;
}

Status RaltAgentImpl::showLogInfoData(ServerContext* server_context, const LogInfoLookUpReq* request,
    LogResult* reply)
{
	LOG_INFO("get log data");	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub_ralt->showLogInfoData(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get log data successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get log data failed");
	}
	return status;
}

Status RaltAgentImpl::getRaltLogs(ServerContext* server_context, const GetRaltLogsReq* request,
    ServerWriter<RaltLogs>* reply)
{
	LOG_INFO("get ralt logs");	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
	std::unique_ptr<ClientReader<RaltLogs> > reader(stub_ralt->getRaltLogs(&client_context, *request));
	RaltLogs logs;
	while (reader->Read(&logs)) {
		reply->Write(logs);
	}
	Status status = reader->Finish();
	if (status.ok()) {
		LOG_INFO("get ralt logs successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get ralt logs failed");
	}
	return status;
}

//configure
Status RaltAgentImpl::getBasicConfig(ServerContext* server_context, const GetBasicConfigReq* request,
    GetBasicConfigRsp* reply)
{
	ClientContext client_context;
    Status status = stub_ralt->getBasicConfig(&client_context, *request, reply);
	return status;
}

Status RaltAgentImpl::setBasicConfig(ServerContext* server_context, const SetBasicConfigReq* request,
    SetBasicConfigRsp* reply)
{
	ClientContext client_context;
    Status status = stub_ralt->setBasicConfig(&client_context, *request, reply);
	return status;
}

Status RaltAgentImpl::getAllDomain(ServerContext* server_context, const GetAllDomainReq* request,
    GetAllDomainRsp* reply)
{
	ClientContext client_context;
    Status status = stub_ralt->getAllDomain(&client_context, *request, reply);
	return status;
}

Status RaltAgentImpl::updateDomain(ServerContext* server_context, ServerReader<Domain>* request,
    UpdateDomainRsp* reply)
{
	ClientContext client_context;
	std::unique_ptr<ClientWriter<Domain> > writer(stub_ralt->updateDomain(&client_context, reply));
	
	Domain element;
	while (request->Read(&element)) {
		writer->Write(element);
    }

	writer->WritesDone();
	Status status = writer->Finish();

	return status;
}

Status RaltAgentImpl::getDomain(ServerContext* server_context, const GetDomainReq* request,
    GetDomainRsp* reply)
{
	ClientContext client_context;
    Status status = stub_ralt->getDomain(&client_context, *request, reply);
	return status;
}

Status RaltAgentImpl::addDomain(ServerContext* server_context, const Domain* request,
    AddDomainRsp* reply)
{
	ClientContext client_context;
    Status status = stub_ralt->addDomain(&client_context, *request, reply);
	return status;
}

Status RaltAgentImpl::deleteDomain(ServerContext* server_context, const DeleteDomainReq* request,
    DeleteDomainRsp* reply)
{
	ClientContext client_context;
    Status status = stub_ralt->deleteDomain(&client_context, *request, reply);
	return status;
}

Status RaltAgentImpl::getMisc(ServerContext* server_context, const GetMiscReq* request,
    GetMiscRsp* reply)
{
	ClientContext client_context;
    Status status = stub_ralt->getMisc(&client_context, *request, reply);
	return status;
}

Status RaltAgentImpl::modMisc(ServerContext* server_context, const Misc* request,
              ModMiscOpRsp* reply)
{
	ClientContext client_context;
    Status status = stub_ralt->modMisc(&client_context, *request, reply);
	return status;
}

//command
Status RaltAgentImpl::getRaltStatus(ServerContext* server_context, const RaltStatusReq* request,
    ServerWriter<RaltStatus>* reply)
{
	LOG_INFO("get ralt status");	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
	std::unique_ptr<ClientReader<RaltStatus> > reader(stub_ralt->getRaltStatus(&client_context, *request));
	RaltStatus ralt_status;
	while (reader->Read(&ralt_status)) {
		reply->Write(ralt_status);
	}
	Status status = reader->Finish();
	if (status.ok()) {
		LOG_INFO("get ralt status successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get ralt status failed");
	}
	return status;
}

Status RaltAgentImpl::execCmd(ServerContext* server_context, const ExecCmdReq* request,
    ExecCmdRsp* reply)
{
	LOG_INFO("exec cmd");	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub_ralt->execCmd(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("exec cmd successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("exec cmd failed");
	}
	return status;
}

