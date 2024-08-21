#include "MainActivity.h"
#include "GameParam.h"
#include "vec.h"
#include <ANUT/ANUT.h>
#include <fstream>

void android_main(android_app* state)
{
	GameParam config;
	config.primaryColor = normalRgbFromHex(0xFF0040);//0xB57EDC);
	config.secondaryColor = normalRgbFromHex(0xFFFFFF);
	config.maxBallCount = 300;
	config.ropeRadius = 16.f;
	config.minBallRadius = 16.f;
	config.maxBallRadius = 32.f;
	
	anut::Engine andronaut(state);
	MainActivity* app = new MainActivity(config);
	int status = andronaut.run(app);
	
	delete app;
}

