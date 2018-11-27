// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: ralt-service.proto

#include "ralt-service.pb.h"
#include "ralt-service.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace raltservice {

static const char* RaltService_method_names[] = {
  "/raltservice.RaltService/getRaltStats",
  "/raltservice.RaltService/getStatsFieldValue",
  "/raltservice.RaltService/getHomePageData",
  "/raltservice.RaltService/showCacheData",
  "/raltservice.RaltService/showFlowStatData",
  "/raltservice.RaltService/showLogInfoData",
  "/raltservice.RaltService/getRaltLogs",
  "/raltservice.RaltService/getBasicConfig",
  "/raltservice.RaltService/setBasicConfig",
  "/raltservice.RaltService/getAllDomain",
  "/raltservice.RaltService/updateDomain",
  "/raltservice.RaltService/getDomain",
  "/raltservice.RaltService/addDomain",
  "/raltservice.RaltService/deleteDomain",
  "/raltservice.RaltService/getMisc",
  "/raltservice.RaltService/modMisc",
  "/raltservice.RaltService/getRaltStatus",
  "/raltservice.RaltService/execCmd",
};

std::unique_ptr< RaltService::Stub> RaltService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< RaltService::Stub> stub(new RaltService::Stub(channel));
  return stub;
}

RaltService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_getRaltStats_(RaltService_method_names[0], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_getStatsFieldValue_(RaltService_method_names[1], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_getHomePageData_(RaltService_method_names[2], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_showCacheData_(RaltService_method_names[3], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_showFlowStatData_(RaltService_method_names[4], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_showLogInfoData_(RaltService_method_names[5], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_getRaltLogs_(RaltService_method_names[6], ::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_getBasicConfig_(RaltService_method_names[7], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_setBasicConfig_(RaltService_method_names[8], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_getAllDomain_(RaltService_method_names[9], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_updateDomain_(RaltService_method_names[10], ::grpc::internal::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_getDomain_(RaltService_method_names[11], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_addDomain_(RaltService_method_names[12], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_deleteDomain_(RaltService_method_names[13], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_getMisc_(RaltService_method_names[14], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_modMisc_(RaltService_method_names[15], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_getRaltStatus_(RaltService_method_names[16], ::grpc::internal::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_execCmd_(RaltService_method_names[17], ::grpc::internal::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status RaltService::Stub::getRaltStats(::grpc::ClientContext* context, const ::raltservice::GetRaltStatsReq& request, ::raltservice::GetRaltStatsRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_getRaltStats_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetRaltStatsRsp>* RaltService::Stub::AsyncgetRaltStatsRaw(::grpc::ClientContext* context, const ::raltservice::GetRaltStatsReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetRaltStatsRsp>::Create(channel_.get(), cq, rpcmethod_getRaltStats_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetRaltStatsRsp>* RaltService::Stub::PrepareAsyncgetRaltStatsRaw(::grpc::ClientContext* context, const ::raltservice::GetRaltStatsReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetRaltStatsRsp>::Create(channel_.get(), cq, rpcmethod_getRaltStats_, context, request, false);
}

::grpc::Status RaltService::Stub::getStatsFieldValue(::grpc::ClientContext* context, const ::raltservice::StatsFieldName& request, ::raltservice::StatsFieldValue* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_getStatsFieldValue_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::StatsFieldValue>* RaltService::Stub::AsyncgetStatsFieldValueRaw(::grpc::ClientContext* context, const ::raltservice::StatsFieldName& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::StatsFieldValue>::Create(channel_.get(), cq, rpcmethod_getStatsFieldValue_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::StatsFieldValue>* RaltService::Stub::PrepareAsyncgetStatsFieldValueRaw(::grpc::ClientContext* context, const ::raltservice::StatsFieldName& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::StatsFieldValue>::Create(channel_.get(), cq, rpcmethod_getStatsFieldValue_, context, request, false);
}

::grpc::Status RaltService::Stub::getHomePageData(::grpc::ClientContext* context, const ::raltservice::HomePageReq& request, ::raltservice::HomePageRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_getHomePageData_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::HomePageRsp>* RaltService::Stub::AsyncgetHomePageDataRaw(::grpc::ClientContext* context, const ::raltservice::HomePageReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::HomePageRsp>::Create(channel_.get(), cq, rpcmethod_getHomePageData_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::HomePageRsp>* RaltService::Stub::PrepareAsyncgetHomePageDataRaw(::grpc::ClientContext* context, const ::raltservice::HomePageReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::HomePageRsp>::Create(channel_.get(), cq, rpcmethod_getHomePageData_, context, request, false);
}

::grpc::Status RaltService::Stub::showCacheData(::grpc::ClientContext* context, const ::raltservice::CacheLookUpReq& request, ::raltservice::CacheResult* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_showCacheData_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::CacheResult>* RaltService::Stub::AsyncshowCacheDataRaw(::grpc::ClientContext* context, const ::raltservice::CacheLookUpReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::CacheResult>::Create(channel_.get(), cq, rpcmethod_showCacheData_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::CacheResult>* RaltService::Stub::PrepareAsyncshowCacheDataRaw(::grpc::ClientContext* context, const ::raltservice::CacheLookUpReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::CacheResult>::Create(channel_.get(), cq, rpcmethod_showCacheData_, context, request, false);
}

::grpc::Status RaltService::Stub::showFlowStatData(::grpc::ClientContext* context, const ::raltservice::FlowStatLookUpReq& request, ::raltservice::FlowResult* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_showFlowStatData_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::FlowResult>* RaltService::Stub::AsyncshowFlowStatDataRaw(::grpc::ClientContext* context, const ::raltservice::FlowStatLookUpReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::FlowResult>::Create(channel_.get(), cq, rpcmethod_showFlowStatData_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::FlowResult>* RaltService::Stub::PrepareAsyncshowFlowStatDataRaw(::grpc::ClientContext* context, const ::raltservice::FlowStatLookUpReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::FlowResult>::Create(channel_.get(), cq, rpcmethod_showFlowStatData_, context, request, false);
}

::grpc::Status RaltService::Stub::showLogInfoData(::grpc::ClientContext* context, const ::raltservice::LogInfoLookUpReq& request, ::raltservice::LogResult* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_showLogInfoData_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::LogResult>* RaltService::Stub::AsyncshowLogInfoDataRaw(::grpc::ClientContext* context, const ::raltservice::LogInfoLookUpReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::LogResult>::Create(channel_.get(), cq, rpcmethod_showLogInfoData_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::LogResult>* RaltService::Stub::PrepareAsyncshowLogInfoDataRaw(::grpc::ClientContext* context, const ::raltservice::LogInfoLookUpReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::LogResult>::Create(channel_.get(), cq, rpcmethod_showLogInfoData_, context, request, false);
}

::grpc::ClientReader< ::raltservice::RaltLogs>* RaltService::Stub::getRaltLogsRaw(::grpc::ClientContext* context, const ::raltservice::GetRaltLogsReq& request) {
  return ::grpc::internal::ClientReaderFactory< ::raltservice::RaltLogs>::Create(channel_.get(), rpcmethod_getRaltLogs_, context, request);
}

::grpc::ClientAsyncReader< ::raltservice::RaltLogs>* RaltService::Stub::AsyncgetRaltLogsRaw(::grpc::ClientContext* context, const ::raltservice::GetRaltLogsReq& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::raltservice::RaltLogs>::Create(channel_.get(), cq, rpcmethod_getRaltLogs_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::raltservice::RaltLogs>* RaltService::Stub::PrepareAsyncgetRaltLogsRaw(::grpc::ClientContext* context, const ::raltservice::GetRaltLogsReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::raltservice::RaltLogs>::Create(channel_.get(), cq, rpcmethod_getRaltLogs_, context, request, false, nullptr);
}

::grpc::Status RaltService::Stub::getBasicConfig(::grpc::ClientContext* context, const ::raltservice::GetBasicConfigReq& request, ::raltservice::GetBasicConfigRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_getBasicConfig_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetBasicConfigRsp>* RaltService::Stub::AsyncgetBasicConfigRaw(::grpc::ClientContext* context, const ::raltservice::GetBasicConfigReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetBasicConfigRsp>::Create(channel_.get(), cq, rpcmethod_getBasicConfig_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetBasicConfigRsp>* RaltService::Stub::PrepareAsyncgetBasicConfigRaw(::grpc::ClientContext* context, const ::raltservice::GetBasicConfigReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetBasicConfigRsp>::Create(channel_.get(), cq, rpcmethod_getBasicConfig_, context, request, false);
}

::grpc::Status RaltService::Stub::setBasicConfig(::grpc::ClientContext* context, const ::raltservice::SetBasicConfigReq& request, ::raltservice::SetBasicConfigRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_setBasicConfig_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::SetBasicConfigRsp>* RaltService::Stub::AsyncsetBasicConfigRaw(::grpc::ClientContext* context, const ::raltservice::SetBasicConfigReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::SetBasicConfigRsp>::Create(channel_.get(), cq, rpcmethod_setBasicConfig_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::SetBasicConfigRsp>* RaltService::Stub::PrepareAsyncsetBasicConfigRaw(::grpc::ClientContext* context, const ::raltservice::SetBasicConfigReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::SetBasicConfigRsp>::Create(channel_.get(), cq, rpcmethod_setBasicConfig_, context, request, false);
}

::grpc::Status RaltService::Stub::getAllDomain(::grpc::ClientContext* context, const ::raltservice::GetAllDomainReq& request, ::raltservice::GetAllDomainRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_getAllDomain_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetAllDomainRsp>* RaltService::Stub::AsyncgetAllDomainRaw(::grpc::ClientContext* context, const ::raltservice::GetAllDomainReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetAllDomainRsp>::Create(channel_.get(), cq, rpcmethod_getAllDomain_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetAllDomainRsp>* RaltService::Stub::PrepareAsyncgetAllDomainRaw(::grpc::ClientContext* context, const ::raltservice::GetAllDomainReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetAllDomainRsp>::Create(channel_.get(), cq, rpcmethod_getAllDomain_, context, request, false);
}

::grpc::ClientWriter< ::raltservice::Domain>* RaltService::Stub::updateDomainRaw(::grpc::ClientContext* context, ::raltservice::UpdateDomainRsp* response) {
  return ::grpc::internal::ClientWriterFactory< ::raltservice::Domain>::Create(channel_.get(), rpcmethod_updateDomain_, context, response);
}

::grpc::ClientAsyncWriter< ::raltservice::Domain>* RaltService::Stub::AsyncupdateDomainRaw(::grpc::ClientContext* context, ::raltservice::UpdateDomainRsp* response, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::raltservice::Domain>::Create(channel_.get(), cq, rpcmethod_updateDomain_, context, response, true, tag);
}

::grpc::ClientAsyncWriter< ::raltservice::Domain>* RaltService::Stub::PrepareAsyncupdateDomainRaw(::grpc::ClientContext* context, ::raltservice::UpdateDomainRsp* response, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncWriterFactory< ::raltservice::Domain>::Create(channel_.get(), cq, rpcmethod_updateDomain_, context, response, false, nullptr);
}

::grpc::Status RaltService::Stub::getDomain(::grpc::ClientContext* context, const ::raltservice::GetDomainReq& request, ::raltservice::GetDomainRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_getDomain_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetDomainRsp>* RaltService::Stub::AsyncgetDomainRaw(::grpc::ClientContext* context, const ::raltservice::GetDomainReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetDomainRsp>::Create(channel_.get(), cq, rpcmethod_getDomain_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetDomainRsp>* RaltService::Stub::PrepareAsyncgetDomainRaw(::grpc::ClientContext* context, const ::raltservice::GetDomainReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetDomainRsp>::Create(channel_.get(), cq, rpcmethod_getDomain_, context, request, false);
}

::grpc::Status RaltService::Stub::addDomain(::grpc::ClientContext* context, const ::raltservice::Domain& request, ::raltservice::AddDomainRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_addDomain_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::AddDomainRsp>* RaltService::Stub::AsyncaddDomainRaw(::grpc::ClientContext* context, const ::raltservice::Domain& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::AddDomainRsp>::Create(channel_.get(), cq, rpcmethod_addDomain_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::AddDomainRsp>* RaltService::Stub::PrepareAsyncaddDomainRaw(::grpc::ClientContext* context, const ::raltservice::Domain& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::AddDomainRsp>::Create(channel_.get(), cq, rpcmethod_addDomain_, context, request, false);
}

::grpc::Status RaltService::Stub::deleteDomain(::grpc::ClientContext* context, const ::raltservice::DeleteDomainReq& request, ::raltservice::DeleteDomainRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_deleteDomain_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::DeleteDomainRsp>* RaltService::Stub::AsyncdeleteDomainRaw(::grpc::ClientContext* context, const ::raltservice::DeleteDomainReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::DeleteDomainRsp>::Create(channel_.get(), cq, rpcmethod_deleteDomain_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::DeleteDomainRsp>* RaltService::Stub::PrepareAsyncdeleteDomainRaw(::grpc::ClientContext* context, const ::raltservice::DeleteDomainReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::DeleteDomainRsp>::Create(channel_.get(), cq, rpcmethod_deleteDomain_, context, request, false);
}

::grpc::Status RaltService::Stub::getMisc(::grpc::ClientContext* context, const ::raltservice::GetMiscReq& request, ::raltservice::GetMiscRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_getMisc_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetMiscRsp>* RaltService::Stub::AsyncgetMiscRaw(::grpc::ClientContext* context, const ::raltservice::GetMiscReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetMiscRsp>::Create(channel_.get(), cq, rpcmethod_getMisc_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::GetMiscRsp>* RaltService::Stub::PrepareAsyncgetMiscRaw(::grpc::ClientContext* context, const ::raltservice::GetMiscReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::GetMiscRsp>::Create(channel_.get(), cq, rpcmethod_getMisc_, context, request, false);
}

::grpc::Status RaltService::Stub::modMisc(::grpc::ClientContext* context, const ::raltservice::Misc& request, ::raltservice::ModMiscOpRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_modMisc_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::ModMiscOpRsp>* RaltService::Stub::AsyncmodMiscRaw(::grpc::ClientContext* context, const ::raltservice::Misc& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::ModMiscOpRsp>::Create(channel_.get(), cq, rpcmethod_modMisc_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::ModMiscOpRsp>* RaltService::Stub::PrepareAsyncmodMiscRaw(::grpc::ClientContext* context, const ::raltservice::Misc& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::ModMiscOpRsp>::Create(channel_.get(), cq, rpcmethod_modMisc_, context, request, false);
}

::grpc::ClientReader< ::raltservice::RaltStatus>* RaltService::Stub::getRaltStatusRaw(::grpc::ClientContext* context, const ::raltservice::RaltStatusReq& request) {
  return ::grpc::internal::ClientReaderFactory< ::raltservice::RaltStatus>::Create(channel_.get(), rpcmethod_getRaltStatus_, context, request);
}

::grpc::ClientAsyncReader< ::raltservice::RaltStatus>* RaltService::Stub::AsyncgetRaltStatusRaw(::grpc::ClientContext* context, const ::raltservice::RaltStatusReq& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::raltservice::RaltStatus>::Create(channel_.get(), cq, rpcmethod_getRaltStatus_, context, request, true, tag);
}

::grpc::ClientAsyncReader< ::raltservice::RaltStatus>* RaltService::Stub::PrepareAsyncgetRaltStatusRaw(::grpc::ClientContext* context, const ::raltservice::RaltStatusReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncReaderFactory< ::raltservice::RaltStatus>::Create(channel_.get(), cq, rpcmethod_getRaltStatus_, context, request, false, nullptr);
}

::grpc::Status RaltService::Stub::execCmd(::grpc::ClientContext* context, const ::raltservice::ExecCmdReq& request, ::raltservice::ExecCmdRsp* response) {
  return ::grpc::internal::BlockingUnaryCall(channel_.get(), rpcmethod_execCmd_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::raltservice::ExecCmdRsp>* RaltService::Stub::AsyncexecCmdRaw(::grpc::ClientContext* context, const ::raltservice::ExecCmdReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::ExecCmdRsp>::Create(channel_.get(), cq, rpcmethod_execCmd_, context, request, true);
}

::grpc::ClientAsyncResponseReader< ::raltservice::ExecCmdRsp>* RaltService::Stub::PrepareAsyncexecCmdRaw(::grpc::ClientContext* context, const ::raltservice::ExecCmdReq& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::internal::ClientAsyncResponseReaderFactory< ::raltservice::ExecCmdRsp>::Create(channel_.get(), cq, rpcmethod_execCmd_, context, request, false);
}

RaltService::Service::Service() {
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[0],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::GetRaltStatsReq, ::raltservice::GetRaltStatsRsp>(
          std::mem_fn(&RaltService::Service::getRaltStats), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[1],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::StatsFieldName, ::raltservice::StatsFieldValue>(
          std::mem_fn(&RaltService::Service::getStatsFieldValue), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[2],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::HomePageReq, ::raltservice::HomePageRsp>(
          std::mem_fn(&RaltService::Service::getHomePageData), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[3],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::CacheLookUpReq, ::raltservice::CacheResult>(
          std::mem_fn(&RaltService::Service::showCacheData), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[4],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::FlowStatLookUpReq, ::raltservice::FlowResult>(
          std::mem_fn(&RaltService::Service::showFlowStatData), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[5],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::LogInfoLookUpReq, ::raltservice::LogResult>(
          std::mem_fn(&RaltService::Service::showLogInfoData), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[6],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< RaltService::Service, ::raltservice::GetRaltLogsReq, ::raltservice::RaltLogs>(
          std::mem_fn(&RaltService::Service::getRaltLogs), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[7],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::GetBasicConfigReq, ::raltservice::GetBasicConfigRsp>(
          std::mem_fn(&RaltService::Service::getBasicConfig), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[8],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::SetBasicConfigReq, ::raltservice::SetBasicConfigRsp>(
          std::mem_fn(&RaltService::Service::setBasicConfig), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[9],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::GetAllDomainReq, ::raltservice::GetAllDomainRsp>(
          std::mem_fn(&RaltService::Service::getAllDomain), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[10],
      ::grpc::internal::RpcMethod::CLIENT_STREAMING,
      new ::grpc::internal::ClientStreamingHandler< RaltService::Service, ::raltservice::Domain, ::raltservice::UpdateDomainRsp>(
          std::mem_fn(&RaltService::Service::updateDomain), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[11],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::GetDomainReq, ::raltservice::GetDomainRsp>(
          std::mem_fn(&RaltService::Service::getDomain), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[12],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::Domain, ::raltservice::AddDomainRsp>(
          std::mem_fn(&RaltService::Service::addDomain), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[13],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::DeleteDomainReq, ::raltservice::DeleteDomainRsp>(
          std::mem_fn(&RaltService::Service::deleteDomain), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[14],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::GetMiscReq, ::raltservice::GetMiscRsp>(
          std::mem_fn(&RaltService::Service::getMisc), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[15],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::Misc, ::raltservice::ModMiscOpRsp>(
          std::mem_fn(&RaltService::Service::modMisc), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[16],
      ::grpc::internal::RpcMethod::SERVER_STREAMING,
      new ::grpc::internal::ServerStreamingHandler< RaltService::Service, ::raltservice::RaltStatusReq, ::raltservice::RaltStatus>(
          std::mem_fn(&RaltService::Service::getRaltStatus), this)));
  AddMethod(new ::grpc::internal::RpcServiceMethod(
      RaltService_method_names[17],
      ::grpc::internal::RpcMethod::NORMAL_RPC,
      new ::grpc::internal::RpcMethodHandler< RaltService::Service, ::raltservice::ExecCmdReq, ::raltservice::ExecCmdRsp>(
          std::mem_fn(&RaltService::Service::execCmd), this)));
}

RaltService::Service::~Service() {
}

::grpc::Status RaltService::Service::getRaltStats(::grpc::ServerContext* context, const ::raltservice::GetRaltStatsReq* request, ::raltservice::GetRaltStatsRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::getStatsFieldValue(::grpc::ServerContext* context, const ::raltservice::StatsFieldName* request, ::raltservice::StatsFieldValue* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::getHomePageData(::grpc::ServerContext* context, const ::raltservice::HomePageReq* request, ::raltservice::HomePageRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::showCacheData(::grpc::ServerContext* context, const ::raltservice::CacheLookUpReq* request, ::raltservice::CacheResult* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::showFlowStatData(::grpc::ServerContext* context, const ::raltservice::FlowStatLookUpReq* request, ::raltservice::FlowResult* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::showLogInfoData(::grpc::ServerContext* context, const ::raltservice::LogInfoLookUpReq* request, ::raltservice::LogResult* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::getRaltLogs(::grpc::ServerContext* context, const ::raltservice::GetRaltLogsReq* request, ::grpc::ServerWriter< ::raltservice::RaltLogs>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::getBasicConfig(::grpc::ServerContext* context, const ::raltservice::GetBasicConfigReq* request, ::raltservice::GetBasicConfigRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::setBasicConfig(::grpc::ServerContext* context, const ::raltservice::SetBasicConfigReq* request, ::raltservice::SetBasicConfigRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::getAllDomain(::grpc::ServerContext* context, const ::raltservice::GetAllDomainReq* request, ::raltservice::GetAllDomainRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::updateDomain(::grpc::ServerContext* context, ::grpc::ServerReader< ::raltservice::Domain>* reader, ::raltservice::UpdateDomainRsp* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::getDomain(::grpc::ServerContext* context, const ::raltservice::GetDomainReq* request, ::raltservice::GetDomainRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::addDomain(::grpc::ServerContext* context, const ::raltservice::Domain* request, ::raltservice::AddDomainRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::deleteDomain(::grpc::ServerContext* context, const ::raltservice::DeleteDomainReq* request, ::raltservice::DeleteDomainRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::getMisc(::grpc::ServerContext* context, const ::raltservice::GetMiscReq* request, ::raltservice::GetMiscRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::modMisc(::grpc::ServerContext* context, const ::raltservice::Misc* request, ::raltservice::ModMiscOpRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::getRaltStatus(::grpc::ServerContext* context, const ::raltservice::RaltStatusReq* request, ::grpc::ServerWriter< ::raltservice::RaltStatus>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RaltService::Service::execCmd(::grpc::ServerContext* context, const ::raltservice::ExecCmdReq* request, ::raltservice::ExecCmdRsp* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace raltservice

