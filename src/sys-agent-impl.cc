#include "sys-agent-impl.h"
#include "ralt-agent-conf.h"
#include "util-log.h"
#include "util-common.h"


unique_ptr< SysInfo::Stub> SysAgentImpl::getStubByIp(string strIp)
{
    LOG_INFO("ip addr: %s", strIp.c_str());
    string strIpAndPort;
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

    for(const auto it : server){
        LOG_INFO("server ip: %s", it.strIpAddr.c_str());
        if(it.strIpAddr.find(strIp) != string::npos){
            LOG_INFO("ip addr is in cluster");
            strIpAndPort = it.strIpAddr + string(":") + to_string(it.nPort);
            break;
        }
    }

    LOG_INFO("create stub");
    shared_ptr<Channel> channel(grpc::CreateChannel(strIpAndPort.c_str(), grpc::InsecureChannelCredentials()));
    unique_ptr< SysInfo::Stub> stub = SysInfo::NewStub(channel);
    return stub;
}

Status SysAgentImpl::getNameAndIpInfo (ServerContext* server_context, const GetNameAndIpInfoReq* request,
    GetNameAndIpInfoRsp* reply)
{
    LOG_INFO("get host name ip info");

    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status = stub->getNameAndIpInfo(&client_context, *request, reply);
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
    
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }
    
    ClientContext client_context;
    Status status= stub->getDeviceInfo(&client_context, *request, reply);
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
    
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getDeviceDetail(&client_context, *request, reply);
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
    
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getCpuUsage(&client_context, *request, reply);
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
    
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getCpuTemp(&client_context, *request, reply);
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
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getMemUsage(&client_context, *request, reply);
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
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getNicInfo(&client_context, *request, reply);
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
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getHostName(&client_context, *request, reply);
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
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getIpInfo(&client_context, *request, reply);
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
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getCpuModel(&client_context, *request, reply);
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
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getCpuCores(&client_context, *request, reply);
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
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getMemTotal(&client_context, *request, reply);
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
    unique_ptr< SysInfo::Stub> stub = getStubByIp(request->ip_addr());
    if(nullptr == stub){
        return Status::CANCELLED;
    }

    ClientContext client_context;
    Status status= stub->getEthCtrlInfo(&client_context, *request, reply);
    if (status.ok()) {
        LOG_INFO("get ethernet controler info successfully");
    } else {
        LOG_INFO("error code:%d, error message: %s", status.error_code(), status.error_message().c_str());
        LOG_INFO("get ethernet controler info failed");
    }
    return status;
}

