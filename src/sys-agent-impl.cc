#include "sys-agent-impl.h"
#include "util-log.h"
#include "util-common.h"


bool SysAgentImpl::getStubByIp(string strIp)
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
		stub_sys = SysInfo::NewStub(channel);
		return true;
	}
}

Status SysAgentImpl::getNameAndIpInfo (ServerContext* server_context, const GetNameAndIpInfoReq* request,
    GetNameAndIpInfoRsp* reply)
{
	LOG_INFO("get host name ip info");
	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status = stub_sys->getNameAndIpInfo(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get host name ip successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get host name ip failed");
	}
	return status;
}

Status SysAgentImpl::getDeviceInfo (ServerContext* server_context, const GetDeviceInfoReq* request,
    GetDeviceInfoRsp* reply)
{
	LOG_INFO("get device info info");
	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}
	
	ClientContext client_context;
    Status status= stub_sys->getDeviceInfo(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get device info successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get device info failed");
	}
	return status;
}

Status SysAgentImpl::getDeviceDetail (ServerContext* server_context, const GetDeviceDetailReq* request,
    GetDeviceDetailRsp* reply)
{
	LOG_INFO("get device detail");
	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getDeviceDetail(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get device detail successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get device detail failed");
	}
	return status;
}

Status SysAgentImpl::getCpuUsage (ServerContext* server_context, const GetCpuUsageReq* request,
    GetCpuUsageRsp* reply)
{
	LOG_INFO("get cpu usage");
	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getCpuUsage(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get cpu usage successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get cpu usage failed");
	}
	return status;
}

Status SysAgentImpl::getCpuTemp (ServerContext* server_context, const GetCpuTempReq* request,
    GetCpuTempRsp* reply)
{
	LOG_INFO("get cpu temp");
	
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getCpuTemp(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get cpu temp successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get cpu temp failed");
	}
	return status;
}

Status SysAgentImpl::getMemUsage (ServerContext* server_context, const GetMemUsageReq* request,
    GetMemUsageRsp* reply)
{
	LOG_INFO("get cpu temp");
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getMemUsage(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get mem usage successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get mem usage failed");
	}
	return status;
}

Status SysAgentImpl::getNicInfo (ServerContext* server_context, const GetNicInfoReq* request,
    GetNicInfoRsp* reply)
{
	LOG_INFO("get nic info");
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getNicInfo(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get nic info successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get nic info failed");
	}
	return status;
}

Status SysAgentImpl::getHostName (ServerContext* server_context, const GetHostNameReq* request,
    GetHostNameRsp* reply)
{
	LOG_INFO("get host name");
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getHostName(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get host name successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get host name failed");
	}
	return status;
}

Status SysAgentImpl::getIpInfo (ServerContext* server_context, const GetIpInfoReq* request,
    GetIpInfoRsp* reply)
{
	LOG_INFO("get ip info");
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getIpInfo(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get ip info successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get ip info failed");
	}
	return status;
}

Status SysAgentImpl::getCpuModel (ServerContext* server_context, const GetCpuModelReq* request,
    GetCpuModelRsp* reply)
{
	LOG_INFO("get cpu model");
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getCpuModel(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get cpu model successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get cpu model failed");
	}
	return status;
}

Status SysAgentImpl::getCpuCores (ServerContext* server_context, const GetCpuCoresReq* request,
    GetCpuCoresRsp* reply)
{
	LOG_INFO("get cpu cores");
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getCpuCores(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get cpu cores successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get cpu cores failed");
	}
	return status;
}
	
Status SysAgentImpl::getMemTotal (ServerContext* server_context, const GetMemTotalReq* request,
    GetMemTotalRsp* reply)
{
	LOG_INFO("get mem total");
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getMemTotal(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get mem total successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get mem total failed");
	}
	return status;
}

Status SysAgentImpl::getEthCtrlInfo (ServerContext* server_context, const GetEthCtrlInfoReq* request,
    GetEthCtrlInfoRsp* reply)
{
	LOG_INFO("get ethernet controler info");
	if(!getStubByIp(request->ip_addr())){
		return Status::CANCELLED;
	}

	ClientContext client_context;
    Status status= stub_sys->getEthCtrlInfo(&client_context, *request, reply);
	if (status.ok()) {
		LOG_INFO("get ethernet controler info successfully");
	} else {
		LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
		LOG_INFO("get ethernet controler info failed");
	}
	return status;
}

