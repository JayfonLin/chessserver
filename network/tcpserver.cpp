
/*
Created on 2015-10-02

@author: jeff
*/

#include "tcpserver.h"
#include <cstdio>

#define LISTEN_PORT 9999
#define LISTEN_BACKLOG 32

#include "../util/log.h"
#include "../util/commons.h"


int TCPServerInit(){
	evutil_socket_t listener;
    listener = socket(AF_INET, SOCK_STREAM, 0);
    assert(listener > 0);
    evutil_make_listen_socket_reuseable(listener);

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = 0;
    sin.sin_port = htons(LISTEN_PORT);

    if (bind(listener, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(listener, LISTEN_BACKLOG) < 0) {
        perror("listen");
        return 1;
    }

    printf("Listening...");

    evutil_make_socket_nonblocking(listener);

    struct event_base *base = event_base_new();
    assert(base != NULL);
    struct event *listen_event;
    listen_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);
    event_add(listen_event, NULL);

    pid_t pid;
    pid = fork();
    if (pid == 0){
        CLog::GetInstance()->start();
    	event_base_dispatch(base);
    	Log("The End.");
    	return 0;
	}else{
    	return 0;
	}
}


void do_accept(evutil_socket_t listener, short event, void *arg)
{
    struct event_base *base = (struct event_base *)arg;
    evutil_socket_t fd;
    struct sockaddr_in sin;
    socklen_t slen = sizeof(sin);
    fd = accept(listener, (struct sockaddr *)&sin, &slen);
    if (fd < 0) {
        perror("accept");
        return;
    }
    if (fd > FD_SETSIZE) { //这个if是参考了那个ROT13的例子，貌似是官方的疏漏，从select-based例子里抄过来忘了改
        perror("fd > FD_SETSIZE\n");
        return;
    }

    Log("ACCEPT: fd = %u", fd);

    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, read_cb, write_cb, error_cb, arg);
    bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);
}

void read_cb(struct bufferevent *bev, void *arg)
{
    Log("read_cb\n");
    CBinUnpacker *pack = new CBinUnpacker(bev);
    OnPackage(bev, pack);
}


void write_cb(struct bufferevent *bev, void *arg) {
	/*
    CBinPacker pack;
    pack.pack_byte(103);
    pack.pack_dword(0x01020304);
    //char str[] = "hello apple nooodles abcdefghijklmnopqrstuvwxyz\0";
    char str[] = "中文\0";

    pack.pack_string(str);
    evbuffer* buf = pack.get_pack_buffer();

    evutil_socket_t fd = bufferevent_getfd(bev);
    int result = evbuffer_write(buf, fd);
    if (result <= 0){
        Log("send failed!");
    }
    */
}

void error_cb(struct bufferevent *bev, short event, void *arg)
{
    evutil_socket_t fd = bufferevent_getfd(bev);
    Log("fd = %u, ", fd);
    if (event & BEV_EVENT_TIMEOUT) {
        Log("Timed out"); //if bufferevent_set_timeouts() called
    }
    else if (event & BEV_EVENT_EOF) {
        Log("connection closed");
    }
    else if (event & BEV_EVENT_ERROR) {
        Log("some other error");
    }
    bufferevent_free(bev);
}
