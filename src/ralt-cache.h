#ifndef __RALT_CACHE_H_
#define __RALT_CACHE_H_

#include <string>
#include <map>

using namespace std;

class RaltCache{
public:
    static RaltCache* GetInstance();
    string GetRegexCacheUrl(string strRegex);
    bool IsUrlInCache(string strUrl);

private:
    RaltCache();
    ~RaltCache();
};

#endif
