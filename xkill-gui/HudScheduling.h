#pragma once

#include <xkill-utilities/Util.h>
#include <QtGui/QLabel>

#include "ui_Menu_HUD.h"


class HudScheduling
{
private:
	QWidget* window;
	QWidget* subWindow;
	QWidget* progressbar;
public:
	void init(Ui::Menu_HUD* ui);
	void hide();
};