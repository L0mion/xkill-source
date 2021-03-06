#include "Menu_Main.h"

#include <QtGui/QStandardItemModel>

struct keyFrame
{
	Float2 pos;
	float duration;
};

void Menu_Main::loadOpeningGif()
{
	// Load opening
	openingAnimation = new QMovie(this);
	openingAnimation->setCacheMode(QMovie::CacheAll);
	std::string fileName = "../../xkill-resources/xkill-gui/images/animations/menu_opening.gif";

	//// Easter egg
	//const int chance = 20;
	//if(Math::randomInt(0, chance) == 0)
	//{
	//	fileName = "../../xkill-resources/xkill-gui/images/animations/tmp.gif";
	//}
	//// Eric easter egg
	//std::string username = getenv( "USERNAME" );
	//if(username == "Eric")
	//{
	//	const int chance = 20;
	//	if(Math::randomInt(0, chance) == 0)
	//	{
	//		fileName = "../../xkill-resources/xkill-gui/images/animations/tmp2.gif";
	//	}
	//}

	openingAnimation->setFileName(fileName.c_str());
	if(!openingAnimation->isValid()) // error checking
		ERROR_MESSAGEBOX("Could not open " + fileName + ". Either the file is missing, or \"imageformats/qgif4.dll\" is missing.");
	openingAnimation->setParent(this); // prevents memory leaks
	ui.label_openingAnimation->setMovie(openingAnimation);
	openingAnimation->start();
	SEND_EVENT(&Event_PlaySound(XKILL_Enums::Sound::SOUND_OPENING_ANIMATION));
	connect(openingAnimation, SIGNAL(frameChanged(int)),	this, SLOT(openingAnimation_frameChanged(int)));

	// Load background
	QMovie* backgroundAnimation = new QMovie(this);
	backgroundAnimation->setCacheMode(QMovie::CacheAll);
	fileName = "../../xkill-resources/xkill-gui/images/animations/menu_background.gif";
	backgroundAnimation->setFileName(fileName.c_str());
	if(!backgroundAnimation->isValid()) // error checking
		ERROR_MESSAGEBOX("Could not open " + fileName + ". Either the file is missing, or \"imageformats/qgif4.dll\" is missing.");
	backgroundAnimation->setParent(this); // prevents memory leaks
	ui.label_background->setMovie(backgroundAnimation);
	backgroundAnimation->start();
}

