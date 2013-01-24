#include "Menu_FiringMode.h"

Menu_FiringMode::Menu_FiringMode(Ui::MainMenu* ui)
{
	this->ui = ui;

	mutatorSettings_ = new MutatorSettings();
}

Menu_FiringMode::~Menu_FiringMode()
{
	delete mutatorSettings_;
}

void Menu_FiringMode::setSettingsMenu()	// TODO: Set good values for the sliders and make a good conversion
{
	FiringMode* firingMode = getFiringModeSettings();

	ui->checkBox_Weapon_Bullet->setChecked(firingMode->canShootBullet);
	ui->checkBox_Weapon_Scatter->setChecked(firingMode->canShootScatter);
	ui->checkBox_Weapon_Explosive->setChecked(firingMode->canShootExplosive);
	ui->horizontalSlider_Weapon_ClipSize->setValue(firingMode->clipSize);
	ui->horizontalSlider_Weapon_DamageModifier->setValue(firingMode->damageModifier);
	ui->horizontalSlider_Weapon_ExplosionSphereModifier->setValue(firingMode->explosionSphereModifier);
	ui->horizontalSlider_Weapon_RateOfFire->setValue(firingMode->cooldownBetweenShots);
	ui->horizontalSlider_Weapon_ReloadTime->setValue(firingMode->reloadTime);
}

void Menu_FiringMode::settingsMenuUpdated()
{
	FiringMode* firingMode = getFiringModeSettings();
	
	firingMode->canShootBullet = ui->checkBox_Weapon_Bullet->isChecked();
	firingMode->canShootScatter = ui->checkBox_Weapon_Scatter->isChecked();
	firingMode->canShootExplosive = ui->checkBox_Weapon_Explosive->isChecked();
	firingMode->clipSize = ui->horizontalSlider_Weapon_ClipSize->value();
	firingMode->cooldownBetweenShots = ui->horizontalSlider_Weapon_RateOfFire->value();
	firingMode->damageModifier = ui->horizontalSlider_Weapon_DamageModifier->value();
	firingMode->explosionSphereModifier = ui->horizontalSlider_Weapon_ExplosionSphereModifier->value();
	firingMode->reloadTime = ui->horizontalSlider_Weapon_ReloadTime->value();
}

FiringMode* Menu_FiringMode::getFiringModeSettings()
{
	FiringMode::FiringModeType firingMode = FiringMode::SINGLE;

	if(ui->radioButton_Weapon_Single->isChecked())
	{
		firingMode = FiringMode::SINGLE;
	}
	else if(ui->radioButton_Weapon_Semi->isChecked())
	{
		firingMode = FiringMode::SEMI;
	}
	else if(ui->radioButton_Weapon_Auto->isChecked())
	{
		firingMode = FiringMode::AUTO;
	}

	return &mutatorSettings_->getStandardFiringMode(firingMode);
}