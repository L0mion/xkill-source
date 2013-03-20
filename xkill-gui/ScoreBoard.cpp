#include "ScoreBoard.h"
#include <xkill-utilities/Converter.h>

int ScoreBoard::valueAt( int index )
{
	return entries.at(index).ptr_player->cycles;
}

void ScoreBoard::bubbleSort()
{
	int numEntries = entries.size();
	bool swapped = true;

	int j = 0;
	while(swapped) 
	{
		swapped = false;
		j++;
		for(int i=0; i<numEntries-j; i++)
		{
			if(valueAt(i) < valueAt(i + 1))
			{
				// Swap values
				AttributePtr<Attribute_Player> tmp = entries.at(i).ptr_player;
				entries.at(i).ptr_player = entries.at(i+1).ptr_player;
				entries.at(i+1).ptr_player = tmp;

				swapped = true;
			}
		}
	}
}

void ScoreBoard::quickSort( int index_start, int index_end )
{
	int left = index_start;
	int right = index_end;

	// Pick pivot (should be random for best O(n))
	int pivot = valueAt(index_end);

	// Repeat until ends meet
	while(left < right) 
	{
		// Find lower and upper values to switch
		while(valueAt(left) < pivot)
		{
			left++;
		}
		while(valueAt(right) > pivot)
		{
			right--;
		}

		// Check if chosen values are still valid
		if(left < right) 
		{
			// Swap values
			AttributePtr<Attribute_Player> tmp = entries.at(left).ptr_player;
			entries.at(left).ptr_player = entries.at(right).ptr_player;
			entries.at(right).ptr_player = tmp;

			left++;
			right--;
		}
	}

	// Continue sorting left and right
	if(index_start < right)
		quickSort(index_start, right);
	if(left < index_end)
		quickSort(left, index_end);
}

void ScoreBoard::sortPlayers()
{
	// Sort using bubblesort
	bubbleSort();

	//// Sort using quicksort
	//quickSort(0, entries.size() - 1);
}

