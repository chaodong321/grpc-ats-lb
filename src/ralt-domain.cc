#include "ralt-domain.h"
#include "util-common.h"
#include "util-log.h"
#include <string.h>
#include <fstream>

#define RALT_DOMAIN_CONF_PATH "/opt/reyzar/can/etc/trafficserver/ralt.domain.conf"
#define RALT_DOMAIN_CONF_BAK_PATH "/opt/reyzar/can/etc/trafficserver/ralt.domain.conf.bak"

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
		LOG_INFO("execute command failed:%s", strerror(errno));
	}
#endif

	if (NULL != fgets(member_limit, sizeof(member_limit), fstream))
	{
		LOG_INFO("Domain member number is : %s", member_limit);
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
		LOG_INFO("ralt domain line: %s", buf);
		if ( strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_member_domain", strlen("ralt_member_domain")) == 0 
				&& ( (!_member_limit) || domain_counts < _member_limit ) ){
				
			const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen("ralt_member_domain");
			
			begin = UtilCommon::ToNextNotChar(begin, buf + 8191, "*. \t", 4);
			const char *end = UtilCommon::ToNextChar(begin, buf + 8191, " \t;", 3);

			if ( end - begin > 0 )
			{
				member.type = DOMAIN_MEMBER;
				member.str_domain.assign(begin, end - begin);
				LOG_INFO("domain: %s", member.str_domain.c_str());

				if( m_DomainMap.find(member.str_domain) != m_DomainMap.end() )
				{
					continue;
				}

				begin = UtilCommon::ToNextNotChar(end, buf + 8191, "*. \t", 4);
				end = UtilCommon::ToNextChar(begin, buf + 8191, " \t;", 3);

				member.str_append_or_replace = string(begin, end - begin);
				LOG_INFO("append or replace string: %s", member.str_append_or_replace.c_str());

                begin = UtilCommon::ToNextNotChar(end, buf + 8191, "*[ \t", 4);
                end = UtilCommon::ToNextChar(begin, buf + 8191, "] \t;", 3);

                member.str_port = string(begin,end - begin);
				LOG_INFO("port: %s", member.str_append_or_replace.c_str());

				m_DomainMap.insert(make_pair(member.str_domain, member));
				m_nMemberNum++;

			}
		}
		else if( strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_subs_domain", strlen("ralt_subs_domain")) == 0){

			LOG_INFO("find ralt_subs_domain");
			member.type = DOMAIN_SUBS;
			
			//Do not set the port separately
			string strLine(buf);
			size_t first_square = strLine.find("]");
			size_t second_square = strLine.find("[", first_square+1);
			size_t third_square = strLine.find("]", first_square+1);

			if(first_square == string::npos 
				|| second_square == string::npos 
				|| third_square == string::npos){
				continue;
			}

			if(third_square < second_square){
				//the 6/4 format
				LOG_INFO("[v6] [v4] format");
				const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen("ralt_subs_domain");
				//make a Good compatibility with the old product nginx ,so we must support for:
				//ralt_subs_domain        [//192.168.52.132] [//[111::16]];
				begin = UtilCommon::ToNextNotChar(begin, buf + 8191, " \t[", 3);
				const char *end = UtilCommon::ToNextChar(begin, buf + 8191, " \t]", 3);

				if (strncmp(begin, "//[", 3) == 0)
					begin += 2;

				//the pattern string is "[[", not "[ ["
				if (strncmp(begin -2 , "[[", 2) == 0)
					begin--;

				//the pattern string is "]]", not "] ]"
				if (strncmp(end,"]]",2) == 0)
					++end;

				if (strncmp(end,"]:",2) == 0) {
					end += 2;
					end = UtilCommon::ToNextChar(end, buf + 8191, "]", 1);
				}

				member.str_domain = string(begin, end - begin);
				LOG_INFO("domain: %s", member.str_domain.c_str());

				begin = UtilCommon::ToNextChar(end, buf + 8191, "\t[", 3);
				end = UtilCommon::ToNextChar(begin, buf + 8191, "\t]", 3);

				//make a Good compatibility with the old product nginx ,so we must support for:
				//ralt_subs_domain        [//192.168.52.132] [//[111::16]];
                begin++;
				if(strncmp(begin,"//",2) == 0)
					begin += 2;

				member.str_append_or_replace = string(begin, end - begin);
				LOG_INFO("append or replace string: %s", member.str_append_or_replace.c_str());

				m_DomainMap.insert(make_pair(member.str_domain, member));
			}
			else{
				//[v4] [v6] or [string] [string]
				LOG_INFO("[v4] [v6] or [string] [string]");
				const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen("ralt_subs_domain");
				begin = UtilCommon::ToNextNotChar(begin, buf + 8191, " \t[", 3);
				const char *end = UtilCommon::ToNextChar(begin, buf + 8191, " \t]", 3);

				//make a Good compatibility with the old product nginx ,so we must support for:
				//ralt_subs_domain        [//192.168.52.132] [//[111::16]];
				if(strncmp(begin,"//",2) == 0)
					begin += 2;

				member.str_domain = string(begin, end - begin);
				LOG_INFO("member.str_domain:%s", member.str_domain.c_str());

				begin = UtilCommon::ToNextNotChar(end, buf + 8191, "] \t[", 4);
				end = UtilCommon::ToNextChar(begin, buf + 8191, ";]\n \t", 5);

				//make a Good compatibility with the old product nginx ,so we must support for:
				//ralt_subs_domain        [//192.168.52.132] [//[111::16]];

				if (strncmp(begin, "//[", 3) == 0)
					begin += 2;

				//the pattern string is "[[", not "[ ["
				if (strncmp(begin -2 , "[[", 2) == 0)
					begin--;

				//the pattern string is "]]", not "] ]"
				if (strncmp(end,"]]",2) == 0)
					++end;

				if (strncmp(end,"]:",2) == 0) {
					end += 2;
					end = UtilCommon::ToNextChar(end, buf + 8191, "]", 1);
				}

				member.str_append_or_replace = string(begin, end - begin);
				LOG_INFO("member.str_append_or_replace:%s", member.str_append_or_replace.c_str());
				m_DomainMap.insert(make_pair(member.str_domain, member));
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


map<string, DomainValue>& RaltDomain::GetAllDomain()
{
	ParseDomainConf();
	return m_DomainMap;
}


void RaltDomain::UpdateDomain(const map<string, DomainValue>* domainMap)
{
	if(!domainMap->empty()){
		rename(RALT_DOMAIN_CONF_PATH, RALT_DOMAIN_CONF_BAK_PATH);
	}
	
	ofstream of;
	of.open(RALT_DOMAIN_CONF_PATH);
	for(const auto it: *domainMap){

		LOG_INFO("member:");
		LOG_INFO("type: %u", it.second.type);
		LOG_INFO("domain: %s", it.second.str_domain.c_str());
		LOG_INFO("append or replace string: %s", it.second.str_append_or_replace.c_str());
		LOG_INFO("port: %s", it.second.str_port.c_str());

		string strDomainType, strMember, strAppendOrReplace, strPort;
		
		if(DOMAIN_MEMBER == it.second.type){
			strDomainType.assign("ralt_member_domain");
			strMember.assign(it.second.str_domain);
			strAppendOrReplace.assign(it.second.str_append_or_replace);
			strPort = string("[") + it.second.str_port + string("]");
		}
		else if(DOMAIN_SUBS == it.second.type){
			
			strDomainType.assign("ralt_subs_domain");
			strMember = string("[") + it.second.str_domain + string("]");
			strAppendOrReplace = string("[") + it.second.str_append_or_replace + string("]");
			strPort = string("[") + it.second.str_port + string("]");
			
		}
		else{
			continue;
		}

		if(it.second.str_domain.empty() || it.second.str_append_or_replace.empty()){
			continue;
		}

		string strDomain;
		if(it.second.str_port.empty()){
			strDomain = strDomainType + " 	" + strMember + " 	" + strAppendOrReplace;
		}
		else{
			strDomain = strDomainType + " 	" + strMember + " 	" + strAppendOrReplace + " 	" + strPort;
		}

		of << strDomain << endl;
	}
	of.close();
}

void RaltDomain::GetDomain(const string &strDomain, const string &strTransDomain,
    map<string, DomainValue> &domainMap)
{
	ParseDomainConf();
	if( !strDomain.empty() && !strTransDomain.empty()){
		for(const auto it:  m_DomainMap){
			if(it.first.find(strDomain) != string::npos && it.second.type == DOMAIN_MEMBER
				&& it.second.str_append_or_replace.find(strTransDomain)!= string::npos){
				domainMap[it.first] = m_DomainMap[it.first];
			}
		}
	}
	else if(!strDomain.empty() && strTransDomain.empty()){
		for(const auto it:  m_DomainMap){
			if(it.first.find(strDomain) != string::npos && it.second.type == DOMAIN_MEMBER){
				domainMap[it.first] = m_DomainMap[it.first];
			}
		}
	}
	else if(strDomain.empty() && !strTransDomain.empty()){
		for(const auto it:  m_DomainMap){
			if(it.second.type == DOMAIN_MEMBER 
				&& it.second.str_append_or_replace.find(strTransDomain) != string::npos){
				domainMap[it.second.str_domain] = m_DomainMap[it.second.str_domain];
			}
		}
	}
	else{
		for(const auto it:  m_DomainMap){
			if(it.second.type == DOMAIN_MEMBER){
				domainMap[it.second.str_domain] = m_DomainMap[it.second.str_domain];
			}
		}
	}
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

