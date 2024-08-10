#include "MainActivity.h"
#include "Game.h"

MainActivity::MainActivity(GameParam& config)
{
	game = new Game(config);
}

MainActivity::~MainActivity()
{
    delete game;
}

void MainActivity::onCreate(void* savedState)
{
//	game->loadState();
}

void MainActivity::onStart()
{
//	game->init();
}

void MainActivity::onSurfaceCreated()
{
	if (!game->initGraphics())
	{
		anut::Engine::finish(1);
		return;
	}
	anut::Engine::window->setFlags(AWINDOW_FLAG_FULLSCREEN, AWINDOW_FLAG_FULLSCREEN);
	anut::Engine::window->vsync(false);
}

void MainActivity::onSurfaceDestroyed()
{
	game->deinitGraphics();
}

void MainActivity::onProcessApp()
{
	game->update(timer.restart());
	game->draw();
	anut::Engine::window->display();
}

int MainActivity::onInputEvent(AInputEvent* event)
{
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		anut::MotionEvent motion(event);
		if (motion.action != anut::MotionEvent::ACTION_MOVE)
		{
			game->handleTouch(motion);
			return 1;
		}
		
		int pointerCount = AMotionEvent_getPointerCount(event);
		for (int p = 0; p < pointerCount; p++)
		{
			motion.id = AMotionEvent_getPointerId(event, p);
			motion.x = AMotionEvent_getX(event, p);
			motion.y = AMotionEvent_getY(event, p);
			game->handleTouch(motion);
		}
		return 1;
	}
	return 0;
}

