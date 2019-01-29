#include "ralt-rule.h"
#include "util-common.h"
#include "util-log.h"
#include <string.h>
#include <fstream>


#define RALT_RULE_FILE "/opt/reyzar/can/etc/trafficserver/ralt.rule.conf"

RaltRule::RaltRule()
{
    RuleTypeInit();
}

RaltRule::~RaltRule()
{
}

void RaltRule::RuleTypeInit()
{
    m_RuleType.clear();

    //ralt.rule.conf
    m_RuleType.insert(pair<RULE_TYPE, string>(TOP_LEVEL_DOMAIN, string("top_level_domain")));
    m_RuleType.insert(pair<RULE_TYPE, string>(HEX_DOMAIN, string("hex_domain")));
    m_RuleType.insert(pair<RULE_TYPE, string>(JS_BUILDIN_VAR, string("js_buildin_var")));
    m_RuleType.insert(pair<RULE_TYPE, string>(IPV4_ADDR, string("ipv4_addr")));
    m_RuleType.insert(pair<RULE_TYPE, string>(EMAIL_ATSIGN, string("email_atsign")));
    m_RuleType.insert(pair<RULE_TYPE, string>(NO_MODIFY, string("no_modify")));
    m_RuleType.insert(pair<RULE_TYPE, string>(UNHANDLED_PROTOCOL, string("unhandled_protocol")));
    m_RuleType.insert(pair<RULE_TYPE, string>(ASSIST_CHAR, string("assist_char")));
    m_RuleType.insert(pair<RULE_TYPE, string>(SUBS_STRING, string("subs_string")));
    m_RuleType.insert(pair<RULE_TYPE, string>(RSUBS_STRING, string("rsubs_string")));
    m_RuleType.insert(pair<RULE_TYPE, string>(IPV6_ADDR, string("ipv6_addr")));
    //lsp
    m_RuleType.insert(pair<RULE_TYPE, string>(JUST_REPLACE, string("just_restore")));
    m_RuleType.insert(pair<RULE_TYPE, string>(JUST_RESTORE, string("just_replace")));     
}

RaltRule* RaltRule::GetInstance()
{
    static RaltRule instance;
    return &instance;
}

vector<int> RaltRule::GetMinElement(vector<char> seperation, string input)
{
    string format;
    vector<int> out;

    int pos_begin = 0;
    int pos_end = 0;
    unsigned int i;
    for(i = 0; i < input.size(); i++)
    {
        unsigned int j;
        for(j = 0; j < seperation.size(); j++)
        {
            if(input[i] == seperation[j] ||'\0' == input[i + 1])
            {
                if('\0' == input[i + 1])
                {
                    pos_end = i;
                }
                else
                {
                    pos_end = i - 1;
                }

                if(pos_end - pos_begin + 1 > 0)
                {
                    string tmp(input.substr(pos_begin, pos_end - pos_begin + 1));
                    out.push_back(atoi(tmp.c_str()));
                }

                pos_begin = i + 1 ;
            }
        }

    }
    return out;
}


