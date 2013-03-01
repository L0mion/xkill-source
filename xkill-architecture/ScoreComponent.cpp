#include "ScoreComponent.h"

#include <xkill-utilities/AttributeManager.h>
#include <xkill-utilities/EventType.h>
#include <xkill-utilities/Timer.h>

ATTRIBUTES_DECLARE_ALL

#define SAFE_DELETE(x) {if(x != nullptr) delete x; x = nullptr;}

ScoreComponent::ScoreComponent()
{
	ATTRIBUTES_INIT_ALL

	SUBSCRIBE_TO_EVENT(this, EVENT_START_DEATHMATCH);

	executingPlayerIndex_ = -1;
	schedulerTimer_ = nullptr;
	cycleTimer_ = nullptr;
	gameTimer_ = nullptr;
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

	schedulerTimer_ = new Timer(10.0f);
	cycleTimer_ = new Timer(1.0f);

	gameTimer_ = new Timer(settings->timeLimit);

	if(settings->timeLimit < 0.001f)
	{
		gameTimer_->setActive(false);
		gameTimer_->setStartTime(1.0f);
		gameTimer_->resetTimer();
	}

	executionMode_ = false;
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

			if(executingPlayerIndex_ == -1)	// Shouldn't happen, but if it does then leave execution mode
			{
				DEBUGPRINT("Score component: Executing player doesn't exist.");
				executionMode_ = false;
			}
			else
			{
				AttributePtr<Attribute_Player> executingPlayer = itrPlayer.at(executingPlayerIndex_);

				if(executingPlayer->priority > 0)  // The player still has some priority so give it execution time
				{
					executingPlayer->priority--;
					executingPlayer->cycles++;
				}
				else								// The player doesn't have any priority left so leave execution mode
				{
					AttributePtr<Attribute_Player> player = itrPlayer.at(executingPlayerIndex_);
					player->executing = false;

					executionMode_ = false;
					executingPlayerIndex_ = -1;
					schedulerTimer_->resetTimer();

					// Send event to notify other components that we're leaving execution mode
					SEND_EVENT(&Event(EVENT_PLAYER_DONE_EXECUTING));
				}
			}
		}
	}
	else
	{
		schedulerTimer_->update(delta);
		if(schedulerTimer_->hasTimerExpired())
		{
			int topPlayerIndex = -1;
			int topPriority = 0;
			bool topPriorityIsTied = false;

			while(itrPlayer.hasNext())	// Loop through all player and find if anyone has top priority
			{
				//Attribute_Player* player = itrPlayer.getNext();
				AttributePtr<Attribute_Player> player = itrPlayer.getNext();

				if(player->priority > 0)
				{
					if(player->priority > topPriority)		// Current player had higher priority than last top player
					{
						topPlayerIndex = itrPlayer.storageIndex();
						topPriority = player->priority;
						topPriorityIsTied = false;
					}
					else if(player->priority == topPriority)	// Current player had the same priority as last top player
					{
						topPriorityIsTied = true;
					}
				}
			}

			if(topPlayerIndex == -1)	// All players had zero priority
			{
				// Punish them all
				schedulerTimer_->resetTimer();
				
				// Post hud message
				//{Event_PostHudMessage e("<p align='center'><span style='font-size:15pt;'>NullProcess is executing</span><br><span style='color: rgba(255, 0, 0, 255); font-size:35pt;'>Punish them all</span></p>"); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setStyle(Event_PostHudMessage::STYLE_SUBTILE); SEND_EVENT(&e);}
				{Event_PostHudMessage e("Punish them all"); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setStyle(Event_PostHudMessage::STYLE_WARNING); SEND_EVENT(&e);}
				{Event_PostHudMessage e("NullProcess is executing"); e.receiver = Event_PostHudMessage::RECEIVER_ALL; e.setStyle(Event_PostHudMessage::STYLE_SUBTILE); SEND_EVENT(&e);}
			}
			else if(topPriorityIsTied)	// Two or more players are tied for the ammount of priority
			{
				// Do nothing, wait until a single player is in the lead

				// Post hud message
				{Event_PostHudMessage e("Two players have tied priority"); e.receiver = Event_PostHudMessage::RECEIVER_ALL;  e.setStyle(Event_PostHudMessage::STYLE_SUBTILE); SEND_EVENT(&e);}
			}
			else						// Execute the player with highest priority
			{
				executingPlayerIndex_ = topPlayerIndex;
				cycleTimer_->resetTimer();
				executionMode_ = true;

				AttributePtr<Attribute_Player> ptr_player = itrPlayer.at(executingPlayerIndex_);
				ptr_player->executing = true;
				DEBUGPRINT("Player with attribute index " << executingPlayerIndex_ << " is executing. Beware of his laserous eyes");


				// Send event to notify other components that we're entering execution mode
				SEND_EVENT(&Event_PlayerExecuting(executingPlayerIndex_));

				// Post hud messages
				{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Now running in", "Kernel Mode"); SEND_EVENT(&e);}
				{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("Chosen by Scheduler"); SEND_EVENT(&e);}
				{Event_PostHudMessage e("", ptr_player); e.setHtmlMessage("", "???", "is executing"); e.receiver = Event_PostHudMessage::RECEIVER_ALL_BUT_SUBJECT; SEND_EVENT(&e);}
			}
		}
	}

	while(itrPlayer.hasNext())
	{
		if(itrPlayer.getNext()->cycles >= settings->cycleLimit)
		{
			SEND_EVENT(&Event(EVENT_GAMEOVER));
		}
	}
}

void ScoreComponent::deathMatchScoreCounting(float delta)
{
	while(itrPlayer.hasNext())
	{
		if(itrPlayer.getNext()->priority >= settings->cycleLimit)
		{
			SEND_EVENT(&Event(EVENT_GAMEOVER));
		}
	}
}
