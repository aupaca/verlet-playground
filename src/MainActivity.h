#ifndef MAIN_ACTIVITY_H
#define MAIN_ACTIVITY_H
#include <ANUT/ANUT.h>
#include <ANUT/ANUT_utils.h>

class Game;
class GameParam;

class MainActivity : public anut::Activity
{
public:
	MainActivity(GameParam& config);
	~MainActivity();
	void onCreate(void* savedState) override;
	void onStart() override;
	void onSurfaceCreated() override;
	void onSurfaceDestroyed() override;
	void onProcessApp() override;
	int onInputEvent(AInputEvent* event) override;
	
private:
	Game* game;
	anut::Timer timer;
};

#endif
