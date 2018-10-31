#include "misc-conf.h"
#include "util-common.h"
#include <string.h>
#include <fstream>

#define MISC_CONF_PATH "/opt/reyzar/can/etc/trafficserver/ralt.miscellaneous.conf"

MiscConf::MiscConf()
{
}

MiscConf::~MiscConf()
{
}

MiscConf* MiscConf::GetInstance()
{
	static MiscConf instance;
	return &instance;
}

void MiscConf::GetMiscData(MiscData *data)
{
	if(NULL == data){
		return;
	}

	memset(data, 0, sizeof(*data));

	char buf[8192]={0};
	fstream fs2(MISC_CONF_PATH, fstream::in);

	while (fs2.getline(buf, 8192)) {
		if('#' == buf[0])
			continue;
		
	    if (strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_filter_type_default", strlen("ralt_filter_type_default")) == 0) {
			const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen("ralt_filter_type_default");
			begin = UtilCommon::ToNextNotChar(begin, buf + 8191, " \t", 2);
			const char *end = UtilCommon::ToNextChar(begin, buf + 8191, ";\n \t", 4);
			if ((end - begin) > 0 && strncmp(begin, "off", end - begin) == 0)
			data->ralt_filter_type_default = 0;
			else
			data->ralt_filter_type_default = 1;
	    }
		else if (strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_referer_default", strlen("ralt_referer_default")) == 0) {
			const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen("ralt_referer_default");
			begin = UtilCommon::ToNextNotChar(begin, buf + 8191, " \t", 2);
			const char *end = UtilCommon::ToNextChar(begin, buf + 8191, ";\n \t", 4);
			if ((end - begin) > 0 && strncmp(begin, "off", end - begin) == 0)
			data->ralt_referer_default = 0;
			else
			data->ralt_referer_default = 1;
	    }
		else if (strncmp(UtilCommon::SkipSpaceLeft(buf), "ralt_utf16_to_utf8_default", strlen("ralt_utf16_to_utf8_default")) == 0) {
			const char *begin = UtilCommon::SkipSpaceLeft(buf) + strlen("ralt_utf16_to_utf8_default");
			begin = UtilCommon::ToNextNotChar(begin, buf + 8191, " \t", 2);
			const char *end = UtilCommon::ToNextChar(begin, buf + 8191, ";\n \t", 4);
			if ((end - begin) > 0 && strncmp(begin, "off", end - begin) == 0)
			data->ralt_utf16_to_utf8_default = 0;
			else
			data->ralt_utf16_to_utf8_default = 1;
	    }

		bzero(buf, 8192);
	}
	
	fs2.close();
}

void MiscConf::ModMisc(MISC_TYPE type, MISC_VALUE value)
{
	printf("type: %d, value: %d\n", type, value);
	string strFieldName, strFieldValue;
	switch(type){
		case RALT_FILTER_TYPE:
			strFieldName = "ralt_filter_type_default";
			break;
		case RALT_REFERER:
			strFieldName = "ralt_referer_default";
			break;
		case RALT_UTF16_TO_UTF8:
			strFieldName = "ralt_utf16_to_utf8_default";
			break;
		default:
			printf("errro MISC_TYPE\n");
			break;
	}

	if(MISC_ON == value){
		strFieldValue = "on";
	}
	else{
		strFieldValue = "off";
	}

	string strDelCmd = "sed -i '/"+strFieldName+"/d' " + string(MISC_CONF_PATH);
	system(strDelCmd.c_str());

	ofstream of;
	of.open(MISC_CONF_PATH, ios::app);
	of << strFieldName << "	" << strFieldValue << ";" << endl;
	of.close();
}

