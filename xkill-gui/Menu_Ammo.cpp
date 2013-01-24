#include "Menu_Ammo.h"

Menu_Ammo::Menu_Ammo(Ui::MainMenu* ui)
{
	this->ui = ui;

	mutatorSettings_ = new MutatorSettings();
}

Menu_Ammo::~Menu_Ammo()
{
	delete mutatorSettings_;
}

void Menu_Ammo::setSettingsMenu()	// TODO: Set good values for the sliders and make a good conversion
{
	Ammunition* ammo = getAmmoSettings();

	ui->horizontalSlider_Ammo_Damage->setValue(ammo->damage);
	ui->horizontalSlider_Ammo_ExplosionSphere->setValue(ammo->explosionSphere);
	ui->horizontalSlider_Ammo_NrOfProjectiles->setValue(ammo->nrOfProjectiles);
	ui->horizontalSlider_Ammo_Speed->setValue(ammo->speed);
	ui->horizontalSlider_Ammo_Spread->setValue(ammo->spread);
	ui->horizontalSlider_Ammo_VelocitVariation->setValue(ammo->velocityVariation);
	ui->groupBox_Ammo_Explosive->setChecked(ammo->explosive);
}

void Menu_Ammo::settingsMenuUpdated()
{
	Ammunition* ammo = getAmmoSettings();

	ammo->damage = ui->horizontalSlider_Ammo_Damage->value();
	ammo->explosionSphere = ui->horizontalSlider_Ammo_ExplosionSphere->value();
	ammo->explosive = ui->groupBox_Ammo_Explosive->isChecked();
	ammo->nrOfProjectiles = ui->horizontalSlider_Ammo_NrOfProjectiles->value();
	ammo->speed = ui->horizontalSlider_Ammo_Speed->value();
	ammo->spread = ui->horizontalSlider_Ammo_Spread->value();
	ammo->velocityVariation = ui->horizontalSlider_Ammo_VelocitVariation->value();
}

Ammunition* Menu_Ammo::getAmmoSettings()
{
	Ammunition::AmmunitionType ammoType = Ammunition::BULLET;

	if(ui->radioButton_Ammo_Bullet->isChecked())
	{
		ammoType = Ammunition::BULLET;
	}
	else if(ui->radioButton_Ammo_Scatter->isChecked())
	{
		ammoType = Ammunition::SCATTER;
	}
	else if(ui->radioButton_Ammo_Explosive->isChecked())
	{
		ammoType = Ammunition::EXPLOSIVE;
	}

	return &mutatorSettings_->getStandardAmmunition(ammoType);
}