Menu_Main::Menu_Main( QWidget* parent ) : QMainWindow()
{
	// Make overlay accessible to other widgets
	SETTINGS->overlayWidget = this;

	// Init
	loadCustomFonts();
	this->parent = parent;
	ui.setupUi(this);
	QWidget::setWindowFlags(Qt::FramelessWindowHint);
	//alwaysOnTop(true);
	QWidget::setAttribute(Qt::WA_TranslucentBackground);
	QWidget::show();
	loadOpeningGif();

	// Events
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_FOCUS_CHANGED);
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_MOVE);
	SUBSCRIBE_TO_EVENT(this, EVENT_WINDOW_RESIZE);
	SUBSCRIBE_TO_EVENT(this, EVENT_ENABLE_MENU);
	SUBSCRIBE_TO_EVENT(this, EVENT_UPDATE);


	//
	// Setup DeubBillboard
	//

	billboard.init(this, ui.label_debugMessageTemplate);


	//
	// Setup GUI
	//

	// center background 
	ui.label_background->move(0,0);
	ui.label_openingAnimation->move(0,0);

	// hide unused menus
	ui.frame_opening->hide();
	ui.frame_main->hide();
	ui.frame_start->hide();
	ui.frame_customize->hide();
	ui.frame_settings->hide();
	ui.frame_howto->hide();
	ui.frame_video->hide();
	ui.frame_audio->hide();
	ui.frame_input->hide();
	ui.frame_credits->hide();
	//ui.pushButton_video->hide();
	//ui.label_background->hide();
	show();

	// show main menu
	push_menu(ui.frame_opening);

	// signals
	connect(ui.pushButton_applyRes, SIGNAL(clicked()),	this, SLOT(applyVideoRes()));
	connect(ui.pushButton_applyShaders, SIGNAL(clicked()),	this, SLOT(applyShaders()));
	connect(ui.pushButton_start, SIGNAL(clicked()),	this, SLOT(slot_menu_start()));
	connect(ui.pushButton_startgame, SIGNAL(clicked()),	this, SLOT(slot_menu_startgame()));
	connect(ui.pushButton_customize, SIGNAL(clicked()),	this, SLOT(slot_menu_customize()));
	connect(ui.pushButton_settings, SIGNAL(clicked()),	this, SLOT(slot_menu_settings()));
	connect(ui.pushButton_howto, SIGNAL(clicked()),	this, SLOT(slot_menu_howto()));
	connect(ui.pushButton_video, SIGNAL(clicked()),	this, SLOT(slot_menu_video()));
	connect(ui.pushButton_audio, SIGNAL(clicked()),	this, SLOT(slot_menu_audio()));
	connect(ui.pushButton_input, SIGNAL(clicked()),	this, SLOT(slot_menu_input()));
	connect(ui.pushButton_editor, SIGNAL(clicked()),	this, SLOT(slot_menu_editor()));
	connect(ui.pushButton_credits, SIGNAL(clicked()),	this, SLOT(slot_menu_credits()));
	connect(ui.pushButton_quit, SIGNAL(clicked()),	this, SLOT(slot_menu_quit()));
	connect(ui.horizontalSlider_numPlayers, SIGNAL(valueChanged(int)),	this, SLOT(setNumPlayers(int)));

	connect(ui.pushButton_levelNext, SIGNAL(clicked()),	this, SLOT(slot_menu_next_level()));
	connect(ui.pushButton_levelPrevious, SIGNAL(clicked()),	this, SLOT(slot_menu_previous_level()));
	
	// standard values
	ui.horizontalSlider_numPlayers->setValue(SETTINGS->numPlayers);

	// init sub-menus
	input_Menu = new Menu_Input(&ui, this);
	input_Menu->Init(new QStandardItemModel(0, 2, this), new QStandardItemModel(0, 1, this));
	ammo_Menu = new Menu_Ammo(&ui, this);
	firingMode_Menu = new Menu_FiringMode(&ui, this);
	sound_Menu = new Menu_Sound(&ui, this);
	hud = new Menu_HUDManager(this);

	// init level menu
	filePath = QString("../../xkill-resources/xkill-scripts/levels.xml");
	levelCurrent = 0;

	Event_GetFileList* fileList = new Event_GetFileList("../../xkill-resources/xkill-level/", ".mdldesc");
	SEND_EVENT(fileList);
	std::vector<std::string> filenames = fileList->filenames;
	delete fileList;

	//editorModel->setHorizontalHeaderItem(1, new QStandardItem("ID"));

	QStringList columnNames;
	levelListModel = new QStandardItemModel(0, filenames.size(), this);
	for(unsigned int i = 0; i < filenames.size(); i++)
	{
		std::string filename = filenames[i];
		int strIndex = std::string::npos;
		strIndex = filename.find_first_of(".");

		if(strIndex != std::string::npos)
			filename = filename.substr(0, strIndex);

		QString qStr = filename.c_str();
		QStandardItem* stdItem = new QStandardItem(qStr);

		levelNames.push_back(filename);
		levelListModel->appendRow(stdItem);
	}

	// Load level
	SEND_EVENT(&Event_LoadLevel(levelNames[levelCurrent]));
	SETTINGS->currentLevel = levelNames[levelCurrent];
	updateLevelSelectionInterface();

	// Init values to standard values
	SEND_EVENT(&Event_EnableMenu(true));
}

void Menu_Main::updateLevelSelectionInterface()
{
	std::string levelName = levelNames[levelCurrent];
	std::string imagePath = "../../xkill-resources/xkill-level/" + levelName + "/" + levelName + ".png";

	ui.label_levelNameText->setText(levelName.c_str());

	ui.label_level_image->setPixmap(QPixmap(QString::fromUtf8(imagePath.c_str())));
	const QPixmap* pixmap = ui.label_level_image->pixmap();
	if(pixmap->isNull()) //If no level-specific image was found, try displaying a default image as replacement
	{
		ui.label_level_image->setPixmap(QPixmap(QString::fromUtf8("../../xkill-resources/xkill-gui/images/icons/levels/arena1.png")));
	}
}

