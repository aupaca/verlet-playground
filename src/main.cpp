#include "MainActivity.h"
#include "GameParam.h"
#include "vec.h"
#include <ANUT/ANUT.h>
#include <fstream>

static glm::vec4 normalRgbFromHex(int hexColor);

void android_main(android_app* state)
{
	GameParam config;
	config.primaryColor = normalRgbFromHex(0xFF0040);
	config.secondaryColor = normalRgbFromHex(0xFFFFFF);
	config.maxBallCount = 200;
	config.ballRadius = 16.f;
	
	anut::Engine andronaut(state);
	MainActivity* app = new MainActivity(config);
	int status = andronaut.run(app);
	
	delete app;
}

glm::vec4 normalRgbFromHex(int hexColor)
{
    float r = (hexColor >> 16) & 0xff;
    float g = (hexColor >> 8) & 0xff;
    float b = hexColor & 0xff;
    return {r/255.f, g/255.f, b/255.f, 1.f};
}


