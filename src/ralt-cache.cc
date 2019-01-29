#include "ralt-cache.h"
#include "util-common.h"
#include "util-log.h"

RaltCache::RaltCache()
{
    
}

RaltCache::~RaltCache()
{
}

RaltCache* RaltCache::GetInstance()
{
    static RaltCache instance;
    return &instance;
}

string RaltCache::GetRegexCacheUrl(string strRegex)
{
    string strCmd = "curl -s http://127.0.0.1/myCI/lookup_regex?url=http://"+strRegex;
    string strRegexCacheUrl;

    LOG_INFO("cmd: %s", strCmd.c_str());
    if(!UtilCommon::ShellCmd(strCmd, strRegexCacheUrl)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return "";
    }

    return strRegexCacheUrl;
}

bool RaltCache::IsUrlInCache(string strUrl)
{
    string strCmd, strRet;
    if(strUrl.empty()){
        return false;
    }
    if(strUrl.find("http://") != string::npos){
        strCmd = string("curl -s http://127.0.0.1/myCI/lookup_url?url=") + strUrl;
    }
    else{
        strCmd = "curl -s http://127.0.0.1/myCI/lookup_url?url=http://"+strUrl;
    }

    LOG_INFO("cmd: %s", strCmd.c_str());
    if(!UtilCommon::ShellCmd(strCmd, strRet)){
        LOG_ERROR("shell cmd failed: %s\n", strCmd.c_str());
        return false;
    }

    if(!strRet.empty() && strRet.find("Cache Lookup Failed") == string::npos){
        return true;
    }
    else{
        return false;
    }
}

