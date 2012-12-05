#pragma once

#include <xkill-utilities/IObserver.h>

#include <vector>

class ScoreComponent : public IObserver
{
public:
	ScoreComponent();
	~ScoreComponent();

	virtual void onEvent(Event* e);
	virtual void onUpdate(float delta);

private:


	void sort(int* elements, int nrOfElements);
	void swap(int* elements, int first, int second);
};