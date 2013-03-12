#include "Menu_Ammo.h"

Menu_Ammo::Menu_Ammo(Ui::MainWindow* ui, QMainWindow* window)
{
	this->ui = ui;
	mutatorSettings_ = new MutatorSettings();
	updateLock = false;

	// Set standard values
	reset();

	// Connects slots
	connect(ui->radioButton_Ammo_Bullet,						SIGNAL(clicked()),			this,	SLOT(setSettingsMenu()));
	connect(ui->radioButton_Ammo_Scatter,						SIGNAL(clicked()),			this,	SLOT(setSettingsMenu()));
	connect(ui->radioButton_Ammo_Explosive,						SIGNAL(clicked()),			this,	SLOT(setSettingsMenu()));
	

	connect(ui->horizontalSlider_Ammo_Initial,					SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_Maximum,					SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_Damage,					SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_Speed,					SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_NrOfProjectiles,			SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_VelocitVariation,			SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_Spread,					SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));

	connect(ui->checkBox_Ammo_Explosive,						SIGNAL(clicked()),			this,	SLOT(settingsMenuUpdated()));

	connect(ui->horizontalSlider_Ammo_ExplosionInitialRadius,	SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_ExplosionFinalRadius,		SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_ExplosionDuration,		SIGNAL(valueChanged(int)),	this,	SLOT(settingsMenuUpdated()));			

	connect(ui->pushButton_Ammo_Reset,							SIGNAL(clicked()),			this,	SLOT(reset()));
}

Menu_Ammo::~Menu_Ammo()
{
	delete mutatorSettings_;
}

void Menu_Ammo::setSettingsMenu()	// TODO: Set good values for the sliders and make a good conversion
{
	Ammunition* ammo = getAmmoSettings();

	// Lock updates, to prevent ghost reads
	updateLock = true;

	// Update sliders
	ui->horizontalSlider_Ammo_Initial					->	setValue(ammo->initialTotalNrOfShots);
	ui->horizontalSlider_Ammo_Maximum					->	setValue(ammo->maxTotalNrOfShots);
	ui->horizontalSlider_Ammo_Maximum					->  setMinimum(ammo->initialTotalNrOfShots);
	ui->horizontalSlider_Ammo_Damage					->	setValue(static_cast<int>(ammo->damage));
	ui->horizontalSlider_Ammo_Speed						->	setValue(static_cast<int>(ammo->speed));
	ui->horizontalSlider_Ammo_NrOfProjectiles			->	setValue(ammo->nrOfProjectilesPerSalvo);
	ui->horizontalSlider_Ammo_VelocitVariation			->	setValue(static_cast<int>(ammo->velocityVariation * 100.0f));
	ui->horizontalSlider_Ammo_Spread					->	setValue(static_cast<int>((ammo->spreadConeRadius * 180) /3.14f));

	ui->checkBox_Ammo_Explosive							->	setChecked(ammo->explosive);

	ui->horizontalSlider_Ammo_ExplosionDuration			->	setValue(static_cast<int>(ammo->explosionSphereExplosionDuration * 10.0f));
	ui->horizontalSlider_Ammo_ExplosionInitialRadius	->	setValue(static_cast<int>(ammo->explosionSphereInitialRadius * 5.0f));
	ui->horizontalSlider_Ammo_ExplosionFinalRadius		->	setValue(static_cast<int>(ammo->explosionSphereFinalRadius * 5.0f));

	// Release lock
	updateLock = false;
	
	// Tell menu to update itself so labels gains the correct value
	settingsMenuUpdated();
}

