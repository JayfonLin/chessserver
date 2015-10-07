
#ifndef PLAY_GAME_H
#define PLAY_GAME_H

/*
Created on 2015-10-05

@author: jeff
*/

#include "board.h"
#include "../define.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/util.h>
#include <event2/buffer.h>
#include "../network/package.h"
#include "../logic/negascout_tt_hh.h"
#include "../logic/chess_move.h"
#include "../client/proto.h"


void OnStartChessGame(int uid, struct bufferevent *bev, CBinUnpacker *unpacker);
void OnChessMove(int uid, struct  bufferevent *bev, CBinUnpacker *unpacker);
void SearchGoodMove(int uid, struct bufferevent *bev, CBoard* board);

#endif