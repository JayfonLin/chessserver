
#ifndef HANDLER_H
#define HANDLER_H

/*
Created on 2015-10-03

@author: jeff
*/

#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/buffer.h>
#include "../network/package.h"
#include "proto.h"

#include <map>
#include "../define.h"
#include "../game/play_game.h"

void OnPackage(struct bufferevent *bev, CBinUnpacker *unpacker);
void OnTestConnection(int uid, struct bufferevent* bev, CBinUnpacker *unpacker);





#endif