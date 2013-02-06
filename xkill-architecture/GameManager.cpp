#include "GameManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"

GameManager::GameManager()
{
	componentManager_	= NULL;
	entityManager_		= NULL;
}
GameManager::~GameManager()
{
	SAFE_DELETE(componentManager_);
	SAFE_DELETE(entityManager_);
}

bool GameManager::init(HWND windowHandle, HWND parentWindowHandle)
{
	// Detect memory leaks
#if defined(DEBUG) || defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	entityManager_ = new EntityManager();

	//Initialize components
	componentManager_ = new ComponentManager();
	if(!componentManager_->init(windowHandle, parentWindowHandle))
	{
		SHOW_MESSAGEBOX("Component manager failed to init");
		return false;
	}

	//SEND_EVENT(&Event_StartDeathmatch(5));

	return true;
}

void GameManager::update(float delta)
{
	componentManager_->update(delta);
}