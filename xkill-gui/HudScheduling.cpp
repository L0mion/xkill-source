#include "HudScheduling.h"

void HudScheduling::init( Ui::Menu_HUD* ui )
{
	window = ui->label_scheduling_background;
	subWindow = ui->label_scheduling_subbackground;
	progressbar = ui->progressBar_scheduling_progressbar;

	Float2 screenSize;
	screenSize.x = window->parentWidget()->width();
	screenSize.y = window->parentWidget()->height();


	const int kSubMargin = 10;

	// Compute sizes
	subWindow->resize(window->width() - 2*kSubMargin, subWindow->height());
	progressbar->resize(window->width() - 2*kSubMargin, progressbar->height());
	int windowHeight = kSubMargin*3 + subWindow->height() + progressbar->height();
	window->resize(window->width(), windowHeight);

	// Compute positions
	window->move(screenSize.x*0.5f - window->width()*0.5, screenSize.y - window->height() - kSubMargin*1.5f);
	subWindow->move(window->x() + kSubMargin, window->y() + kSubMargin);
	progressbar->move(subWindow->x(), subWindow->y() + subWindow->height() + kSubMargin);
	hide();
	ui->label_7->hide();
	ui->label_8->hide();
	ui->label_priority_advantage->hide();
}

void HudScheduling::hide()
{
	window->hide();
	subWindow->hide();
	progressbar->hide();
}
