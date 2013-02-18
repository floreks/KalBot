#pragma once
#include "stdafx.h"


#define maxHPptr 0x008BD944
#define maxMPptr 0x008BD948
#define curMPptr 0x008BD940

enum Color
{
	orange = 16594,
	lightblue = 15073034,
	violett = 12615808,
	green = 32768,
	pink = 16751615,
	blue = 15453831,
	red = 255
};

struct Monster
{
	DWORD id;
	WORD classe;
	DWORD x;
	DWORD y;
	WORD z;
	WORD HP;
};

struct Player
{
	DWORD id;
	char name[16];
	BYTE classe;
	DWORD x;
	DWORD y;
	WORD z;
};

enum SPacket
{
	login = 0x02,
	connectp = 0x08,
	checkVer = 0x09,
	loadPlayer = 0x0a,
	secondPass = 0x75,
	normalAttack = 0x0c,
	skillAttack = 0x0d,
	chat = 0x0e,
	relog = 0x10,
	move = 0x11,
	moveStop = 0x12,
	dropItem = 0x17,
	quitGame = 0x18,
	pickItem = 0x1d,
	revive = 0x22,
};

enum RPacket
{
	mobAppear = 0x33,
	mobDisappear = 0x38,
	playerAppear = 0x32,
	charInfo = 0x11,
	welcome = 0x2a,
	playerMove = 0x22,
	playerStop = 0x23,
	mobMove = 0x24,
	mobStop = 0x25,
	playerStat = 0x42,
	stateChange = 0x45,
	mobAnimation = 0x3e
};