void ScoreBoard::syncLabelsWithPlayers()
{
	for(int i=0; i<entries.size(); i++)
	{
		ScoreboardEntry* e = &entries.at(i);

		// Update if a player has
		// started/stopped executing to
		// apply change in color 
		if(e->ptr_player->executing != e->wasExecuting)
		{
			e->isChanged = true;
			e->wasExecuting = e->ptr_player->executing;
		}

		// Detect if label has changed
		if(e->ptr_player->avatarName != e->playerName)
		{
			e->isChanged = true;
			e->playerName = e->ptr_player->avatarName;
		}
		if(e->ptr_player->cycles != e->cycles)
		{
			e->isChanged = true;
			e->cycles = e->ptr_player->cycles;
		}
		if(e->ptr_player->priority != e->priority)
		{
			e->isChanged = true;
			e->priority = e->ptr_player->priority;
		}

		// Update label
		if(e->isChanged)
		{
			e->isChanged = false;

			// Set text
			e->label_process->setText(e->ptr_player->avatarName.c_str());
			e->label_cycles->setNum(e->ptr_player->cycles);
			e->label_priority->setNum(e->ptr_player->priority);

			// Empty style sheets
			std::string sheet_process = "font-weight: bold;";
			std::string sheet_cycles = "";
			std::string sheet_priority = "";

			// Add name color
			Float3 color = e->ptr_player->avatarColor;
			std::string str_color = "color: rgba("+Converter::IntToStr((int)(color.x * 255))+", "+Converter::IntToStr((int)(color.y * 255))+", "+ Converter::IntToStr((int)(color.z * 255)) +", 220);";
			sheet_process += str_color;
	

			// Apply extra stuff if we're at the current player
			if(e->ptr_player == ptr_current_player)
			{
				int brightness = 240;
				std::string str_color = "color: rgba("+Converter::IntToStr((int)(color.x * brightness))+", "+Converter::IntToStr((int)(color.y * brightness))+", "+ Converter::IntToStr((int)(color.z * brightness)) +", 220);";
				std::string sheet ="background-color: rgba(240, 240, 230, 180);";
				sheet_process += sheet+str_color;
				sheet_cycles += sheet;
				sheet_priority += sheet;
			}

			// Apply extra stuff if we have most cycles
			if(e->ptr_player->cycles == maxCycles && maxCycles > 0)
			{
				sheet_cycles += "background-color: rgba(0, 255, 0, 100);";
			}

			// Apply extra stuff if we have most priority
			if(e->ptr_player->priority == maxPriority  && maxPriority > 0)
			{
				sheet_priority += "background-color: rgba(0, 255, 0, 100);";
			}

			// Apply extra stuff if we are executing
			if(e->ptr_player->executing )
			{
				sheet_process += "background-color: rgba(255, 0, 0, 50);";
			}

			// Apply style sheet
			e->label_process->setStyleSheet(sheet_process.c_str());
			e->label_cycles->setStyleSheet(sheet_cycles.c_str());
			e->label_priority->setStyleSheet(sheet_priority.c_str());


			// Resize scoreboard to fit long
			// player names if needed
			const int kMinLabelSize = 150;
			int labelSize = e->label_process->sizeHint().width();
			if(labelSize < kMinLabelSize)
				labelSize = kMinLabelSize;
			if(labelSize > maxLabelSize)
			{
				maxLabelSize = labelSize;

				const int kPadding = 75;
				const int kColumnWidth = 100;
				int new_scoreboardWidth = kPadding;
				int a = e->label_process->sizeHint().width();
				a = e->label_process->width();

				new_scoreboardWidth += labelSize;
				new_scoreboardWidth += kColumnWidth;
				new_scoreboardWidth += kColumnWidth;

				// Resize scoreboard
				frame_scoreboard->resize(new_scoreboardWidth, frame_scoreboard->height());

				// Reposition scoreboard to center to accommodate change in size
				QWidget* parent_scoreboard = frame_scoreboard->parentWidget();
				Float2 centerPos;
				centerPos.x = parent_scoreboard->width() * 0.5f;
				centerPos.y = parent_scoreboard->height() * 0.5f;
				float test1 = frame_scoreboard->width()* 0.5f;
				float test2 = frame_scoreboard->height()* 0.5f;
				frame_scoreboard->move(centerPos.x - frame_scoreboard->width()* 0.5f, centerPos.y - frame_scoreboard->height()* 0.5f);
			}
		}
	}
}

void ScoreBoard::init( AttributePtr<Attribute_Player> ptr_current_player, QWidget* frame_scoreboard )
{
	this->frame_scoreboard = frame_scoreboard;
	this->ptr_current_player = ptr_current_player;
}

void ScoreBoard::findMaxValues()
{
	int numEntries = entries.size();

	// Find max cycles
	maxCycles = 0;
	for(int i=0; i<numEntries; i++)
	{
		ScoreboardEntry* e = &entries.at(i);

		if(e->ptr_player->cycles > maxCycles)
			maxCycles = e->ptr_player->cycles;
	}

	// Find max priority
	maxPriority = 0;
	int maxPriority_index = 0;
	for(int i=0; i<numEntries; i++)
	{
		ScoreboardEntry* e = &entries.at(i);

		if(e->ptr_player->priority > maxPriority)
		{
			maxPriority = e->ptr_player->priority;
			maxPriority_index = i;
		}
	}

	// Find second max priority
	secondMaxPriority = 0;
	for(int i=0; i<numEntries; i++)
	{
		ScoreboardEntry* e = &entries.at(i);

		if(e->ptr_player->priority > maxPriority && maxPriority_index != i)
			secondMaxPriority = e->ptr_player->priority;
	}
}

void ScoreBoard::addEntry( ScoreboardEntry entry )
{
	entry.playerName = entry.ptr_player->avatarName;
	entry.cycles = entry.ptr_player->cycles;
	entry.priority = entry.ptr_player->priority;

	entries.push_back(entry);
}

void ScoreBoard::refresh()
{
	sortPlayers();
	findMaxValues();
	syncLabelsWithPlayers();
}
