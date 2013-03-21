#pragma once

#include <xkill-utilities/Util.h>
#include <QtGui/QLabel>

class ScoreboardEntry
{
public:
	QLabel* label_process;
	QLabel* label_cycles;
	QLabel* label_priority;

	bool isChanged;
	bool wasExecuting;

	std::string playerName;
	int cycles;
	int priority;

	AttributePtr<Attribute_Player> ptr_player;

	ScoreboardEntry()
	{
		isChanged = true;
	}
};

class ScoreBoard
{
private:
	std::vector<ScoreboardEntry> entries;
	AttributePtr<Attribute_Player> ptr_current_player;
	QWidget* frame_scoreboard;
	int maxLabelSize;

	int valueAt(int index);
	void bubbleSort();

	void quickSort(int index_start, int index_end);
	void sortPlayers();
	void syncLabelsWithPlayers();
public:
	int maxCycles;
	int maxPriority;
	int previousMaxCycles;
	int previousTime;
	int secondMaxPriority;
	

	void init(AttributePtr<Attribute_Player> ptr_current_player, QWidget* frame_scoreboard);
	void findMaxValues();
	void addEntry(ScoreboardEntry entry);
	void refresh();
};