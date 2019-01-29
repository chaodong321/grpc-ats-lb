#include "ralt-domain.h"
#include "util-common.h"
#include "util-log.h"
#include <string.h>
#include <fstream>

#define RALT_DOMAIN_CONF_PATH "/opt/reyzar/can/etc/trafficserver/ralt.domain.conf"
#define RALT_DOMAIN_CONF_BAK_PATH "/opt/reyzar/can/etc/trafficserver/ralt.domain.conf.bak"

const static int member_upper_limit = 500;  //maximum member domain size (if aes config's value bigger than it, use it)
const static int member_lower_limit = 20;   //default minimum member domain size.

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

    if (!_member_limit) //if license picked on to zero
    {
        _member_limit = member_lower_limit;
    } 
    else if (_member_limit < 0 || _member_limit > member_upper_limit)
    {
        _member_limit = member_upper_limit;
    }

    int domain_counts = 0;

    while (fs2.getline(buf, 8192)) {
        LOG_INFO("ralt domain line: %s", buf);
        
        DomainValue member;
        member.type = -1;
        const char *key_word = NULL;
        
        if ( strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_member_domain", strlen("ralt_member_domain")) == 0 
                && ( (!_member_limit) || domain_counts < _member_limit ) )
        {
            key_word = "ralt_member_domain";
            member.type = RALT_MEMBER_DOMAIN;
        }
        else if (strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_subs_domain_4to6", strlen("ralt_subs_domain_4to6")) == 0) {
            key_word = "ralt_subs_domain_4to6";
            member.type = RALT_SUBS_DOMAIN_4TO6;
        }
        else if (strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_subs_domain_6to4", strlen("ralt_subs_domain_6to4")) == 0) {
            key_word = "ralt_subs_domain_6to4";
            member.type = RALT_SUBS_DOMAIN_6TO4;
        }
        else if (strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_subs_domain", strlen("ralt_subs_domain")) == 0) {
            std::string line(buf);
            //line.erase(std::remove(line.begin(), line.end(), '/'), line.end());
            size_t frist_right_brackets = line.find("]");
            size_t next_left_brackets = line.find("[", frist_right_brackets+1);
            size_t second_right_brackets = line.find("]", frist_right_brackets+1);
            if(frist_right_brackets == string::npos 
                || next_left_brackets == string::npos 
                || second_right_brackets == string::npos){
                continue;
            }

            key_word = "ralt_subs_domain";
            if(second_right_brackets < next_left_brackets){
                //the 6/4 format
                member.type = RALT_SUBS_DOMAIN_6TO4;
            }
            else{
                //[v4] [v6] or [string] [string]
                member.type = RALT_SUBS_DOMAIN;
            }
        }

        switch(member.type){
            case RALT_MEMBER_DOMAIN:
                {
                    const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen(key_word);
                
                    begin = UtilCommon::ToNextNotChar(begin, buf + 8191, "*. \t", 4);
                    const char *end = UtilCommon::ToNextChar(begin, buf + 8191, " \t;", 3);

                    if ( end - begin > 0 )
                    {
                        member.str_domain.assign(begin, end - begin);
                        

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
                        
                        LOG_INFO("domain: %s", member.str_domain.c_str());
                        LOG_INFO("append or replace string: %s", member.str_append_or_replace.c_str());
                        LOG_INFO("port: %s", member.str_append_or_replace.c_str());
                        m_DomainMap.insert(make_pair(member.str_domain, member));
                        m_nMemberNum++;

                    }
                }
                break;
            case RALT_SUBS_DOMAIN_6TO4:
                {
                    const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen(key_word);
                    begin = UtilCommon::ToNextNotChar(begin, buf + 8191, " \t[", 3);
                    const char *end = UtilCommon::ToNextChar(begin, buf + 8191, "\t]", 3);

                    //make a Good compatibility with the old product nginx ,so we must support for:
                    //ralt_subs_domain        [//192.168.52.132] [//[111::16]];
                    if(strncmp(begin,"//",2) == 0)
                        begin += 2;

                    member.str_domain = string(begin, end - begin);

                    begin = UtilCommon::ToNextNotChar(end, buf + 8191, "] \t[", 4);
                    end = UtilCommon::ToNextChar(begin, buf + 8191, ";]\n\t", 5);

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

                    LOG_INFO("search: %s", member.str_domain.c_str());
                    LOG_INFO("replace: %s", member.str_append_or_replace.c_str());
                    m_DomainMap.insert(make_pair(member.str_domain, member));
                }
                break;
            case RALT_SUBS_DOMAIN:
            case RALT_SUBS_DOMAIN_4TO6:
                {
                    const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen(key_word);
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

                    begin = UtilCommon::ToNextChar(end, buf + 8191, "\t[", 3);
                    end = UtilCommon::ToNextChar(begin, buf + 8191, "\t]", 3);

                    //make a Good compatibility with the old product nginx ,so we must support for:
                    //ralt_subs_domain        [//192.168.52.132] [//[111::16]];
                    begin++;
                    if(strncmp(begin,"//",2) == 0)
                        begin += 2;

                    member.str_append_or_replace = string(begin, end - begin);

                    LOG_INFO("member.str_append_or_replace:%s", member.str_append_or_replace.c_str());
                    m_DomainMap.insert(make_pair(member.str_domain, member));
                }
                break;
            default:
                break;
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

        int unknowm = 0;
        switch(it.second.type){
            case RALT_MEMBER_DOMAIN:
                {
                    strDomainType.assign("ralt_member_domain");
                    strMember.assign(it.second.str_domain);
                    strAppendOrReplace.assign(it.second.str_append_or_replace);
                    strPort = string("[") + it.second.str_port + string("]");
                }
                break;
            case RALT_SUBS_DOMAIN:
                {
                    strDomainType.assign("ralt_subs_domain");
                    strMember = string("[") + it.second.str_domain + string("]");
                    strAppendOrReplace = string("[") + it.second.str_append_or_replace + string("]");
                    strPort = string("[") + it.second.str_port + string("]");
                }
                break;
            case RALT_SUBS_DOMAIN_6TO4:
                {
                    strDomainType.assign("ralt_subs_domain_6to4");
                    strMember = string("[") + it.second.str_domain + string("]");
                    strAppendOrReplace = string("[") + it.second.str_append_or_replace + string("]");
                    strPort = string("[") + it.second.str_port + string("]");
                }
                break;
            case RALT_SUBS_DOMAIN_4TO6:
                {
                    strDomainType.assign("ralt_subs_domain_4to6");
                    strMember = string("[") + it.second.str_domain + string("]");
                    strAppendOrReplace = string("[") + it.second.str_append_or_replace + string("]");
                    strPort = string("[") + it.second.str_port + string("]");
                }
                break;
            default:
                unknowm = 1;
                break;
        }

        if(unknowm){
            continue;
        }

        if(it.second.str_domain.empty() || it.second.str_append_or_replace.empty()){
            continue;
        }

        string strDomain;
        if(it.second.str_port.empty()){
            strDomain = strDomainType + "   " + strMember + "   " + strAppendOrReplace;
        }
        else{
            strDomain = strDomainType + "   " + strMember + "   " + strAppendOrReplace + "  " + strPort;
        }

        of << strDomain << endl;
    }
    of.close();
}

