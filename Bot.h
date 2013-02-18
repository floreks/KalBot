#pragma once
class Bot
{
private:
public:
	Bot(void);
	~Bot(void);

	static WORD getCurrentHP();
	static WORD getCurrentMP();
	static WORD getMaxHP();
	static WORD getMaxMP();
};

