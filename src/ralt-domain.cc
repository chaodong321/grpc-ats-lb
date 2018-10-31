#include "ralt-domain.h"
#include "util-common.h"
#include <string.h>
#include <fstream>

#define RALT_DOMAIN_CONF_PATH "/opt/reyzar/can/etc/trafficserver/ralt.domain.conf"

const static int member_upper_limit = 500;	//maximum member domain size (if aes config's value bigger than it, use it)
const static int member_lower_limit = 20;	//default minimum member domain size.

RaltDomain::RaltDomain()
{
	m_nMemberNum = 0;
}

RaltDomain::~RaltDomain()
{
}

RaltDomain* RaltDomain::GetInstance()
{
	static RaltDomain instance;
	return &instance;
}

bool RaltDomain::ParseDomainConf()
{
	m_nMemberNum = 0;
	m_DomainMap.clear();
	
	char buf[8192]={0};
	fstream fs2(RALT_DOMAIN_CONF_PATH, fstream::in);

	//to get the number of domain members by encrypt process with /etc/reyzar.product (by huangwanlong 20160922)
	int _member_limit = 0;
	FILE *fstream = NULL;
	char member_limit[1024] = { 0 };
#if 1
	if ((fstream = popen("/opt/reyzar/tools/product_lookup -d", "r")) == NULL)
	{
		printf("execute command failed:%s\n", strerror(errno));
	}
#endif

	if (NULL != fgets(member_limit, sizeof(member_limit), fstream))
	{
		printf("Domain member number is : %s\n", member_limit);
		_member_limit = atoi(member_limit);
	}
	else
	{
		_member_limit = member_lower_limit;
	}
	pclose(fstream);

	if (!_member_limit)	//if license picked on to zero
	{
		_member_limit = member_lower_limit;
	} 
	else if (_member_limit < 0 || _member_limit > member_upper_limit)
	{
		_member_limit = member_upper_limit;
	}

	int domain_counts = 0;

	while (fs2.getline(buf, 8192)) {
		DomainValue member;
		if ( strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_member_domain", strlen("ralt_member_domain")) == 0 
				&& ( (!_member_limit) || domain_counts < _member_limit ) )
		{
			const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen("ralt_member_domain");
			
			begin = UtilCommon::ToNextNotChar(begin, buf + 8191, "*. \t", 4);
			const char *end = UtilCommon::ToNextChar(begin, buf + 8191, " \t;", 3);

			if ( end - begin > 0 )
			{
				member.str_domain.assign(begin, end - begin);
				printf("str_domain: %s\n", member.str_domain.c_str());

				if( m_DomainMap.find(member.str_domain) != m_DomainMap.end() )
				{
					continue;
				}

				begin = UtilCommon::ToNextNotChar(end, buf + 8191, "*. \t", 4);
				end = UtilCommon::ToNextChar(begin, buf + 8191, " \t;", 3);

				member.str_append_or_replace = string(begin, end - begin);

                begin = UtilCommon::ToNextNotChar(end, buf + 8191, "*[ \t", 4);
                end = UtilCommon::ToNextChar(begin, buf + 8191, "] \t;", 3);

                member.str_port = string(begin,end - begin);

				m_DomainMap.insert(make_pair(member.str_domain, member));
				m_nMemberNum++;

			}
		}
	}

	return true;
}

int RaltDomain::GetMemberNum()
{
	ParseDomainConf();
	return m_nMemberNum;
}


map<string, DomainValue>* RaltDomain::GetAllDomain()
{
	ParseDomainConf();
	return &m_DomainMap;
}


void RaltDomain::UpdateDomain(const map<string, DomainValue>* domainMap)
{
	ofstream of;
	of.open(RALT_DOMAIN_CONF_PATH);
	for(const auto it: *domainMap){

		string strDomainType;
		
		if(DOMAIN_MEMBER == it.second.type){
			strDomainType.assign("ralt_member_domain");
		}
		else if(DOMAIN_SUBS == it.second.type){
			strDomainType.assign("ralt_subs_domain");
		}
		else{
			continue;
		}

		if(it.second.str_domain.empty() || it.second.str_append_or_replace.empty()){
			continue;
		}

		string strDomain;
		if(it.second.str_port.empty()){
			strDomain = strDomainType + " 	" + it.second.str_domain + " 	" + it.second.str_append_or_replace;
		}
		else{
			strDomain = strDomainType + " 	" + it.second.str_domain + " 	" + it.second.str_append_or_replace + " 	" + it.second.str_port;
		}

		of << strDomain << endl;
	}
	of.close();
}

DomainValue RaltDomain::GetDomain(string strDomain)
{
	ParseDomainConf();
	return m_DomainMap[strDomain];
}

void RaltDomain::AddDomain(const DomainValue *domain)
{
	if(NULL == domain)
		return;
	
	ParseDomainConf();
	m_DomainMap.insert(make_pair(domain->str_domain, *domain));
	m_nMemberNum++;

	string strType;
	if(DOMAIN_SUBS == domain->type){
		strType = "ralt_subs_domain";
	}
	else{
		strType = "ralt_member_domain";		//default
	}
	
	string strDomainMember = strType + string("	") + domain->str_domain + string("	") 
									+ domain->str_append_or_replace + "	" + domain->str_port;
	
	ofstream of;
	of.open(RALT_DOMAIN_CONF_PATH, ios::app);
	of << strDomainMember << endl;
	of.close();
}

void RaltDomain::DelDomain(string strDomain)
{
	ParseDomainConf();
	if( m_DomainMap.find(strDomain) != m_DomainMap.end())
	{
		m_DomainMap.erase(strDomain);
		m_nMemberNum--;
	}

	string strCmd = "sed -i '/"+strDomain+"/d' " + string(RALT_DOMAIN_CONF_PATH);
	system(strCmd.c_str());
}

