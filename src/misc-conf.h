#ifndef __MISC_CONF_H_
#define __MISC_CONF_H_

#include <string>
#include <map>

using namespace std;

enum MISC_TYPE{
	RALT_FILTER_TYPE = 0,
	RALT_REFERER,
	RALT_UTF16_TO_UTF8,
};

enum MISC_VALUE{
	MISC_OFF = 0,
	MISC_ON
};

typedef struct _MiscData{
	int ralt_filter_type_default;
	int ralt_referer_default;
	int ralt_utf16_to_utf8_default;
}MiscData;

class MiscConf
{
public:
	static MiscConf* GetInstance();
	void GetMiscData(MiscData *data);
	void ModMisc(MISC_TYPE type, MISC_VALUE value);

private:
	MiscConf();
	~MiscConf();
};

#endif
