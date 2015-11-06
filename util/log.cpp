
/*
Created on 2015-10-26
@author: jeff
*/

#include "log.h"
#include <cstdio>
#include <unistd.h>

CLog* CLog::instance = NULL;
CLog* CLog::GetInstance(){
    if (instance == NULL){
        instance = new CLog();
    }

    return instance;
}

void* CLog::start_thread(void* arg){
    CLog *ptr = (CLog*)arg;
    ptr->run();
}

CLog::CLog(){
    time_t t = time(0);
    char t_str[64];
    log_time = localtime(&t);
    strftime(t_str, sizeof(t_str), "%Y-%02m-%02d.log", log_time);
    this->log_file = fopen(t_str, "a+");
    logs = new vector<string>();
    pt = (my_struct_t*)(malloc(sizeof(my_struct_t)));
    pthread_mutex_init(&pt->mutex, NULL);
    pthread_cond_init(&pt->cond, NULL);
}

CLog::CLog(const char* path){
    this->log_file = fopen(path, "a+");
    logs = new vector<string>();
    pt = (my_struct_t*)(malloc(sizeof(my_struct_t)));
    pthread_mutex_init(&pt->mutex, NULL);
    pthread_cond_init(&pt->cond, NULL);
}

CLog::~CLog(){
    printf("~CLog\n");
    //fclose(log_file);
    finish();
}

void CLog::start(){
    int status;
    pthread_t run_thread_id;
    is_running = true;

    status = pthread_create(&run_thread_id, NULL, start_thread, (void*)this);
    if (status != 0){
        printf("Create run thread failed\n");
    }
}

void CLog::run(){
    while (is_running){
        pthread_mutex_lock(&pt->mutex);
        if (logs->empty()){
            pthread_mutex_unlock(&pt->mutex);
            sleep(3);
            //usleep(1000000);
        }
        else{
            vector<string>* write_logs = logs;
            logs = new vector<string>();

            pthread_mutex_unlock(&pt->mutex);
            DumpLogs(write_logs);
            delete write_logs;
        }
    }

    finish();
}



void CLog::DumpLogs(vector<string>* write_logs){
    printf("DumpLogs\n");
    GetLogFile();
    for (vector<string>::iterator it = write_logs->begin(); it != write_logs->end(); it++){
        fputs((*it).c_str(), this->log_file);
    }
    fflush(this->log_file);
}

string CLog::GetOneLine(string prefix, string line){
    time_t t = time(0);
    tm* now = localtime(&t);
    char t_str[64];
    sprintf(t_str, "[%02d:%02d:%02d %s]", now->tm_hour, now->tm_min, now->tm_sec, prefix.c_str());
    string s = t_str + line + "\n";
    return s;
}

void CLog::finish(){
    printf("finish\n");
    stop();
    fclose(this->log_file);
    pthread_exit(NULL);
}

void CLog::stop(){
    is_running = false;
}

void CLog::AddLog(string prefix, string line){
    printf("AddLog\n");
    pthread_mutex_lock(&pt->mutex);
    line = GetOneLine(prefix, line);
    logs->push_back(line);
    pthread_mutex_unlock(&pt->mutex);
}

FILE* CLog::GetLogFile(){
    time_t t = time(0);
    tm* cur_tm = localtime(&t);
    if (log_time->tm_year == cur_tm->tm_year &&
        log_time->tm_mon == cur_tm->tm_mon &&
        log_time->tm_mday == cur_tm->tm_mday){
        return this->log_file;
    }

    fflush(this->log_file);
    fclose(this->log_file);

    char t_str[64];
    log_time = cur_tm;
    strftime(t_str, sizeof(t_str), "%Y-%02m-%02d.log", log_time);
    this->log_file = fopen(t_str, "a+");

    return this->log_file;
}