void RaltDomain::GetDomain(const string &strDomain, const string &strTransDomain,
    map<string, DomainValue> &domainMap)
{
    ParseDomainConf();
    unsigned int search_type = 0; //默认查询所有
    if( !strDomain.empty() && !strTransDomain.empty()){
        //域名与域名替换串共同模糊查询
        search_type = 1;
    }
    else if(!strDomain.empty() && strTransDomain.empty()){
        //只根据域名模糊查询
        search_type = 2;
    }
    else if(strDomain.empty() && !strTransDomain.empty()){
        //只根据域名替换串模糊查询
        search_type = 3;
    }

    for(const auto it:  m_DomainMap){
        if(it.second.type == RALT_MEMBER_DOMAIN){      //只查询会员，不查询字符串替换

            switch(search_type){
                case 1:
                    {
                        if(it.first.find(strDomain) != string::npos
                            && it.second.str_append_or_replace.find(strTransDomain)!= string::npos){

                            domainMap[it.first] = m_DomainMap[it.first];
                        }
                    }
                    break;
                case 2:
                    {
                        if(it.first.find(strDomain) != string::npos){
                            domainMap[it.first] = m_DomainMap[it.first];
                        }
                    }
                    break;
                case 3:
                    {
                        if(it.second.str_append_or_replace.find(strTransDomain) != string::npos){
                            domainMap[it.first] = m_DomainMap[it.second.str_domain];
                        }
                    }
                    break;
                default:
                    domainMap[it.first] = m_DomainMap[it.second.str_domain];
                    break;
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
    switch(domain->type){
        case RALT_MEMBER_DOMAIN:
            strType = "ralt_member_domain";
            break;
        case RALT_SUBS_DOMAIN_4TO6:
            strType = "ralt_subs_domain_4to6";
            break;
        case RALT_SUBS_DOMAIN_6TO4:
            strType = "ralt_subs_domain_6to4";
            break;
        default:
            strType = "ralt_subs_domain";
            break;
    }
    
    string strDomainMember = strType + string(" ") + domain->str_domain + string("  ") 
        + domain->str_append_or_replace + " " + domain->str_port;
    
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

