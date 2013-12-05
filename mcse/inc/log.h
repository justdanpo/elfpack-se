#ifndef _LOG_H
#define _LOG_H

#ifndef LOG
#define StartLog(...)
#define WriteLog(...)
#else
void StartLog(void);
void WriteLog(char *buf);
void WriteLog(wchar_t *buf);
void WriteLog(char *buf1, char *buf2);
void WriteLog(int buf);
#endif

#endif
