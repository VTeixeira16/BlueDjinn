#ifndef GAMBLING_H
#define GAMBLING_H

#include <stdio.h>

#define GAME_TYPE_UNDEFINED     0
#define GAME_TYPE_BINGO         1
#define GAME_TYPE_SLOT          2

//Convert defines CMAKE to C
#if __GAME_TYPE__ == GAME_TYPE_UNDEFINED
#define GAME_UNDEFINED
#elif __GAME_TYPE__ == GAME_TYPE_BINGO
#define GAME_BINGO
#elif __GAME_TYPE__ == GAME_TYPE_SLOT
#define GAME_SLOT
#endif


typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#endif //GAMBLING_H
