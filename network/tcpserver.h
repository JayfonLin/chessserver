
#ifndef TCPSERVER_H
#define TCPSERVER_H

/*
Created on 2015-10-05

@author: jeff
*/

#include "../define.h"
#include "package.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/buffer.h>
#include <netinet/in.h>
#include <errno.h>
#include <assert.h>
#include <cstdio>
#include <unistd.h>
#include "../client/handler.h"

void do_accept(evutil_socket_t listener, short event, void *arg);
void read_cb(struct bufferevent *bev, void *arg);
void error_cb(struct bufferevent *bev, short event, void *arg);
void write_cb(struct bufferevent *bev, void *arg);
int TCPServerInit();

#endif