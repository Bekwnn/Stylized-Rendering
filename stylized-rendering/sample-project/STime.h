#pragma once
#include <ctime>

class STime
{
public:
	static void InitTime();
	static double GetDeltaTime() { return deltaTime; }
	static double GetCurTime() { return curTime; }
	static void UpdateDeltaTime();

	static float globalTimeDilation;

private:
	static double deltaTime;
	static double curTime;
	static time_t lastTimer;
};