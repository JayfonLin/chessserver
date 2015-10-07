
#ifndef PACKAGE_H
#define PACKAGE_H

/*
Created on 2015-10-02

@author: jeff
*/

#include "../define.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/buffer.h>

const int MEMORY_BLOCK_SIZE = 512;
const int DWORD_SIZE = 4;

struct MemoryBlock
{
    char* buf[MEMORY_BLOCK_SIZE];
    size_t off;
    struct MemoryBlock* next;

    MemoryBlock(){
        off = 0;
        next = NULL;
    }
};

class CBinUnpacker
{
private:
	struct bufferevent *bev;
    char *line;
    int offset;

    DWORD ReadDWord(char *data);

public:
	CBinUnpacker(struct bufferevent *bev);
	BYTE UnpackByte();
	WORD UnpackWord();
	DWORD UnpackDWord();
	STRING UnpackString();

};

class CBinPacker
{
private:
	struct evbuffer *buf;

public:
	CBinPacker();
	void WriteBuf(char* data, size_t len);
	void PackByte(BYTE b);
	void PackWord(WORD w);
	void PackDWord(DWORD d);
	void PackString(STRING str);
	struct evbuffer* GetPackBuffer();
	virtual ~CBinPacker();
};

#endif
