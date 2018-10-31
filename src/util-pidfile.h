#ifndef __UTIL_PID_H__
#define __UTIL_PID_H__

int PidfileCreate(const char *);
void PidfileRemove(const char *);
int PidfileTestRunning(const char *pid_filename);

#endif /* __UTIL_PID_H__ */

