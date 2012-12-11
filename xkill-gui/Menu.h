#pragma once

#include <QtGui>
#include <QKeyEvent> // needed to grab mouse input

class Menu : public QDialog
{
private:
	bool show;

public:
	Menu(QWidget* parent = 0, Qt::WFlags flags = 0) : QDialog(parent, flags)
	{
		QWidget::setAttribute(Qt::WA_ShowWithoutActivating);
		//QWidget::setWindowFlags(Qt::ToolTip);

		// Create layout
		QVBoxLayout* layout = new QVBoxLayout(this);
		for (int i=0; i<10; i++)
		{
			QPushButton* buttons = new QPushButton(tr("Button %1").arg(i + 1));
			layout->addWidget(buttons);
		}
		QWidget::setLayout(layout);

		show = true;
		toggleMenu();
	}

	void onUpdate(float delta)
	{
	}

	void toggleMenu()
	{
		show = !show;
		if(show)
			QWidget::show();
		else
			QWidget::hide();
	}

	void onEvent(Event* e)
	{
	}

protected:
	// Behavior on keyboard input
	void keyPressEvent(QKeyEvent* e)
	{
		switch (e->key()) 
		{
		case Qt::Key_Tab:
			// Quit menu
			QWidget::hide();
			break;
		default:
			break;
		}
	}
};
