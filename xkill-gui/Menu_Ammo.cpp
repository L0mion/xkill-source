#include "Menu_Ammo.h"

Menu_Ammo::Menu_Ammo(Ui::MainWindow* ui, QMainWindow* window)
{
	this->ui = ui;

	mutatorSettings_ = new MutatorSettings();

	connect(ui->radioButton_Ammo_Bullet,						SIGNAL(clicked()),			this,	SLOT(setSettingsMenu()));
	connect(ui->radioButton_Ammo_Scatter,						SIGNAL(clicked()),			this,	SLOT(setSettingsMenu()));
	connect(ui->radioButton_Ammo_Explosive,						SIGNAL(clicked()),			this,	SLOT(setSettingsMenu()));
	
	connect(ui->horizontalSlider_Ammo_Damage,					SIGNAL(sliderMoved(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_ExplosionFinalRadius,		SIGNAL(sliderMoved(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_ExplosionInitialRadius,	SIGNAL(sliderMoved(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_ExplosionDuration,		SIGNAL(sliderMoved(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_NrOfProjectiles,			SIGNAL(sliderMoved(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_Speed,					SIGNAL(sliderMoved(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_NrOfProjectiles,			SIGNAL(sliderMoved(int)),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_VelocitVariation,			SIGNAL(sliderMoved(int)),	this,	SLOT(settingsMenuUpdated()));

	connect(ui->horizontalSlider_Ammo_Damage,					SIGNAL(sliderReleased()),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_ExplosionFinalRadius,		SIGNAL(sliderReleased()),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_ExplosionInitialRadius,	SIGNAL(sliderReleased()),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_ExplosionDuration,		SIGNAL(sliderReleased()),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_NrOfProjectiles,			SIGNAL(sliderReleased()),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_Speed,					SIGNAL(sliderReleased()),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_Spread,					SIGNAL(sliderReleased()),	this,	SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Ammo_VelocitVariation,			SIGNAL(sliderReleased()),	this,	SLOT(settingsMenuUpdated()));
																							
	connect(ui->checkBox_Ammo_Explosive,						SIGNAL(clicked()),			this,	SLOT(settingsMenuUpdated()));
}

Menu_Ammo::~Menu_Ammo()
{
	delete mutatorSettings_;
}

void Menu_Ammo::setSettingsMenu()	// TODO: Set good values for the sliders and make a good conversion
{
	Ammunition* ammo = getAmmoSettings();

	ui->horizontalSlider_Ammo_Damage->setValue(static_cast<int>(ammo->damage));
	
	ui->horizontalSlider_Ammo_ExplosionInitialRadius->setValue(static_cast<int>(ammo->explosionSphereInitialRadius * 100.0f));
	ui->horizontalSlider_Ammo_ExplosionDuration->setValue(static_cast<int>(ammo->explosionSphereExplosionDuration * 100.0f));
	ui->horizontalSlider_Ammo_ExplosionFinalRadius->setValue(static_cast<int>(ammo->explosionSphereFinalRadius * 100.0f));
	
	ui->horizontalSlider_Ammo_NrOfProjectiles->setValue(ammo->nrOfProjectilesPerSalvo);
	ui->horizontalSlider_Ammo_Speed->setValue(static_cast<int>(ammo->speed));
	ui->horizontalSlider_Ammo_Spread->setValue(static_cast<int>(ammo->spreadConeRadius * 126.6f));
	ui->horizontalSlider_Ammo_VelocitVariation->setValue(static_cast<int>(ammo->velocityVariation * 100.0f));
	ui->checkBox_Ammo_Explosive->setChecked(ammo->explosive);

	/*ui->doubleSpinBox_Ammo_Spread->setValue(ammo->spreadConeRadius);
	ui->doubleSpinBox_Ammo_VelocityVariation->setValue(ammo->velocityVariation);
	ui->doubleSpinBox_Ammo_ExplosionDuration->setValue(ammo->explosionSphereExplosionDuration);
	ui->doubleSpinBox_Ammo_ExplosionInitialRadius->setValue(ammo->explosionSphereInitialRadius);
	ui->doubleSpinBox_Ammo_ExplosionFinalRadius->setValue(ammo->explosionSphereFinalRadius);*/
}

void Menu_Ammo::settingsMenuUpdated()
{
	Ammunition* ammo = getAmmoSettings();

	ammo->damage = static_cast<float>(ui->horizontalSlider_Ammo_Damage->value());
	ammo->explosionSphereInitialRadius = static_cast<float>(ui->horizontalSlider_Ammo_ExplosionInitialRadius->value()) * 0.01f;
	ammo->explosionSphereFinalRadius = static_cast<float>(ui->horizontalSlider_Ammo_ExplosionFinalRadius->value()) * 0.01f;
	ammo->explosionSphereExplosionDuration = static_cast<float>(ui->horizontalSlider_Ammo_ExplosionDuration->value()) * 0.01f;
	ammo->explosive = ui->checkBox_Ammo_Explosive->isChecked();
	ammo->nrOfProjectilesPerSalvo = ui->horizontalSlider_Ammo_NrOfProjectiles->value();
	ammo->speed = static_cast<float>(ui->horizontalSlider_Ammo_Speed->value());
	ammo->spreadConeRadius = static_cast<float>(ui->horizontalSlider_Ammo_Spread->value()) * 0.0079f;
	ammo->velocityVariation = static_cast<float>(ui->horizontalSlider_Ammo_VelocitVariation->value()) * 0.01f;

	/*ui->doubleSpinBox_Ammo_Spread->setValue(ammo->spreadConeRadius);
	ui->doubleSpinBox_Ammo_VelocityVariation->setValue(ammo->velocityVariation);
	ui->doubleSpinBox_Ammo_ExplosionDuration->setValue(ammo->explosionSphereExplosionDuration);
	ui->doubleSpinBox_Ammo_ExplosionInitialRadius->setValue(ammo->explosionSphereInitialRadius);
	ui->doubleSpinBox_Ammo_ExplosionFinalRadius->setValue(ammo->explosionSphereFinalRadius);*/
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