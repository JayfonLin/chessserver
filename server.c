#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

#define LISTEN_PORT 9999
#define LISTEN_BACKLOG 32

#define oops(msg) { perror(msg); exit(EXIT_FAILURE); }

void read_cb(struct bufferevent *bev, void *arg) {
	#define MAX_LINE    256
    char line[MAX_LINE+1];
    int n;
    evutil_socket_t fd = bufferevent_getfd(bev);

    while (n = bufferevent_read(bev, line, MAX_LINE), n > 0) {
        line[n] = '\0';
        printf("fd=%u, read line: %s\n", fd, line);

        bufferevent_write(bev, line, n);
    }
}

void write_cb(struct bufferevent *bev, void *arg) {}

void error_cb(struct bufferevent *bev, short event, void *arg) {
    evutil_socket_t fd = bufferevent_getfd(bev);
    printf("fd = %u, ", fd);
    if (event & BEV_EVENT_TIMEOUT) {
        printf("Timed out\n"); //if bufferevent_set_timeouts() called
    }
    else if (event & BEV_EVENT_EOF) {
        printf("connection closed\n");
    }
    else if (event & BEV_EVENT_ERROR) {
        printf("some other error\n");
    }
    bufferevent_free(bev);
}

void do_accept_cb(evutil_socket_t sockfd, short event_type, void *arg){
	evutil_socket_t fd;
	struct sockaddr_in sin;
	socklen_t slen;

	fd = accept(sockfd, (struct sockaddr *)&sin, &slen);
	if (fd < 0) oops("ERROR: accept: ");
	if (fd > FD_SETSIZE) oops("ERROR: fd > FD_SETSIZE");

	printf("ACCEPT: fd = %u\n", fd);

	struct event_base *base;
	base = (struct event_base *)arg;
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(bev, read_cb, NULL, error_cb, arg);
	bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);
}

int main(int argc, char **argv){
	evutil_socket_t socket_fd;
	socket_fd = socket(PF_INET, SOCK_STREAM, 0);
	assert(socket_fd > 0);
	if(socket_fd == -1) oops("ERROR: socket initiate:");
	evutil_make_listen_socket_reuseable(socket_fd);

	struct sockaddr_in address;
	bzero((void *)&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("172.16.166.129");
//	address.sin_addr.s_addr = 0;
	address.sin_port =  htons(LISTEN_PORT);

	int bind_return =  bind(socket_fd, (struct sockaddr *)&address, sizeof(address));
	if(bind_return == -1) oops("ERROR: bind:");

	int listen_return = listen(socket_fd, LISTEN_BACKLOG);
	if(listen_return == -1) oops("ERROR: listen:");

	//================================================
	puts("Listening...");
    evutil_make_socket_nonblocking(socket_fd);


    struct event_base *base;
    struct event *listen_event;

    base = event_base_new();
    assert(base != NULL);
    if(base == NULL) oops("ERROR: event_base_new() :");
    listen_event = event_new(base, socket_fd, EV_READ|EV_PERSIST, do_accept_cb, (void*)base);
    event_add(listen_event, NULL); //NULL: no timeout
    event_base_dispatch(base);

    puts("The End.");
	return 0;
}