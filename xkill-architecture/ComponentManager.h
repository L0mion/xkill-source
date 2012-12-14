#pragma once

#include <xkill-utilities/IObserver.h>

class IOComponent;
class RenderingComponent;
class BulletPhysicsComponent;
class SoundComponent;
class CameraComponent;
class InputComponent;
enum StateType;

#include <Windows.h>

//#include <windef.h>

//typedef HWND *HWND;

//struct HWND;

//#include <WinNT.h>

//typedef void* PVOID;
//typedef PVOID HANDLE;
//typedef HANDLE HWND;

class GameComponent;
class ScoreComponent;

#include "AttributeManager.h"

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
	IOComponent*			ioComponent_;
	RenderingComponent*		render_;
	BulletPhysicsComponent*	physics_;
	SoundComponent*			sound_;
	CameraComponent*		camera_;
	InputComponent*			input_;
	GameComponent*			game_;
	ScoreComponent*			score_;
	//FiniteStateMachine		stateMachine_;
	StateType				state_TemporaryVariableUsedAsSubstituteForStateMachine;

public:
	ComponentManager();
	~ComponentManager();

	bool init(HWND windowHandle, HWND parentWindowHandle);
	void onEvent(Event* e);
	void update(float delta);
};