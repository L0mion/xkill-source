#include "Menu_FiringMode.h"

Menu_FiringMode::Menu_FiringMode(Ui::MainWindow* ui, QMainWindow* window)
{
	this->ui = ui;
	updateLock = false;
	mutatorSettings_ = new MutatorSettings();
	

	// Set standard values
	reset();

	// Connects slots
	connect(ui->radioButton_Weapon_Single,						SIGNAL(clicked()),				this,	SLOT(setSettingsMenu()));
	connect(ui->radioButton_Weapon_Semi,						SIGNAL(clicked()),				this,	SLOT(setSettingsMenu()));
	connect(ui->radioButton_Weapon_Auto,						SIGNAL(clicked()),				this,	SLOT(setSettingsMenu()));
	
	connect(ui->checkBox_Weapon_Bullet,							SIGNAL(clicked()),				this,	SLOT(settingsMenuUpdated()));
	connect(ui->checkBox_Weapon_Scatter,						SIGNAL(clicked()),				this,	SLOT(settingsMenuUpdated()));
	connect(ui->checkBox_Weapon_Explosive,						SIGNAL(clicked()),				this,	SLOT(settingsMenuUpdated()));
	
	connect(ui->horizontalSlider_Weapon_ClipSize,				SIGNAL(valueChanged(int)),		this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Weapon_ReloadTime,				SIGNAL(valueChanged(int)),		this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Weapon_RateOfFire,				SIGNAL(valueChanged(int)),		this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Weapon_DamageModifier,			SIGNAL(valueChanged(int)),		this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Weapon_ExplosionSphereModifier,SIGNAL(valueChanged(int)),		this,	SLOT(settingsMenuUpdated()));

	connect(ui->pushButton_Weapon_Reset,						SIGNAL(clicked()),				this,	SLOT(reset()));
}

Menu_FiringMode::~Menu_FiringMode()
{
	delete mutatorSettings_;
}

void Menu_FiringMode::setSettingsMenu()	// TODO: Set good values for the sliders and make a good conversion
{
	FiringMode* firingMode = getFiringModeSettings();

	// Lock updates, to prevent ghost reads
	updateLock = true;

	ui->checkBox_Weapon_Bullet->setChecked(firingMode->canShootBullet);
	ui->checkBox_Weapon_Scatter->setChecked(firingMode->canShootScatter);
	ui->checkBox_Weapon_Explosive->setChecked(firingMode->canShootExplosive);

	ui->horizontalSlider_Weapon_ClipSize->setValue(firingMode->clipSize);
	ui->horizontalSlider_Weapon_ReloadTime->setValue(static_cast<int>(firingMode->reloadTime * 100.0f));

	ui->horizontalSlider_Weapon_RateOfFire->setValue(static_cast<int>(firingMode->cooldownBetweenShots* 100.0f));
	ui->horizontalSlider_Weapon_DamageModifier->setValue(static_cast<int>(firingMode->damageModifier * 100.0f));
	ui->horizontalSlider_Weapon_ExplosionSphereModifier->setValue(static_cast<int>(firingMode->explosionSphereModifier * 100.0f));
	
	// Release lock
	updateLock = false;
	
	// Tell menu to update itself so labels gains the correct value
	settingsMenuUpdated();
}

void Menu_FiringMode::settingsMenuUpdated()
{
	// Only update if not locked, which means we probably are settings values
	if(!updateLock)
	{
		// Read from sliders
		FiringMode* firingMode = getFiringModeSettings();

		firingMode->canShootBullet = ui->checkBox_Weapon_Bullet->isChecked();
		firingMode->canShootScatter = ui->checkBox_Weapon_Scatter->isChecked();
		firingMode->canShootExplosive = ui->checkBox_Weapon_Explosive->isChecked();
		firingMode->clipSize = ui->horizontalSlider_Weapon_ClipSize->value();
		firingMode->cooldownBetweenShots = static_cast<float>(ui->horizontalSlider_Weapon_RateOfFire->value() * 0.01f);
		firingMode->damageModifier = static_cast<float>(ui->horizontalSlider_Weapon_DamageModifier->value()) * 0.01f;
		firingMode->explosionSphereModifier = static_cast<float>(ui->horizontalSlider_Weapon_ExplosionSphereModifier->value()) * 0.01f;
		firingMode->reloadTime = static_cast<float>(ui->horizontalSlider_Weapon_ReloadTime->value()) * 0.01f;

		// Update labels
		ui->label_Weapon_ClipSize->setNum((int)firingMode->clipSize);
		ui->label_Weapon_ReloadTime->setNum(static_cast<int>(firingMode->reloadTime * 100.0f));
		ui->label_Weapon_RateOfFire->setNum(static_cast<int>(firingMode->cooldownBetweenShots));
		ui->label_Weapon_DamageModifier->setNum(static_cast<int>(firingMode->damageModifier * 100.0f));
		ui->label_Weapon_ExplosionSphereModifier->setNum(static_cast<int>(firingMode->explosionSphereModifier * 100.0f));
	}

	/*ui->doubleSpinBox_FiringMode_ReloadTime->setValue(firingMode->reloadTime);
	ui->doubleSpinBox_FiringMode_DamageModifier->setValue(firingMode->damageModifier);
	ui->doubleSpinBox_FiringMode_ExplosionSphereModifier->setValue(firingMode->explosionSphereModifier);*/
}

FiringMode* Menu_FiringMode::getFiringModeSettings()
{
	XKILL_Enums::FiringModeType firingMode = XKILL_Enums::FiringModeType::SINGLE;

	if(ui->radioButton_Weapon_Single->isChecked())
	{
		firingMode = XKILL_Enums::FiringModeType::SINGLE;
	}
	else if(ui->radioButton_Weapon_Semi->isChecked())
	{
		firingMode = XKILL_Enums::FiringModeType::SEMI;
	}
	else if(ui->radioButton_Weapon_Auto->isChecked())
	{
		firingMode = XKILL_Enums::FiringModeType::AUTO;
	}

	return &mutatorSettings_->getStandardFiringMode(firingMode);
}

void Menu_FiringMode::reset()
{
	// Reset settings
	mutatorSettings_->initStandardFiringModes();

	// Init settings menu
	setSettingsMenu();
}
