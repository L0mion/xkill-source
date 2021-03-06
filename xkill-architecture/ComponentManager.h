#pragma once

#include <xkill-utilities/IObserver.h>

class RenderingComponent;
class PhysicsComponent;
class SoundComponent;
class CameraComponent;
class InputComponent;
class GameComponent;
class ScoreComponent;
class HacksComponent;
class IOComponent;
class CullingComponent;
enum StateType;

#ifndef HWND_FORWARD_DECLARATION
#define HWND_FORWARD_DECLARATION
typedef struct HWND__* HWND;
#endif

/// Is responsible for updating Components in a certain order
/** 
Some \ref COMPONENTS such as RenderComponent and PhysicsComponent need to run 
every frame while other components such as AIComponent do not need to 
run as often. Some components should run in a certain order. The InputComponent 
should run before the RenderComponent to hide latency. This should 
be the responsibility of the ComponentManager.

\ingroup ARCHITECTURE
*/

class ComponentManager : public IObserver
{
private:
	RenderingComponent*		render_;
	PhysicsComponent*		physics_;
	SoundComponent*			sound_;
	CameraComponent*		camera_;
	InputComponent*			input_;
	GameComponent*			game_;
	ScoreComponent*			score_;
	HacksComponent*			hacks_;
	IOComponent*			ioComponent_;
	CullingComponent*		cullingComponent_;

	float					gameOverDelay_;
	float					inputDeviceSearchTime_;
public:
	ComponentManager();
	~ComponentManager();

	bool init(HWND windowHandle, HWND parentWindowHandle);
	void onEvent(Event* e);
	void resetGame();
	void update(float delta);
};