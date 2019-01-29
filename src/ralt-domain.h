#ifndef __RALT_DOMAIN_H_
#define __RALT_DOMAIN_H_

#include <string>
#include <map>

using namespace std;

//该enum与ralt-service.proto的enum DomainType一致（会涉及强制转换）
enum {
	RALT_MEMBER_DOMAIN,
	RALT_SUBS_DOMAIN,
	RALT_SUBS_DOMAIN_6TO4,
	RALT_SUBS_DOMAIN_4TO6
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
    map<string, DomainValue>& GetAllDomain();
    void UpdateDomain(const map<string, DomainValue>* domainMap);
    void GetDomain(const string &strDomain, const string &strTransDomain,
        map<string, DomainValue> &domainMap);
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
