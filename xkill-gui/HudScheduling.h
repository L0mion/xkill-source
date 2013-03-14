#pragma once

#include <xkill-utilities/Util.h>
#include <xkill-utilities/Converter.h>
#include <QtGui/QLabel>

#include "ui_Menu_HUD.h"

class HudScheduling_Player
{
public:
	QLabel* label;
	AttributePtr<Attribute_Player> ptr_player;
	int priority;
	Float2 position;

	void update();
};

class HudScheduling
{
private:
	QWidget* parent;
	QWidget* window;
	QWidget* subWindow;
	QProgressBar* progressbar;
	QLabel* advantageLabel;
	AttributePtr<Attribute_Player> ptr_player;
public:
	void init(Ui::Menu_HUD* ui, AttributePtr<Attribute_Player> ptr_player)
	{
		this->ptr_player = ptr_player;
		parent = ui->label_scheduling_background->parentWidget();
		window = ui->label_scheduling_background;
		subWindow = ui->label_scheduling_subbackground;
		progressbar = ui->progressBar_scheduling_progressbar;
		advantageLabel = ui->label_priority_advantage;

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
		advantageLabel->move(subWindow->x(), subWindow->y());
		//hide();
		ui->label_7->hide();
		ui->label_8->hide();
		ui->groupBox_bottomCenter->hide();
		//ui->label_priority_advantage->hide();
		progressbar->setMaximum(500);


		// Add players
		AttributeIterator<Attribute_Player> itrPlayer = ATTRIBUTE_MANAGER->player.getIterator();
		while(itrPlayer.hasNext())
		{
			AttributePtr<Attribute_Player> ptr_player = itrPlayer.getNext();

			QLabel* l = new QLabel(parent);
			int size = subWindow->height();
			l->resize(size, size);
			l->move(subWindow->x() + 0.5f, subWindow->y() + 0.5f);

			// Set color based on player's color
			Float3 color = ptr_player->avatarColor;
			std::string str_color = "background-color: rgba("+Converter::IntToStr((int)(color.x * 255))+", "+Converter::IntToStr((int)(color.y * 255))+", "+ Converter::IntToStr((int)(color.z * 255)) +", 150);";
			l->setStyleSheet(QString(str_color.c_str()));

			l->show();


			/*HudScheduling_Player item;
			item.label = l;
			item.priority = -1;
			item.ptr_player = ptr_player;

			item.label = 
			ptr_player->
			ptr_player->*/
		}
	}

	void hide();
	void refresh()
	{
		// Update progressbar
		const int kPrecition = 500;
		int schedulingRatio = kPrecition - (int)(SETTINGS->timeUntilScheduling / SETTINGS->schedulerTime * kPrecition);
		if(progressbar->value() != schedulingRatio)
		{
			progressbar->setValue(schedulingRatio);
			progressbar->update();
		}
	}
};