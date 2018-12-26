#include "ralt-agent-impl.h"
#include "ralt-agent-conf.h"
#include "util-log.h"
#include "util-common.h"

unique_ptr<RaltService::Stub> RaltAgentImpl::getStubByIp(string strIp)
{
	LOG_INFO("ip addr: %s", strIp.c_str());
	
	vector<RaltServer> &server = RaltAgentConf::GetInstance().GetServer();
	if(server.size() <= 0){
		LOG_ERROR("cluster no server");
		return nullptr;
	}

	if(strIp.empty()){
		strIp = server[0].strIpAddr;
		LOG_INFO("request ip addr is empty, use the first cluster server: %s", strIp.c_str());
	}

	if(!UtilCommon::IsIp(strIp.c_str())){
		LOG_ERROR("the format of ip is error, ip: %s", strIp.c_str());
		return nullptr;
	}

	string strIpAndPort;
	for(const auto it : server){
		LOG_INFO("server ip: %s", it.strIpAddr.c_str());
		if(it.strIpAddr.find(strIp) != string::npos){
			LOG_INFO("ip addr is in cluster");
			strIpAndPort = it.strIpAddr + string(":") + to_string(it.nPort);
			break;
		}
	}

	if(strIpAndPort.empty()){
		return nullptr;
	}

	LOG_INFO("create stub");
	shared_ptr<Channel> channel(grpc::CreateChannel(strIpAndPort.c_str(), grpc::InsecureChannelCredentials()));
	unique_ptr<RaltService::Stub> stub = RaltService::NewStub(channel);
	return stub;
}

Status RaltAgentImpl::getRaltStats (ServerContext* server_context, const GetRaltStatsReq* request,
    GetRaltStatsRsp* reply)
{
	LOG_INFO("get ralt stats");
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub->getRaltStats(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get ralt stats successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get ralt stats failed");
	}
	return status;
}

Status RaltAgentImpl::getStatsField (ServerContext* server_context, const GetStatsFieldReq* request,
    GetStatsFieldRsp* reply)
{
	LOG_INFO("get stats field value");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
	Status status = stub->getStatsField(&client_context, *request, reply);
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
	LOG_INFO("get home page");
	string strIpAddr;
	vector<RaltServer> &server = RaltAgentConf::GetInstance().GetServer();
	if(server.size() > 0)
	{
		strIpAddr = server[0].strIpAddr;
	}

	unique_ptr<RaltService::Stub> stub = getStubByIp(strIpAddr);
	if(nullptr == stub){
		return Status::CANCELLED;
	}
	
	
	ClientContext client_context;
    Status status = stub->getHomePageData(&client_context, *request, reply);
	if (status.ok()) {
		vector<RaltServer> &server = RaltAgentConf::GetInstance().GetServer();
		reply->set_cluster_device(server.size());
		LOG_INFO("get home page data successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get home page data failed");
	}
	return status;
}

Status RaltAgentImpl::showCacheData(ServerContext* server_context, const CacheLookUpReq* request,
    CacheResult* reply)
{
	LOG_INFO("get cache data");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub->showCacheData(&client_context, *request, reply);
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
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub->showFlowStatData(&client_context, *request, reply);
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
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub->showLogInfoData(&client_context, *request, reply);
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
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
	std::unique_ptr<ClientReader<RaltLogs> > reader(stub->getRaltLogs(&client_context, *request));
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
	LOG_INFO("get basic config");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub->getBasicConfig(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get basic config successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get basic config failed");
	}
	return status;
}

Status RaltAgentImpl::setBasicConfig(ServerContext* server_context, const SetBasicConfigReq* request,
    SetBasicConfigRsp* reply)
{
	LOG_INFO("set basic config value");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub->setBasicConfig(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("set basic config successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("set basic config failed");
	}
	return status;
}

Status RaltAgentImpl::getAllDomain(ServerContext* server_context, const GetAllDomainReq* request,
    GetAllDomainRsp* reply)
{
	LOG_INFO("get all domain");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub->getAllDomain(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get all domain successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get all domain failed");
	}
	return status;
}

Status RaltAgentImpl::updateDomain(ServerContext* server_context, const UpdateDomainReq* request,
    UpdateDomainRsp* reply)
{
	LOG_INFO("update domain");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}
	
	ClientContext client_context;
    Status status = stub->updateDomain(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("update domain successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("update domain failed");
	}
	return status;
}

Status RaltAgentImpl::getDomain(ServerContext* server_context, const GetDomainReq* request,
    GetDomainRsp* reply)
{
	LOG_INFO("get domain");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}
	
	ClientContext client_context;
    Status status = stub->getDomain(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get domain successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get domain failed");
	}
	return status;
}

Status RaltAgentImpl::addDomain(ServerContext* server_context, const AddDomainReq* request,
    AddDomainRsp* reply)
{
	LOG_INFO("add domain");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}
	
	ClientContext client_context;
    Status status = stub->addDomain(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("add domain successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("add domain failed");
	}
	return status;
}

Status RaltAgentImpl::deleteDomain(ServerContext* server_context, const DeleteDomainReq* request,
    DeleteDomainRsp* reply)
{
	LOG_INFO("delete domain");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}
	
	ClientContext client_context;
    Status status = stub->deleteDomain(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("delete domain successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("delete domain failed");
	}
	return status;
}

Status RaltAgentImpl::getMisc(ServerContext* server_context, const GetMiscReq* request,
    GetMiscRsp* reply)
{
	LOG_INFO("get misc");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}
	
	ClientContext client_context;
    Status status = stub->getMisc(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get misc successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get misc failed");
	}
	return status;
}

Status RaltAgentImpl::modMisc(ServerContext* server_context, const ModMiscOpReq* request,
    ModMiscOpRsp* reply)
{
	LOG_INFO("mod misc");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}
	
	ClientContext client_context;
    Status status = stub->modMisc(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("mod misc successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("mod misc failed");
	}
	return status;
}

//command
Status RaltAgentImpl::getRaltStatus(ServerContext* server_context, const RaltStatusReq* request,
    ServerWriter<RaltStatus>* reply)
{
	LOG_INFO("get ralt status");	
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
	std::unique_ptr<ClientReader<RaltStatus> > reader(stub->getRaltStatus(&client_context, *request));
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
	unique_ptr<RaltService::Stub> stub = getStubByIp(request->ip_addr());
	if(nullptr == stub){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub->execCmd(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("exec cmd successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("exec cmd failed");
	}
	return status;
}

