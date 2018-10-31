#ifndef __UTIL_COMMON_H_
#define __UTIL_COMMON_H_

#include <string>

class UtilCommon
{
public:
	static char *SkipSpaceLeft(char *str_in);
	static const char* ToNextChar(const char *begin, const char *end, const char *chars, int nchar);
	static const char* ToNextNotChar(const char *begin, const char *end, const char *chars, int nchar);
	static bool ShellCmd(std::string &strCmd,std::string &strRet);
	static bool GetLine(std::string&strValue, std::string&strLine);
	static std::string &Trim(std::string &s);
};

#endif