void Menu_Ammo::settingsMenuUpdated()
{
	// Only update if not locked, which means we probably are settings values
	if(!updateLock)
	{
		Ammunition* ammo = getAmmoSettings();

		if(ui->checkBox_Ammo_Explosive->isChecked())
		{
			ui->horizontalSlider_Ammo_ExplosionDuration->setEnabled(true);
			ui->horizontalSlider_Ammo_ExplosionInitialRadius->setEnabled(true);
			ui->horizontalSlider_Ammo_ExplosionFinalRadius->setEnabled(true);
		}
		else
		{
			ui->horizontalSlider_Ammo_ExplosionDuration->setEnabled(false);
			ui->horizontalSlider_Ammo_ExplosionInitialRadius->setEnabled(false);
			ui->horizontalSlider_Ammo_ExplosionFinalRadius->setEnabled(false);
		}

		// Read from sliders
		ammo->initialTotalNrOfShots				= ui->horizontalSlider_Ammo_Initial->value();
		ui->horizontalSlider_Ammo_Maximum		->  setMinimum(ammo->initialTotalNrOfShots);
		ammo->maxTotalNrOfShots					= ui->horizontalSlider_Ammo_Maximum->value();
		ammo->damage							= static_cast<float>(ui->horizontalSlider_Ammo_Damage->value());
		ammo->speed								= static_cast<float>(ui->horizontalSlider_Ammo_Speed->value());
		ammo->nrOfProjectilesPerSalvo			= ui->horizontalSlider_Ammo_NrOfProjectiles->value();
		ammo->velocityVariation					= static_cast<float>(ui->horizontalSlider_Ammo_VelocitVariation->value()) * 0.01f;
		ammo->spreadConeRadius					= static_cast<float>(ui->horizontalSlider_Ammo_Spread->value()) * 3.14f / 180;

		ammo->explosive							= ui->checkBox_Ammo_Explosive->isChecked();

		ammo->explosionSphereExplosionDuration	= static_cast<float>(ui->horizontalSlider_Ammo_ExplosionDuration->value()) * 0.1f;
		ammo->explosionSphereInitialRadius		= static_cast<float>(ui->horizontalSlider_Ammo_ExplosionInitialRadius->value()) * 0.2f;
		ammo->explosionSphereFinalRadius		= static_cast<float>(ui->horizontalSlider_Ammo_ExplosionFinalRadius->value()) * 0.2f;

		// Update labels
		ui->label_Ammo_Initial					->	setNum(static_cast<int>(ammo->initialTotalNrOfShots));
		ui->label_Ammo_Maximum					->	setNum(static_cast<int>(ammo->maxTotalNrOfShots));
		ui->label_Ammo_Damage					->	setNum(ammo->damage);
		ui->label_Ammo_Speed					->	setNum(ammo->speed);
		ui->label_Ammo_NrOfProjectiles			->	setNum(static_cast<int>(ammo->nrOfProjectilesPerSalvo));
		ui->label_Ammo_VelocityVariation		->	setNum(ammo->velocityVariation);
		ui->label_Ammo_Spread					->	setNum(ammo->spreadConeRadius * 360 / 3.14f);

		ui->label_Ammo_ExplosionDuration		->	setNum(ammo->explosionSphereExplosionDuration);
		ui->label_Ammo_ExplosionInitialRadius	->	setNum(ammo->explosionSphereInitialRadius);
		ui->label_Ammo_ExplosionFinalRadius		->	setNum(ammo->explosionSphereFinalRadius);
	}
}

Ammunition* Menu_Ammo::getAmmoSettings()
{
	XKILL_Enums::AmmunitionType ammoType = XKILL_Enums::AmmunitionType::BULLET;

	if(ui->radioButton_Ammo_Bullet->isChecked())
	{
		ammoType = XKILL_Enums::AmmunitionType::BULLET;
	}
	else if(ui->radioButton_Ammo_Scatter->isChecked())
	{
		ammoType = XKILL_Enums::AmmunitionType::SCATTER;
	}
	else if(ui->radioButton_Ammo_Explosive->isChecked())
	{
		ammoType = XKILL_Enums::AmmunitionType::EXPLOSIVE;
	}

	return &mutatorSettings_->getStandardAmmunition(ammoType);
}

void Menu_Ammo::reset()
{
	// Reset settings
	mutatorSettings_->initStandardAmmunition();

	// Init settings menu
	setSettingsMenu();
}
