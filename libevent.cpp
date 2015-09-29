#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#include <event2/event.h>
#include <event2/bufferevent.h>

#include <stdint.h>
#include <netinet/in.h>
#include <cstddef>

#define LISTEN_PORT 9999
#define LISTEN_BACKLOG 32

//1024 is too small!
#define MAX_LINE    1024 

#define MEMORY_BLOCK_SIZE 512

typedef int8_t BYTE;
typedef int16_t WORD;
typedef int32_t DWORD;
typedef signed char* STRING;

void do_accept(evutil_socket_t listener, short event, void *arg);
void read_cb(struct bufferevent *bev, void *arg);
void error_cb(struct bufferevent *bev, short event, void *arg);
void write_cb(struct bufferevent *bev, void *arg);

struct MemoryBlock
{
    char* buf[MEMORY_BLOCK_SIZE];
    size_t off;
    MemoryBlock* next;

    MemoryBlock(){
        off = 0;
        next = NULL:
    }
};

class CBinUnpacker
{
private:
    struct bufferevent *bev;
    unsigned char line[MAX_LINE];
    int offset;
public:
    CBinUnpacker(struct bufferevent *bev){
        this->bev = bev;
        
        int n;
        offset = 0;
        evutil_socket_t fd = bufferevent_getfd(bev);
        n = bufferevent_read(bev, line, MAX_LINE);
    }

    BYTE read_byte(){
        BYTE b = *(BYTE*)&line[offset];
        offset += sizeof(BYTE);
        return b;
    }

    WORD read_word(){
        WORD w = ntohs(*(WORD*)&line[offset]);
        offset += sizeof(WORD);
        return w;
    }


    DWORD read_dword(){
        DWORD dword = ntohl(*(DWORD*)&line[offset]);
        offset += sizeof(DWORD);
        return dword;
    }

    STRING read_string(){

        WORD len = read_word();
        BYTE *data = new BYTE[len+1];
        memcpy(data, line+offset, len*sizeof(BYTE));
        data[len] = '\0';
        offset += len*sizeof(BYTE);
        
        return (STRING)data; 
    }

};

class CBinPacker{
private:
    struct bufferevent *bev;
    //unsigned char line[MAX_LINE];

    MemoryBlock *head;
    MemoryBlock *tail;

public:
    CBinPacker(struct bufferevent *bev){
        this->bev = bev;

        MemoryBlock *block = new MemoryBlock();
        head = tail = block;
        offset = 0;
    }

    void write_buf(char* buffer, size_t len){
        size_t offset = 0;

        while (offset < len){

            if (tail->off == MEMORY_BLOCK_SIZE){
                MemoryBlock* block = new MemoryBlock();
                tail->next = block;
                tail = block;
            }

            int remain_space = (len - offset < MEMORY_BLOCK_SIZE) ? len - offset : MEMORY_BLOCK_SIZE - tail->off;
            memcpy(tail->buf+tail->off, buffer+offset, remain_space);
            tail->off += remain_space;
            offset += remain_space;
        }
    }

    void pack_byte(BYTE b){
        write_buf(*(char*)&b, sizeof(BYTE));
    }

    void pack_dword(DWORD d){
        d = htonl(d);
        write_buf(*(char*)&d, sizeof(DWORD));
    }

    char* get_pack_buffer(){
        
    }

    /*
    void pack_string(const STRING& str){
        size_t len = sizeof(str);
    }
    */

};

int main(int argc, char *argv[])
{
    int ret;
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

    printf ("Listening...\n");

    evutil_make_socket_nonblocking(listener);

    struct event_base *base = event_base_new();
    assert(base != NULL);
    struct event *listen_event;
    listen_event = event_new(base, listener, EV_READ|EV_PERSIST, do_accept, (void*)base);
    event_add(listen_event, NULL);
    event_base_dispatch(base);

    printf("The End.");
    return 0;
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

    printf("ACCEPT: fd = %u\n", fd);

    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, read_cb, NULL, error_cb, arg);
    bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);
}

void read_cb(struct bufferevent *bev, void *arg)
{
    CBinUnpacker pack = CBinUnpacker(bev);

    DWORD d = pack.read_dword();
    printf("read dword: %x\n", d);


    
    STRING str = pack.read_string();
    printf("read string: %s\n", str);

    str = pack.read_string();
    printf("read string: %s\n", str);

    //d = pack.read_dword();
    //printf("read dword: %x\n", d);

}


void write_cb(struct bufferevent *bev, void *arg) {}

void error_cb(struct bufferevent *bev, short event, void *arg)
{
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