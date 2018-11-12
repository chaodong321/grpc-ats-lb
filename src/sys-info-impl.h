#ifndef __SYS_INFO_IMPL_H_
#define __SYS_INFO_IMPL_H_

#include "sys-info.grpc.pb.h"
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

using sysinfo::SysInfo;

//system info
using sysinfo::GetNameAndIpInfoReq;
using sysinfo::GetNameAndIpInfoRsp;
using sysinfo::GetDeviceInfoReq;
using sysinfo::GetDeviceInfoRsp;
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


using namespace std;

class SysInfoImpl final : public SysInfo::Service {
public:

	Status getNameAndIpInfo (ServerContext* context, const GetNameAndIpInfoReq* request,
                  GetNameAndIpInfoRsp* reply) override;

	Status getDeviceInfo (ServerContext* context, const GetDeviceInfoReq* request,
                  GetDeviceInfoRsp* reply) override;
	
	Status getCpuUsage (ServerContext* context, const GetCpuUsageReq* request,
                  GetCpuUsageRsp* reply) override;
	
	Status getCpuTemp (ServerContext* context, const GetCpuTempReq* request,
                  GetCpuTempRsp* reply) override;
	
	Status getMemUsage (ServerContext* context, const GetMemUsageReq* request,
                  GetMemUsageRsp* reply) override;
	
	Status getNicInfo (ServerContext* context, const GetNicInfoReq* request,
                  GetNicInfoRsp* reply) override;

	Status getHostName (ServerContext* context, const GetHostNameReq* request,
                  GetHostNameRsp* reply) override;

	Status getIpInfo (ServerContext* context, const GetIpInfoReq* request,
                  GetIpInfoRsp* reply) override;
	
	Status getCpuModel (ServerContext* context, const GetCpuModelReq* request,
                  GetCpuModelRsp* reply) override;
	
	Status getCpuCores (ServerContext* context, const GetCpuCoresReq* request,
                  GetCpuCoresRsp* reply) override;
	
	Status getMemTotal (ServerContext* context, const GetMemTotalReq* request,
                  GetMemTotalRsp* reply) override;
	
	Status getEthCtrlInfo (ServerContext* context, const GetEthCtrlInfoReq* request,
                  GetEthCtrlInfoRsp* reply) override;

};

#endif
