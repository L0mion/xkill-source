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

	int _iconIndex;
	float _fadeTimer;
	std::vector<QWidget*> _icons;

	int _iconIndex_subHud;
	float _fadeTimer_subHud;
	std::vector<QWidget*> _icons_subHud;

	Float2 _position;

public:
	WeaponInfoHud();
	~WeaponInfoHud(){}

	void init(QWidget* groupBox, QLayout* layout, QWidget* groupBox_subHud, QLayout* layout_subHud);
	void update(int iconIndex, int iconIndex_subHud);

	int width();
	void addIcon(std::string path);
	void addSubIcon(std::string path);
	void setPosition(Float2 position);
};