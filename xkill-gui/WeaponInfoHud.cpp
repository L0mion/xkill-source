#include "WeaponInfoHud.h"

WeaponInfoHud::WeaponInfoHud()
{
	_iconIndex = -1;
	_fadeTimer = 0;
	_iconIndex_subHud = -1;
	_fadeTimer_subHud = 0;
}

void WeaponInfoHud::init( QWidget* groupBox, QLayout* layout, QWidget* groupBox_subHud, QLayout* layout_subHud )
{
	_groupBox = groupBox;
	_layout = layout;
	_groupBox_subHud = groupBox_subHud;
	_layout_subHud = layout_subHud;

	// Add weapon icons, one for each weapon
	addIcon(":/xkill/images/icons/weapons/w_single.png");
	addIcon(":/xkill/images/icons/weapons/w_semi.png");
	addIcon(":/xkill/images/icons/weapons/w_auto.png");

	// Add ammo icons
	addSubIcon(":/xkill/images/icons/weapons/a_bullet.png");
	addSubIcon(":/xkill/images/icons/weapons/a_scatter.png");
	addSubIcon(":/xkill/images/icons/weapons/a_explosive.png");
}

void WeaponInfoHud::update( int iconIndex, int iconIndex_subHud )
{
	// Detect change in firing mode
	if(_iconIndex != iconIndex)
	{
		// Clear previous selection
		if(_iconIndex >= 0)
			_icons.at(_iconIndex)->setStyleSheet("");

		// Select new
		_iconIndex = iconIndex;
		_icons.at(_iconIndex)->setStyleSheet("background: rgba(255, 255, 255, 150);");

		// Set face timer
		_fadeTimer = 1.0f;

		// Extent show time on sub info-box
		if(_fadeTimer_subHud > 0.0f)
			_fadeTimer_subHud = 1.0f;
	}

	// Detect change in ammo
	if(_iconIndex_subHud != iconIndex_subHud)
	{
		// Clear previous selection
		if(_iconIndex_subHud >= 0)
			_icons_subHud.at(_iconIndex_subHud)->setStyleSheet("");

		// Select new
		_iconIndex_subHud = iconIndex_subHud;
		_icons_subHud.at(_iconIndex_subHud)->setStyleSheet("background: rgba(255, 255, 255, 150);");

		// Set face timer
		_fadeTimer_subHud = 1.0f;

		// Extent show time on main info-box
		if(_fadeTimer > 0.0f)
			_fadeTimer = 1.0f;
	}


	// Determine if to hide or show by choosing between two different 
	// target positions to move between

	Float2 targetPos;
	Float2 targetPos_subHud;

	if(_fadeTimer > 0.0f)
		_fadeTimer -= SETTINGS->trueDeltaTime;
	if(_fadeTimer_subHud > 0.0f)
		_fadeTimer_subHud -= SETTINGS->trueDeltaTime;


	// Determine if to show or hide
	// and offset the determined position 
	// relative to info-box size

	const int kHidingEpsilon = 10;
	if(_fadeTimer > 0.0f)
	{
		// Show position
		targetPos = _position;
		targetPos.x -= _groupBox->width();
		targetPos.y -= _groupBox->height();
	}
	else
	{
		// Hiding position
		targetPos = _position;
		targetPos.y -= _groupBox->height();
		targetPos.x += kHidingEpsilon;
	}

	if(_fadeTimer_subHud > 0.0f)
	{
		// Show position
		targetPos_subHud = _position;
		targetPos_subHud.x -= _groupBox_subHud->width();
		targetPos_subHud.y -= _groupBox_subHud->height();

		// Adapt sub-box to be shown together with main-box
		if(_fadeTimer > 0.0f)
		{
			targetPos_subHud.x += - _groupBox->width();
			//targetPos_subHud.y += - (32 + 9) * _iconIndex;
			
			// Remove left margins to make them
			// they fit together more nicely
			QMargins margins = _groupBox_subHud->layout()->contentsMargins();
			if(margins.right() != 0)
			{
				margins.setRight(0);
				_groupBox_subHud->layout()->setContentsMargins(margins);
				_groupBox_subHud->resize(_groupBox_subHud->sizeHint());
			}
		}
		else
		{
			// Restore margins if we have tampered with it
			QMargins margins = _groupBox_subHud->layout()->contentsMargins();
			if(margins.right() == 0)
			{
				margins.setRight(9);
				_groupBox_subHud->layout()->setContentsMargins(margins);
				_groupBox_subHud->resize(_groupBox_subHud->sizeHint());
			}
		}
	}
	else
	{
		// Hiding position
		targetPos_subHud = _position;
		targetPos_subHud.y -= _groupBox_subHud->height();
		targetPos_subHud.x += kHidingEpsilon;
	}


	// Interpolate position
	//if(_fadeTimer > -1.0f)
	{
		// Determine factor which to interpolate with
		float factor = 9.0f * SETTINGS->trueDeltaTime;
		if(factor > 1.0f)
			factor = 1.0f;

		// Determine new position
		Float2 currentPos;
		currentPos.x = _groupBox->pos().x();
		currentPos.y = _groupBox->pos().y();
		Float2 newPos = Float2::lerp(&currentPos, &targetPos, factor);
		_groupBox->move(newPos.x, newPos.y);

		// Determine new sub-hud position
		currentPos.x = _groupBox_subHud->pos().x();
		currentPos.y = _groupBox_subHud->pos().y();
		newPos = Float2::lerp(&currentPos, &targetPos_subHud, factor);
		_groupBox_subHud->move(newPos.x, newPos.y);
	}
}

void WeaponInfoHud::setPosition( Float2 position )
{
	// Set target
	_position = position;
}

int WeaponInfoHud::width()
{
	return _groupBox->width();
}

void WeaponInfoHud::addIcon( std::string path )
{
	// Add icon to layout
	QLabel* l = new QLabel();
	l->setPixmap(QString(path.c_str()));
	_layout->addWidget(l);
	_icons.push_back(l);

	// Rearrange layout to make new items appear at
	// top instead of bottom, this is quite ineffective,
	// but seems to be the only way of solving it in Qt,
	// since we don't have a lot of icons it should not affect performance in practice
	int numPreviousIcons = _icons.size() - 1;
	for(int i=0; i<numPreviousIcons; i++)
	{
		_layout->addWidget(_layout->takeAt(0)->widget());
	}

	// Resize group box to accommodate new size
	_groupBox->resize(_groupBox->sizeHint());
}

void WeaponInfoHud::addSubIcon( std::string path )
{
	// Add icon to layout
	QLabel* l = new QLabel();
	l->setPixmap(QString(path.c_str()));
	_layout_subHud->addWidget(l);
	_icons_subHud.push_back(l);

	// Rearrange layout to make new items appear at
	// top instead of bottom, this is quite ineffective,
	// but seems to be the only way of solving it in Qt,
	// since we don't have a lot of icons it should not affect performance in practice
	int numPreviousIcons = _icons_subHud.size() - 1;
	for(int i=0; i<numPreviousIcons; i++)
	{
		_layout_subHud->addWidget(_layout_subHud->takeAt(0)->widget());
	}

	// Resize group box to accommodate new size
	_groupBox_subHud->resize(_groupBox_subHud->sizeHint());
}
