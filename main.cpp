/*
Created on 2015-09-06

@author:jeff
*/

#include "logic/negascout_tt_hh.h"
#include "network/tcpserver.h"

#include <iostream>
#include <cstdlib>
#include <util/log.h>
#include "util/commons.h"
using namespace std;


int main()
{
	GetEngineInstance();
	TCPServerInit();

	sleep(1000);

	return 0;
}