void Menu_Main::mousePressEvent( QMouseEvent *e )
{
	if(GET_STATE() == STATE_MAINMENU)
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
}

void Menu_Main::loadCustomFonts()
{
	QStringList list;
	list 
		<< "arcade_interlaced.ttf" 
		<< "arcade_rounded.ttf" 
		<< "arista_light.ttf" 
		<< "digital_7_mono.ttf" 
		<< "angies_new_house.ttf" 
		<< "pixel_square_10.ttf" 
		<< "pixel_square_bold10.ttf" 
		<< "pxll.ttf" 
		<< "king_richard.ttf"
		<< "bebas_neue.otf"
		<< "kenyan_coffee.ttf"
		<< "imagine_font.ttf"
		<< "statix.ttf";

	
	int fontID(-1);
	bool fontWarningShown(false);
	for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator) 
	{
		QFile res(":/xkill/fonts/" + *constIterator);
		if(res.open(QIODevice::ReadOnly) == false)
		{
			if(fontWarningShown == false)
			{
				std::string fontName = (*constIterator).toStdString();
				ERROR_MESSAGEBOX("Problem loading custom font \"" + fontName + "\".");
				fontWarningShown = true;
			}
		} 
		else 
		{
			fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
			if (fontID == -1 && fontWarningShown == false)
			{
				ERROR_MESSAGEBOX("Problem loading custom font");
				fontWarningShown = true;
			}
		}
	}
}

void Menu_Main::push_menu( QFrame* menu )
{
	// Hide previous menu, if any
	if(menuStack.size() > 0)
	{
		QFrame* topMenu = menuStack.back();
		topMenu->hide();
	}

	// Show new menu
	menu->move(0,0);
	menu->resize(width(), height());
	menu->show();
	menuStack.push_back(menu);

	SEND_EVENT(&Event_PlaySound(XKILL_Enums::Sound::SOUND_BUTTON_CLICK));
}

void Menu_Main::pop_menu()
{
	// Make sure rot-menu is not poped
	if(menuStack.size() > 2)
	{
		// Pop current menu
		QFrame* topMenu = menuStack.back();
		hideMenu();
		menuStack.pop_back();

		// Show previous menu
		QFrame* menu = menuStack.back();
		menu->move(0,0);
		menu->resize(width(), height());
		showMenu();
	}
}

void Menu_Main::menuResize()
{
	QFrame* topMenu = menuStack.back();

	// Resize background
	ui.label_openingAnimation->resize(width(), height());
	ui.label_background->resize(width(), height());
	ui.label_openingAnimation->lower();
	ui.label_background->lower();

	// Resize current menu
	topMenu->resize(width(), height());


	// Update resolution menu if neccisary
	{
		int width = SETTINGS->render_width;
		int height = SETTINGS->render_height;

		if(ui.spinBox_videoResX->value() != width || ui.spinBox_videoResY->value() != height)
		{
			ui.spinBox_videoResX->setValue(width);
			ui.spinBox_videoResY->setValue(height);
		}
	}
	
	

}

void Menu_Main::setAlwaysOnTop( bool on )
{
	if(on)
	{
		// Enable Window Stay on Top flag
		this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
	}
	else
	{
		// Disable Window Stay on Top flag
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
	}

	this->show();
}

void Menu_Main::event_windowMove( Event_WindowMove* e )
{
	move(e->pos.x, e->pos.y);
	raise();
}

void Menu_Main::keyPressEvent( QKeyEvent *e )
{
	if(GET_STATE() == STATE_MAINMENU)
	{
		if(e->key() == Qt::Key_Escape)
		{
			pop_menu();
		}
	}

	QCoreApplication::sendEvent(parent, e);
}

void Menu_Main::keyReleaseEvent( QKeyEvent *e )
{
	QCoreApplication::sendEvent(parent, e);
}

void Menu_Main::event_windowResize( Event_WindowResize* e )
{
	// Resize window
	resize(e->width, e->height);

	// Make sure menu is resized as well
	menuResize();
}

