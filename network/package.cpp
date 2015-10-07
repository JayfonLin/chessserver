
/*
Created on 2015-10-02

@author: jeff
*/

#include "package.h"
#include <netinet/in.h>
#include <cstring>
#include <cstdio>

DWORD CBinUnpacker::ReadDWord(char *data){
	DWORD dword = ntohl(*(DWORD*)data);
    printf("CBinUnpacker::ReadDWord dword: %d\n", dword);
	return dword;
}

CBinUnpacker::CBinUnpacker(struct bufferevent *bev){

	this->bev = bev;
        
    offset = 0;
    evutil_socket_t fd = bufferevent_getfd(bev);
    char pack_len_data[DWORD_SIZE];

    int n = bufferevent_read(bev, pack_len_data, DWORD_SIZE);
    DWORD pack_len = ReadDWord(pack_len_data);

    line = new char[pack_len];
    n = bufferevent_read(bev, line, pack_len);
}

BYTE CBinUnpacker::UnpackByte(){
    BYTE b = *(BYTE*)&line[offset];
    offset += sizeof(BYTE);
    return b;
}

WORD CBinUnpacker::UnpackWord(){
    WORD w = ntohs(*(WORD*)&line[offset]);
    offset += sizeof(WORD);
    return w;
}

DWORD CBinUnpacker::UnpackDWord(){
    DWORD dword = ntohl(*(DWORD*)&line[offset]);
    offset += sizeof(DWORD);
    return dword;
}

STRING CBinUnpacker::UnpackString(){

    WORD len = UnpackWord();
    BYTE *data = new BYTE[len+1];
    memcpy(data, line+offset, len*sizeof(BYTE));
    data[len] = '\0';
    offset += len*sizeof(BYTE);
    
    return (STRING)data; 
}




CBinPacker::CBinPacker(){
	buf = evbuffer_new();
}

void CBinPacker::WriteBuf(char* data, size_t len){
	int result = evbuffer_add(buf, data, len);
    if (result != 0){
        printf("write buffer failed!");
    }
}

void CBinPacker::PackByte(BYTE b){
	WriteBuf((char*)&b, sizeof(BYTE));
}

void CBinPacker::PackWord(WORD w){
	w = htons(w);
    WriteBuf((char*)&w, sizeof(WORD));
}

void CBinPacker::PackDWord(DWORD d){
	d = htonl(d);
    WriteBuf((char*)&d, sizeof(DWORD));
}

void CBinPacker::PackString(STRING str){
	WORD len = (WORD)strlen(str);
    PackWord(len);
    printf("pack_word: %d\n", len);
    WriteBuf(str, len*sizeof(char));
}

struct evbuffer* CBinPacker::GetPackBuffer(){
	DWORD pack_len = evbuffer_get_length(buf);
    pack_len = htonl(pack_len);
    evbuffer_prepend(buf, (char*)&pack_len, sizeof(DWORD));
    return buf;
}

CBinPacker::~CBinPacker(){
	evbuffer_free(buf);
}






