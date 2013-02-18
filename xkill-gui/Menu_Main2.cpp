#include "Menu_Main2.h"



struct keyFrame
{
	Float2 pos;
	float duration;
};

Menu_Main2::Menu_Main2( QWidget* parent ) : QMainWindow()
{
	loadCustomFonts();

	this->parent = parent;
	ui.setupUi(this);
	QWidget::setWindowFlags(Qt::FramelessWindowHint);
	alwaysOnTop(true);
	QWidget::setAttribute(Qt::WA_TranslucentBackground);
	QWidget::show();

	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_MOVE);
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);
	SUBSCRIBE_TO_EVENT(this, EVENT_ENABLE_MENU);

	

	// Center background 
	ui.label_background->move(0,0);
	ui.label_openingAnimation->move(0,0);

	// Hide unused menus
	ui.frame_opening->hide();
	ui.frame_main->hide();
	ui.frame_start->hide();
	ui.frame_customize->hide();
	ui.frame_settings->hide();
	ui.frame_video->hide();
	ui.frame_audio->hide();
	ui.frame_input->hide();
	ui.frame_credits->hide();

	// Show main menu
	push_menu(ui.frame_opening);

	// Setup GUI
	connect(ui.pushButton_start, SIGNAL(clicked()),	this, SLOT(slot_menu_start()));
	connect(ui.pushButton_startgame, SIGNAL(clicked()),	this, SLOT(slot_menu_startgame()));
	connect(ui.pushButton_customize, SIGNAL(clicked()),	this, SLOT(slot_menu_customize()));
	connect(ui.pushButton_settings, SIGNAL(clicked()),	this, SLOT(slot_menu_settings()));
	connect(ui.pushButton_video, SIGNAL(clicked()),	this, SLOT(slot_menu_video()));
	connect(ui.pushButton_audio, SIGNAL(clicked()),	this, SLOT(slot_menu_audio()));
	connect(ui.pushButton_input, SIGNAL(clicked()),	this, SLOT(slot_menu_input()));
	connect(ui.pushButton_credits, SIGNAL(clicked()),	this, SLOT(slot_menu_credits()));
	connect(ui.pushButton_quit, SIGNAL(clicked()),	this, SLOT(slot_menu_quit()));

	

	connect(ui.horizontalSlider_numPlayers, SIGNAL(valueChanged(int)),	this, SLOT(setNumPlayers(int)));


	// Init values
	ui.horizontalSlider_numPlayers->setValue(SETTINGS->numPlayers);
	
	// Load opening GIF
	openingAnimation = new QMovie(this);
	openingAnimation->setCacheMode(QMovie::CacheAll);
	std::string fileName = "../../xkill-resources/xkill-gui/images/animations/menu_opening.gif"; 
	openingAnimation->setFileName(fileName.c_str());
	if(!openingAnimation->isValid()) // error checking
		SHOW_MESSAGEBOX("Could not open " + fileName + ". Either the file is missing, or \"imageformats/qgif4.dll\" is missing.");
	openingAnimation->setParent(this); // prevents memory leaks
	ui.label_openingAnimation->setMovie(openingAnimation);
	openingAnimation->start();
	SEND_EVENT(&Event_PlaySound(Event_PlaySound::SOUND_OPENING_ANIMATION));

	connect(openingAnimation, SIGNAL(frameChanged(int)),	this, SLOT(openingAnimation_frameChanged(int)));
}

void Menu_Main2::mousePressEvent( QMouseEvent *e )
{
	if(e->button() == Qt::RightButton)
	{
		pop_menu();
	}
	if(e->button() == Qt::LeftButton)
	{
		// Skip opening, if at opening (index 0)
		if(menuStack.size()==1)
			endOpening();
	}
}

void Menu_Main2::loadCustomFonts()
{
	QStringList list;
	list << "arcade_interlaced.ttf" << "arcade_rounded.ttf";
	int fontID(-1);
	bool fontWarningShown(false);
	for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator) 
	{
		QFile res(":/xkill/fonts/" + *constIterator);
		if(res.open(QIODevice::ReadOnly) == false)
		{
			if(fontWarningShown == false)
			{
				SHOW_MESSAGEBOX("Problem loading custom font");
				fontWarningShown = true;
			}
		} 
		else 
		{
			fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
			if (fontID == -1 && fontWarningShown == false)
			{
				SHOW_MESSAGEBOX("Problem loading custom font");
				fontWarningShown = true;
			}
		}
	}
}
