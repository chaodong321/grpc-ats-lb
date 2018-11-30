
#ifndef __RALT_AGENT_CONF_H_
#define __RALT_AGENT_CONF_H_
#include <string>
#include <vector>

using namespace std;

typedef struct RaltServer_{
	string strIpAddr;
	unsigned int nPort;
}RaltServer;

class RaltAgentConf
{
public:
	static RaltAgentConf& GetInstance();
	bool LoadConf(string strConfPath);
	string GetIpAddr();
	unsigned int GetPort();
	vector<RaltServer>& GetServer();

private:
	string strIpAddr;
	unsigned int nPort;
	vector<RaltServer> vRaltServer;

private:
	RaltAgentConf();
	~RaltAgentConf();
};

#endif
