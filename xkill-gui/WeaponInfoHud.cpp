#include "WeaponInfoHud.h"

WeaponInfoHud::WeaponInfoHud()
{
	_firingIndex = -1;
	_fadeTimer = 0;
}

void WeaponInfoHud::init( QWidget* groupBox, QLayout* layout, QWidget* groupBox_subHud, QLayout* layout_subHud )
{
	_groupBox = groupBox;
	_layout = layout;
	_groupBox_subHud = groupBox_subHud;
	_layout_subHud = layout_subHud;

	// Add weapon icons, one for each weapon
	addFiringIcon(":/xkill/images/icons/weapons/a_bullet.png");
	addFiringIcon(":/xkill/images/icons/weapons/a_scatter.png");
	addFiringIcon(":/xkill/images/icons/weapons/a_explosive.png");
}

void WeaponInfoHud::update( int firingIndex )
{
	// Detect change in firing mode
	if(_firingIndex != firingIndex)
	{
		// Clear previous selection
		if(_firingIndex >= 0)
			_firingIcons.at(_firingIndex)->setStyleSheet("");

		// Select new
		_firingIndex = firingIndex;
		_firingIcons.at(_firingIndex)->setStyleSheet("background: rgba(255, 255, 255, 150);");

		// Set face timer
		_fadeTimer = 1.0f;
	}


	// Determine if to hide or show by choosing between two different 
	// target positions to move between

	Float2 targetPos;

	if(_fadeTimer > 0.0f)
		_fadeTimer -= SETTINGS->trueDeltaTime;

	// Determine if to show or hide
	if(_fadeTimer > 0.0f)
		targetPos = _position_show;
	else
		targetPos = _position_hide;


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
		_position_current = newPos;
		_groupBox->move(_position_current.x, _position_current.y);
	}
}

void WeaponInfoHud::addFiringIcon( std::string path )
{
	// Add icon to layout
	QLabel* l = new QLabel();
	l->setPixmap(QString(path.c_str()));
	_layout->addWidget(l);
	_firingIcons.push_back(l);

	// Rearrange layout to make new items appear at
	// top instead of bottom, this is quite ineffective,
	// but seems to be the only way of solving it in Qt,
	// since we don't have a lot of icons it should not affect performance in practice
	int numPreviousIcons = _firingIcons.size() - 1;
	for(int i=0; i<numPreviousIcons; i++)
	{
		_layout->addWidget(_layout->takeAt(0)->widget());
	}

	// Resize group box to accommodate new size
	_groupBox->resize(_groupBox->sizeHint());
}

void WeaponInfoHud::setPositionOnShow( Float2 position )
{
	// We don't want to care how 
	// many items is in the menu, therefore
	// offset position relative to height
	position.y -= _groupBox->height();

	// Set target
	_position_show = position;
}

void WeaponInfoHud::setPositionOnHide( Float2 position )
{
	// Compensate for height of label
	position.y -= _groupBox->height();

	// Set target
	_position_hide = position;

	// Move label to hidden position
	_position_current = _position_hide;
	_groupBox->move(_position_current.x, _position_current.y);
}

int WeaponInfoHud::width()
{
	return _groupBox->width();
}
