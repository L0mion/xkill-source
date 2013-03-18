#include "ScoreComponent.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/Timer.h>
#include <xkill-utilities/Converter.h>

ATTRIBUTES_DECLARE_ALL

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

ScoreComponent::ScoreComponent()
{
	ATTRIBUTES_INIT_ALL

	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);
	SUBSCRIBE_TO_EVENT(this, EVENT_END_DEATHMATCH);

	executingPlayerIndex_ = -1;
	schedulerTimer_ = nullptr;
	cycleTimer_ = nullptr;
	gameTimer_ = nullptr;
	nullProcessExecuting_ = false;
	executionMode_ = false;
}

ScoreComponent::~ScoreComponent()
{
	SAFE_DELETE(schedulerTimer_);
	SAFE_DELETE(cycleTimer_);
	SAFE_DELETE(gameTimer_);
}

bool ScoreComponent::init()
{
	SAFE_DELETE(schedulerTimer_);
	SAFE_DELETE(cycleTimer_);
	SAFE_DELETE(gameTimer_);

	schedulerTimer_ = new Timer(SETTINGS->schedulerTime);
	cycleTimer_ = new Timer(SETTINGS->cycleTime);

	gameTimer_ = new Timer(SETTINGS->timeLimit);

	if(SETTINGS->timeLimit < 0.001f)
	{
		gameTimer_->setActive(false);
		gameTimer_->setStartTime(1.0f);
		gameTimer_->resetTimer();
	}

	executionMode_ = false;
	nullProcessExecuting_ = false;
	executingPlayerIndex_ = -1;

	return true;
}

void ScoreComponent::onEvent(Event* e)
{
	switch(e->getType())
	{
	case EVENT_START_DEATHMATCH:
		init();
		break;
	case EVENT_END_DEATHMATCH:
		if(executionMode_)
		{
			SEND_EVENT(&Event(EVENT_PLAYER_DONE_EXECUTING));
		}
		else if(nullProcessExecuting_)
		{
			SEND_EVENT(&Event(EVENT_NULL_PROCESS_STOPPED_EXECUTING));
		}
		break;
	default:
		break;
	}
}

void ScoreComponent::onUpdate(float delta)
{
	if(GET_STATE() == STATE_DEATHMATCH)
	{
		//if scheduler game mode
		schedulerScoreCounting(delta);

		//if death match game mode
		//deathMatchScoreCounting(delta);

		gameTimer_->update(delta);
		SETTINGS->timeLeft = gameTimer_->getTimeLeft();
		if(gameTimer_->hasTimerExpired())
		{
			SEND_EVENT(&Event(EVENT_GAMEOVER));
		}
	}
}

void ScoreComponent::schedulerScoreCounting(float delta)
{
	if(executionMode_)
	{
		cycleTimer_->update(delta);
		if(cycleTimer_->hasTimerExpired())
		{
			cycleTimer_->resetTimer();

			handleExecutionMode(delta);
		}
	}
	else
	{
		handleSchedulerMode(delta);
	}

	while(itrPlayer.hasNext())
	{
		if(itrPlayer.getNext()->cycles >= SETTINGS->cycleLimit)
		{
			SEND_EVENT(&Event(EVENT_GAMEOVER));
			break;
		}
	}
}

void ScoreComponent::deathMatchScoreCounting(float delta)
{
	while(itrPlayer.hasNext())
	{
		if(itrPlayer.getNext()->priority >= SETTINGS->cycleLimit)
		{
			SEND_EVENT(&Event(EVENT_GAMEOVER));
			break;
		}
	}
}

void ScoreComponent::handleExecutionMode(float delta)
{
	if(executingPlayerIndex_ == -1)	// Shouldn't happen, but if it does then leave execution mode
	{
		DEBUGPRINT("Score component: Executing player doesn't exist.");
		executionMode_ = false;
	}
	else
	{
		AttributePtr<Attribute_Player> executingPlayer = itrPlayer.at(executingPlayerIndex_);

		if(priorityWhenSelectedForExecution > 0) // The player still has some priority so give it execution time
		{
			priorityWhenSelectedForExecution--;
			executingPlayer->priority--;
			executingPlayer->cycles++;

			{Event_PostHudMessage e("", executingPlayer); e.setHtmlMessage("", "priority to cycles", "", "-1 priority +1 cycle"); SEND_EVENT(&e);}
		}
		else								// The player doesn't have any priority left so leave execution mode
		{
			AttributePtr<Attribute_Player> player = itrPlayer.at(executingPlayerIndex_);
			player->executing = false;

			SEND_EVENT(&Event_StopSound(XKILL_Enums::Sound::SOUND_LASER, itrPlayer.ownerIdAt(executingPlayerIndex_)));

			executionMode_ = false;
			executingPlayerIndex_ = -1;
			schedulerTimer_->resetTimer();

			// Send event to notify other components that we're leaving execution mode
			SEND_EVENT(&Event(EVENT_PLAYER_DONE_EXECUTING));
		}
	}
}

