#ifndef __RALT_SERVICE_CONF_H_
#define __RALT_SERVICE_CONF_H_
#include <string>

using namespace std;

class RaltServiceConf
{
public:
    static RaltServiceConf& GetInstance();
    bool LoadConf(string strConfPath);
    string GetIpAddr();
    unsigned int GetPort();

private:
    string strIpAddr;
    unsigned int nPort;

private:
    RaltServiceConf();
    ~RaltServiceConf();
};

#endif
