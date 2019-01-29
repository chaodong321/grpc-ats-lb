#ifndef __RALT_RULE_H_
#define __RALT_RULE_H_

#include <string>
#include <vector>
#include <map>

using namespace std;

typedef enum _RULE_TYPE{
    TOP_LEVEL_DOMAIN=1,
    HEX_DOMAIN,
    JS_BUILDIN_VAR,
    IPV4_ADDR,
    EMAIL_ATSIGN,
    NO_MODIFY,
    UNHANDLED_PROTOCOL,
    ASSIST_CHAR,
    SUBS_STRING,
    RSUBS_STRING,
    IPV6_ADDR,
    JUST_REPLACE,
    JUST_RESTORE
}RULE_TYPE;

typedef struct _RuleValue
{
    RULE_TYPE type;
    string strSearch;
    string strReplace;
    string strAppend;
    vector<int> vRelateDomain;
}RuleValue;

class RaltRule{
public:
    static RaltRule* GetInstance();
    multimap<RULE_TYPE, RuleValue> GetRule();
    void UpdateRule(multimap<RULE_TYPE, RuleValue> ruleMap);
    vector<int> GetMinElement(vector<char> seperation, string input);

private:
    RaltRule();
    ~RaltRule();
    void RuleTypeInit();
    void ParseRule();

private:
    map<RULE_TYPE, string> m_RuleType;
    multimap<RULE_TYPE, RuleValue> m_RuleMap;
};

#endif