void ScoreComponent::handleSchedulerMode(float delta)
{
	int topPlayerIndex = -1;
	int topPriority = 0;
	bool topPriorityIsTied = false;
	int nrOfPlayersAlive = 0;
	int lastManStanding = 0;

	while(itrPlayer.hasNext())	// Loop through all players and find if anyone has top priority and if they are alive
	{
		AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();

		if(ptr_player->priority > topPriority)		// Current player had higher priority than last top player
		{
			topPlayerIndex = itrPlayer.storageIndex();
			topPriority = ptr_player->priority;
			topPriorityIsTied = false;
		}
		else if(ptr_player->priority == topPriority)	// Current player had the same priority as last top player
		{
			topPriorityIsTied = true;
		}

		if(!ptr_player->detectedAsDead && ptr_player->ptr_health->health > 0.0f)
		{
			nrOfPlayersAlive++;
			lastManStanding = itrPlayer.storageIndex();
		}
	}

	if(!nullProcessExecuting_)
	{
		schedulerTimer_->update(delta);
		SETTINGS->timeUntilScheduling = schedulerTimer_->getTimeLeft();
		if(schedulerTimer_->hasTimerExpired())
		{
			if(topPlayerIndex == -1)	// All players had zero priority
			{
				{Event_PostHudMessage e("No players had any priority"); e.receiver = Event_PostHudMessage::RECEIVER_ALL;  e.setStyle(Event_PostHudMessage::STYLE_SUBTILE); SEND_EVENT(&e);}
				
				activateNullProcess();
			}
			else if(topPriorityIsTied)	// Two or more players are tied for the ammount of priority
			{
				{Event_PostHudMessage e("Two players had tied priority"); e.receiver = Event_PostHudMessage::RECEIVER_ALL;  e.setStyle(Event_PostHudMessage::STYLE_SUBTILE); SEND_EVENT(&e);}
				
				activateNullProcess();
			}
			else						// Execute the player with highest priority
			{
				executePlayer(topPlayerIndex);
			}
		}
	}

	if(nullProcessExecuting_)
	{
		if(nrOfPlayersAlive == 1 && SETTINGS->numPlayers !=1)
		{
			deactivateNullProcess();

			AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(lastManStanding);
			int priorityReward = itrPlayer.count();
			{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("You ", "survived", " the null process execution.", "+" + Converter::IntToStr(priorityReward) + " priority"); SEND_EVENT(&e);}
			ptr_player->priority += priorityReward;

			SEND_EVENT(&Event_SpawnPlayer(lastManStanding));
		}
		else if(nrOfPlayersAlive <= 0)
		{
			deactivateNullProcess();
		}
	}
}

void ScoreComponent::activateNullProcess()
{
	schedulerTimer_->resetTimer();
	nullProcessExecuting_ = true;

	SEND_EVENT(&Event(EVENT_NULL_PROCESS_STARTED_EXECUTING));
	SEND_EVENT(&Event_PlaySound(XKILL_Enums::Sound::SOUND_RUMBLE));
	{Event_PostHudMessage e("");  e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setHtmlMessage("","NullProcess", "is executing"); SEND_EVENT(&e);}
				
	// Post hud message
	//{Event_PostHudMessage e("<p align='center'><span style='font-size:15pt;'>NullProcess is executing</span><br><span style='color: rgba(255, 0, 0, 255); font-size:35pt;'>Punish them all</span></p>"); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setStyle(Event_PostHudMessage::STYLE_SUBTILE); SEND_EVENT(&e);}
	//{Event_PostHudMessage e("Punish them all"); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setStyle(Event_PostHudMessage::STYLE_WARNING); SEND_EVENT(&e);}
}

void ScoreComponent::deactivateNullProcess()
{
	nullProcessExecuting_ = false;
	SEND_EVENT(&Event(EVENT_NULL_PROCESS_STOPPED_EXECUTING));
	SEND_EVENT(&Event_StopSound(XKILL_Enums::Sound::SOUND_RUMBLE));
}

void ScoreComponent::executePlayer(int playerIndex)
{
	executingPlayerIndex_ = playerIndex;
	cycleTimer_->resetTimer();
	executionMode_ = true;

	AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(executingPlayerIndex_);
	ptr_player->executing = true;
	priorityWhenSelectedForExecution = ptr_player->priority;
	DEBUGPRINT("Player with attribute index " << executingPlayerIndex_ << " is executing. Beware of his laserous eyes");

	// Send event to notify other components that we're entering execution mode
	SEND_EVENT(&Event_PlayerExecuting(executingPlayerIndex_));
	SEND_EVENT(&Event_PlaySound(XKILL_Enums::Sound::SOUND_LASER, itrPlayer.ownerIdAt(playerIndex)));

	// Post hud messages
	{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Chosen by Scheduler"); SEND_EVENT(&e);}
	{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Now running in", "Kernel Mode"); SEND_EVENT(&e);}
	{Event_PostHudMessage e("", ptr_player); e.setColor(ptr_player->avatarColor); e.setHtmlMessage("", ptr_player->avatarName, "is executing"); e.receiver = Event_PostHudMessage::RECEIVER_ALL_BUT_SUBJECT; SEND_EVENT(&e);}
}