void Menu_Main::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_WINDOW_FOCUS_CHANGED:
		raise();
		break;
	case EVENT_ENABLE_MENU:
		if(((Event_EnableMenu*)e)->enableMenu)
		{
			// Refresh menu
			ammo_Menu->setSettingsMenu();
			firingMode_Menu->setSettingsMenu();

			// Refresh additional menu parts
			ui.horizontalSlider_cycleLimit->setValue(SETTINGS->cycleLimit);
			ui.horizontalSlider_timeLimit->setValue(SETTINGS->timeLimit);
			ui.horizontalSlider_executionFrequency->setValue(SETTINGS->schedulerTime);
			ui.horizontalSlider_priorityToCycleRate->setValue(SETTINGS->cycleTime);
			ui.horizontalSlider_respawnTime->setValue(SETTINGS->respawnTime);
			ui.horizontalSlider_nullProcessDuration->setValue(SETTINGS->nullprocessExecutionTime);
			
			ui.label_cycleLimit->setNum(ui.horizontalSlider_cycleLimit->value());
			ui.label_timeLimit->setNum(ui.horizontalSlider_timeLimit->value());
			ui.label_respawnTime->setNum(ui.horizontalSlider_respawnTime->value());
			ui.label_executionFrequency->setNum(ui.horizontalSlider_executionFrequency->value());
			ui.label_priorityToCycleRate->setNum(ui.horizontalSlider_priorityToCycleRate->value());
			ui.label_nullProcessDuration->setNum(ui.horizontalSlider_nullProcessDuration->value());

			//input_Menu->setSettingsMenu();
			//sound_Menu->setSettingsMenu();

			// Display menu
			showMenu();
		}
		else
		{
			// Hide menu
			hideMenu();
		}
		break;
	case EVENT_WINDOW_MOVE:
		event_windowMove((Event_WindowMove*)e);
		break;
	case EVENT_WINDOW_RESIZE:
		event_windowResize((Event_WindowResize*)e);
		break;
	default:
		break;
	}
}

Menu_Main::~Menu_Main()
{
	UNSUBSCRIBE_TO_EVENTS(this);

	delete input_Menu;
	delete ammo_Menu;
	delete firingMode_Menu;
	delete sound_Menu;
}

void Menu_Main::closeEvent( QCloseEvent* event )
{
	SEND_EVENT(&Event(EVENT_QUIT_TO_DESKTOP));
}

void Menu_Main::slot_menu_next_level()
{
	levelCurrent++;
	levelCurrent %= levelNames.size();
	updateLevelSelectionInterface();
}

void Menu_Main::slot_menu_previous_level()
{
	levelCurrent--;
	if(levelCurrent < 0)
		levelCurrent += levelNames.size();

	levelCurrent %= levelNames.size();
	updateLevelSelectionInterface();
}

void DebugBillboard::init( QWidget* window, QLabel* labelTemplate )
{
	_window = window;
	_template = labelTemplate;
	_template->hide();
	_isHidden = true;

	SUBSCRIBE_TO_EVENT(this, EVENT_POST_DEBUG_MESSAGE);
	SUBSCRIBE_TO_EVENT(this, EVENT_TOGGLE_DEBUG_MESSAGES);
}

void DebugBillboard::onEvent( Event* e )
{
	EventType type = e->getType();
	switch (type) 
	{
	case EVENT_POST_DEBUG_MESSAGE:
		{
			// Ignore messages if hidden
			
			{
				Event_PostDebugMessage* debugMessage = (Event_PostDebugMessage*)(e);
				int index = debugMessage->index;

				//  Create place for more labels if required
				while(_messages.size() <= index)
				{
					QLabel* l = new QLabel(_window);
					l->setStyleSheet(_template->styleSheet());
					_messages.push_back(l);

					if(!_isHidden)
						l->show();
				}

				// Add message to label
				if(!_isHidden)
				{
					std::string message = debugMessage->message;
					QLabel* l = _messages[index];
					l->setText(message.c_str());
					l->resize(l->sizeHint());
					l->move(0, l->height() * index * 1.0f);
				}
			}
		}
		break;
	case EVENT_TOGGLE_DEBUG_MESSAGES:
		{
			// Toggle
			_isHidden = !_isHidden;

			// Hide messages
			if(_isHidden)
			{
				for(int i=0; i<_messages.size(); i++)
					_messages[i]->hide();
			}
			// Show messages
			else
			{
				for(int i=0; i<_messages.size(); i++)
					_messages[i]->show();
			}
		}
		break;
	default:
		break;
	}
}
