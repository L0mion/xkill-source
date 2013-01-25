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

	ui->horizontalSlider_Ammo_Damage->setValue(static_cast<int>(ammo->damage));
	ui->horizontalSlider_Ammo_ExplosionSphere->setValue(static_cast<int>(ammo->explosionSphere * 100.0f));
	ui->horizontalSlider_Ammo_NrOfProjectiles->setValue(ammo->nrOfProjectiles);
	ui->horizontalSlider_Ammo_Speed->setValue(static_cast<int>(ammo->speed));
	ui->horizontalSlider_Ammo_Spread->setValue(static_cast<int>(ammo->spread * 126.6f));
	ui->horizontalSlider_Ammo_VelocitVariation->setValue(static_cast<int>(ammo->velocityVariation * 100.0f));
	ui->groupBox_Ammo_Explosive->setChecked(ammo->explosive);

	ui->doubleSpinBox_Ammo_Spread->setValue(ammo->spread);
	ui->doubleSpinBox_Ammo_VelocityVariation->setValue(ammo->velocityVariation);
	ui->doubleSpinBox_Ammo_ExplosionSphere->setValue(ammo->explosionSphere);
}

void Menu_Ammo::settingsMenuUpdated()
{
	Ammunition* ammo = getAmmoSettings();

	ammo->damage = static_cast<float>(ui->horizontalSlider_Ammo_Damage->value());
	ammo->explosionSphere = static_cast<float>(ui->horizontalSlider_Ammo_ExplosionSphere->value()) * 0.01f;
	ammo->explosive = ui->groupBox_Ammo_Explosive->isChecked();
	ammo->nrOfProjectiles = ui->horizontalSlider_Ammo_NrOfProjectiles->value();
	ammo->speed = static_cast<float>(ui->horizontalSlider_Ammo_Speed->value());
	ammo->spread = static_cast<float>(ui->horizontalSlider_Ammo_Spread->value()) * 0.0079f;
	ammo->velocityVariation = static_cast<float>(ui->horizontalSlider_Ammo_VelocitVariation->value()) * 0.01f;

	ui->doubleSpinBox_Ammo_Spread->setValue(ammo->spread);
	ui->doubleSpinBox_Ammo_VelocityVariation->setValue(ammo->velocityVariation);
	ui->doubleSpinBox_Ammo_ExplosionSphere->setValue(ammo->explosionSphere);
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