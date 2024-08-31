#include "MainActivity.h"
#include "GameParam.h"
#include "vec.h"
#include <ANUT/ANUT.h>
#include <fstream>

void android_main(android_app* state)
{
	GameParam config;
	config.primaryColor = normalRgbFromHex(0xFF0040);
	config.secondaryColor = normalRgbFromHex(0xFFFFFF);
	config.maxBallCount = 20;
	config.ballRadius = 16.f;
	
	anut::Engine andronaut(state);
	MainActivity* app = new MainActivity(config);
	int status = andronaut.run(app);
	
	delete app;
}

