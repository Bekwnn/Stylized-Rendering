#include "STime.h"

double STime::curTime = 0.0;
double STime::deltaTime = 1.0 / 60.0;
float STime::globalTimeDilation = 1.0f;
time_t STime::lastTimer = { 0 };

void STime::InitTime()
{
	time_t timer;
	lastTimer = time(&timer);
}

void STime::UpdateDeltaTime()
{
	time_t timer;
	time(&timer);
	deltaTime = globalTimeDilation * difftime(timer, lastTimer);
	curTime += deltaTime;
	lastTimer = timer;
}
