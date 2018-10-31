#include "util-common.h"
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char *UtilCommon::SkipSpaceLeft(char *str_in)
{
	if (str_in == NULL)
		return NULL;

	while( ( *str_in == ' ' || *str_in == '\t' )&& *str_in != '\0')
		++str_in;
	return str_in;
}

const char* UtilCommon::ToNextChar(const char *begin, const char *end, const char *chars, int nchar)
{
	const char *p = begin;
	char c = *p;
	int i;
	while (p <= end && *p!='\0') {
		for (i = 0; i < nchar; i++)
			if (c == chars[i])
				return p;
		c = *++p;
	}
	return p;
}

const char* UtilCommon::ToNextNotChar(const char *begin, const char *end, const char *chars, int nchar)
{
	const char *p = begin;
	char c = *p;
	int i;

	while (p <= end) {
		for (i = 0; i < nchar; i++)
			if (c == chars[i])
				break;
		if (i >= nchar)
			return p;
		c = *++p;
	}
	return p;
}

bool UtilCommon::ShellCmd(std::string &strCmd,std::string &strRet)
{
	bool bRet=false;    
    FILE *fpRead=popen(strCmd.c_str(),"r");
    if(fpRead!=NULL)
    {
        char chBuffer[40960];
        memset(chBuffer,0,sizeof(chBuffer));
        strRet.clear();
        while(fgets(chBuffer,sizeof(chBuffer)-1,fpRead)!=NULL)
        {
            strRet+=std::string(chBuffer);
        }
        bRet=true;
        pclose(fpRead);
    }
    return bRet;
}

bool UtilCommon::GetLine(std::string&strValue, std::string&strLine)
{
	bool bRet=false;
	int nPos=strValue.find('\n');	  
	
   if(nPos>=1)
   {
	   strLine=strValue.substr(0,nPos);//delete '\n';
	   strValue=strValue.substr(nPos+1,strValue.length());
	   bRet=true;
   }
   else
   {
	   //the last line no '\n' character:
	   if((nPos==-1)&&(strValue.length()>1))
	   {
		   strLine=strValue;
		   strValue="";
		   bRet=true;
	   }
   }	 
	return bRet;	 
}

std::string &UtilCommon::Trim(std::string &s)
{
    int i=0;
    while( (s[i]==' ')|| (s[i]=='	') || (s[i]=='\n') || (s[i]=='\r'))
    {
        i++;
    }
    s=s.substr(i);
    
    i=s.size()-1;    
    while( (s[i]==' ')|| (s[i]=='	') || (s[i]=='\n') || (s[i]=='\r'))
    {
        i--;
    }
    s=s.substr(0,i+1);    
    return s;
}


