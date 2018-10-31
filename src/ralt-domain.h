#ifndef __RALT_DOMAIN_H_
#define __RALT_DOMAIN_H_

#include <string>
#include <map>

using namespace std;

enum DOMAIN_TYPE{
	DOMAIN_MEMBER,
	DOMAIN_SUBS
};

typedef struct _DomainValue
{
	int type;
	string str_domain;  // domain menber
	string str_append_or_replace;  // append or replace string
	string str_port;    // domain port
}DomainValue;

class RaltDomain{
public:
	static RaltDomain* GetInstance();
	bool ParseDomainConf();
	int GetMemberNum();
	map<string, DomainValue>* GetAllDomain();
	void UpdateDomain(const map<string, DomainValue>* domainMap);
	DomainValue GetDomain(string strDomain);
	void AddDomain(const DomainValue *domain);
	void DelDomain(string strDomain);

private:
	RaltDomain();
	~RaltDomain();

private:
	int m_nMemberNum;
	map<string, DomainValue> m_DomainMap;
};

#endif
