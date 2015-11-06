
/*
Created on 2015-11-03

@author: jeff
*/

#include "commons.h"
#include "util/log.h"
#include <cstdio>
#include <cstdarg>

void Log(const char *fmt, ...){
    char temp[MAX_STR_LEN] = {0};
    va_list args;
    va_start(args, fmt);
    vsprintf(temp, fmt, args);
    va_end(args);
    CLog::GetInstance()->AddLog(LOG_INFO, temp);
    string p_line = temp;
    p_line += "\n";
    printf(p_line.c_str());
}

void LogErr(string line){
    CLog::GetInstance()->AddLog(LOG_ERR, line);
    string p_line = line+"\n";
    printf(p_line.c_str());
}
