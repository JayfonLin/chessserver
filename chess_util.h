#ifndef CHESS_UTIL_H
#define CHESS_UTIL_H

/*
Created on 2015-09-03

@author:jeff
*/

#include "constant.h"
#include "define.h"

class CChessUtil
{
public:
	static bool InBoard(int piece_position){
		return PIECE_IN_BOARD[piece_position];
	}

	static bool InFort(int piece_position){
		return PIECE_IN_FORT[piece_position];
	}

	static bool RankY(int piece_position){
		return piece_position >> 4;
	}

	static bool FileX(int piece_position){
		return piece_position & 0xf;
	}

	static bool CoordXY(int x, int y){
		return x + (y << 4);
	}

	static int SquareFlip(int piece_position){
		return 0xfe - piece_position;
	}

	static int FileFlip(int x){
		return 0xe - x;
	}

	static int RankFilp(int y){
		return 0xf - y;
	}

	static int MirrorSquare(int piece_position){
		return CoordXY(FileFlip(FileX(piece_position)), RankY(piece_position));
	}

	static int SquareForward(int piece_position, int side){
		return piece_position - 0x10 + (side << 5);
	}

	static bool KingSpan(int piece_src, int piece_dst){
		return PIECE_LEGAL_SPAN[piece_dst - piece_src + 0x100] == 1;
	}

	static bool BodyguardSpan(int piece_src, int piece_dst){
		return PIECE_LEGAL_SPAN[piece_dst - piece_src + 0x100] == 2;
	}

	static bool BishopSpan(int piece_src, int piece_dst){
		return PIECE_LEGAL_SPAN[piece_dst - piece_src + 0x100] == 3;
	}

	static bool BishopPin(int piece_src, int piece_dst){
		return (piece_src + piece_dst)  >> 1;
	}

	static int HorsePin(int piece_src, int piece_dst){
		return piece_src + HORSE_PIN[piece_dst - piece_src + 0x100];
	}

	static bool HomeHalf(int piece_src, int side){
		return (piece_src & 0x80) != (side << 7);
	}

	static bool AwayHalf(int piece_src, int side){
		return (piece_src & 0x80) == (side << 7);
	}

	static bool SameHalf(int piece_src, int piece_dst){
		return ((piece_src ^ piece_dst) & 0x80) == 0;
	}

	static bool SameSide(int type_a, int type_b){
		return IsRed(type_a) && IsRed(type_b) || IsBlack(type_a) && IsBlack(type_b);
	}
	static bool SameRank(int piece_src, int piece_dst){
		return ((piece_src ^ piece_dst) & 0xf0) == 0;
	}

	static bool SameFile(int piece_src, int piece_dst){
		return ((piece_src ^ piece_dst) & 0x0f) == 0;
	}

	static int SideTag(int side){
		return 8 + (side << 3);
	}

	static int OppSideTag(int side){
		return 16 - (side << 3);
	}

	static int Src(int mv){
		return mv & 0xff;
	}

	static int Dst(int mv){
		return mv >> 8;
	}

	static int Move(int piece_src, int piece_dst){
		return piece_src + piece_dst * 256;
	}

	static int MirrorMove(int mv){
		return Move(MirrorSquare(Src(mv)), MirrorSquare(Dst(mv)));
	}

	static bool IsRed(int type){
		return type != 0 && (type >> 4) == 0;
	}

	static bool IsBlack(int type){
		return type != 0 && (type >> 4) == 1;
	}
};
#endif