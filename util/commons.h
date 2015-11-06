
#ifndef COMMONS_H
#define COMMONS_H

/*
Created on 2015-11-03
@author: jeff
*/

#include <string>
using std::string;

const static int MAX_STR_LEN = 1024;

void Log(const char *fmt, ...);
void LogErr(string line);


#endif // COMMONS_H
