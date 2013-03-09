#pragma once


#include <xkill-utilities/Util.h>
#include <QtGui/QLabel>
#include <QtGui/QLayout>

class WeaponInfoHud
{
private:
	QWidget* _groupBox;
	QLayout* _layout;
	QWidget* _groupBox_subHud;
	QLayout* _layout_subHud;

	int _firingIndex;
	float _fadeTimer;

	Float2 _position_current;
	Float2 _position_show;
	Float2 _position_hide;

	std::vector<QWidget*> _firingIcons;

public:
	WeaponInfoHud();
	~WeaponInfoHud(){}

	void init(QWidget* groupBox, QLayout* layout, QWidget* groupBox_subHud, QLayout* layout_subHud);
	void update(int firingIndex);

	int width();
	void addFiringIcon(std::string path);
	void setPositionOnShow(Float2 position);
	void setPositionOnHide(Float2 position);
};