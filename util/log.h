
#ifndef LOG_H
#define LOG_H

/*
Created on 2015-10-16

@author: jeff
*/

#include <vector>
#include <string>
#include <cstdio>
#include <pthread.h>
#include <ctime>

using std::vector;
using std::string;

typedef struct my_struct_tag {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} my_struct_t;

const static string LOG_INFO = "INFO";
const static string LOG_ERR = "ERR";
const static string LOG_WARN = "WARN";


class CLog
{
public:
    static CLog* GetInstance();

    virtual ~CLog();
    void start();
    void run();
    void AddLog(string prefix, string line);
    void stop();
    void DumpLogs(vector<string>* write_logs);

private:
    vector<string>* logs;
    FILE* log_file;
    my_struct_t* pt;
    bool is_running;
    static CLog* instance;

    tm* log_time;

private:
    CLog();
    CLog(const char* file_path);

    void finish();
    static void* start_thread(void* arg);
    FILE* GetLogFile();
    string GetOneLine(string prefix, string line);
};

#endif