void RaltRule::ParseRule()
{
    char buf[8192]={0};
    m_RuleMap.clear();

    string members;
    vector<char> sepration;
    sepration.push_back(',');
    sepration.push_back(' ');
    vector<string> elements;
    fstream fs(RALT_RULE_FILE, fstream::in);
    while (fs.getline(buf, 8192)) {

        if(UtilCommon::SkipSpaceLeft(buf)[0] != '['){
            continue;
        }

        RuleValue rule;

        int begin = -1;
        int argument = 0;
        int i = 0;

        while(i < 8192) {
            if(buf[i] == '\n' || buf[i] == '\0'){
                break; 
            }
            else if(buf[i] == '[') {
                if (begin == -1)
                    begin = i; 
            }
            else if(buf[i] == ']' && begin != -1) {
                argument++;

                switch(argument) {
                    case 1: {
                        string strType = string(&buf[begin + 1], i - 1 - begin);
                        for(const auto it: m_RuleType){
                            if(it.second == strType){
                                rule.type = it.first;
                            }
                        }
                        break;
                    }
                    case 2: {
                        rule.strSearch = string(&buf[begin + 1], i - 1 - begin);
                        break;
                    }
                    case 3: {
                        rule.strReplace = string(&buf[begin + 1], i - 1 - begin);
                        break;
                    }
                    case 4: {
                        rule.strAppend = string(&buf[begin + 1], i - 1 - begin);

                        if (rule.type != -1){
                            pair<RULE_TYPE, RuleValue> value(rule.type, rule);
                            m_RuleMap.insert(value);
                        }
                        break;
                    }
                    case 5: {
                        members.assign(string(&buf[begin + 1], i - 1 - begin));
                        rule.vRelateDomain = GetMinElement(sepration, members);

                        multimap<RULE_TYPE, RuleValue>::iterator it = m_RuleMap.begin();
                        for (; it != m_RuleMap.end(); it++)
                        {
                            if (it->first == rule.type && it->second.strSearch == rule.strSearch)
                            {
                                m_RuleMap.erase(it);
                                break;
                            }
                        }

                        if (rule.type != -1){
                            pair<RULE_TYPE, RuleValue> value(rule.type, rule);
                            m_RuleMap.insert(value);
                        }
                        break;
                    }
                    default:
                        break;
                }

                begin = -1;
            }

            if (argument >= 5)
                break;

            i++;
        }
        bzero(buf, 8192);
    }
    fs.close();
}

multimap<RULE_TYPE, RuleValue> RaltRule::GetRule()
{
    ParseRule();
    return m_RuleMap;
}

void RaltRule::UpdateRule(multimap<RULE_TYPE, RuleValue> ruleMap)
{
    ofstream of;
    of.open(RALT_RULE_FILE);
    of << "#nginx过滤模块规则配置文件" << endl;
    of << "#每项内容由[]括起，*代表内容为空  $代表主配置中的一般变量（例：.baidu.com.ipv6）" << endl;
    of << "#类别                          查找字串                   替换字串           追加字串          对应会员关联列表的开关" << endl;
    of << "#目前ralt遇到的常见顶级域名，如果有新的顶级域名，继续新增即可" << endl;
    for(const auto it: ruleMap){
        if(it.second.type <=0){
            continue;
        }

        map<RULE_TYPE, string>::iterator rule_type_iter= m_RuleType.find(it.second.type);
        if(rule_type_iter == m_RuleType.end()){
            continue;
        }

        string strType = string("[") + rule_type_iter->second + string("]");
        string strSearch = string("[") + it.second.strSearch + string("]");
        string strReplace = string("[") + it.second.strReplace + string("]");
        string strAppend = string("[") + it.second.strAppend + string("]");
        string strRalateDomain;
        vector<int>::iterator relate_domain_it;
        for(const auto relate_domain_it: it.second.vRelateDomain){
            if(strRalateDomain.empty()){
                strRalateDomain = to_string(relate_domain_it);
            }
            else{
                strRalateDomain = strRalateDomain + "," + to_string(relate_domain_it);
            }
        }
        if(!strRalateDomain.empty()){
            strRalateDomain = string("[") + strRalateDomain + string("]");
        }

        char buf[8192]={0};;
        if(strRalateDomain.empty()){
            snprintf(buf, sizeof(buf)-1, "%-32s  %-32s  %-32s  %s", 
                strType.c_str(), strSearch.c_str(), strReplace.c_str(), strAppend.c_str());
        }
        else{
            snprintf(buf, sizeof(buf)-1, "%-32s  %-32s  %-32s  %-8s  %s", 
                strType.c_str(), strSearch.c_str(), strReplace.c_str(), strAppend.c_str(), strRalateDomain.c_str());
        }

        of << buf << endl;
    }
    of.close();
}

