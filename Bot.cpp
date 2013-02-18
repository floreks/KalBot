#include "stdafx.h"
#include "Bot.h"
#include "KalTools.h"
#include "GameStruct.h"

WORD curHP = 0;
WORD maxHP = 0;

DWORD WINAPI monitorStats(void *pParams)
{
	while(1)
	{
		if(maxHP != Bot::getMaxHP())
			KalTools::LogTextBox("New max hp: %d ",maxHP);
		if(curHP != Bot::getCurrentHP())
			KalTools::LogTextBox("Current hp: %d ",curHP);
		Sleep(10);
	}
	return 0;
}


Bot::Bot(void)
{
}


Bot::~Bot(void)
{
}

WORD Bot::getCurrentHP()
{
	curHP = KalTools::getCurHP();
	return curHP;
}

WORD Bot::getMaxHP()
{
	memcpy(&maxHP,(WORD*)maxHPptr,sizeof(WORD));
	return maxHP;
}