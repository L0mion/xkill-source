#include "Menu_Sound.h"

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeManager.h>

ATTRIBUTES_DECLARE_ALL

Menu_Sound::Menu_Sound(Ui::MainWindow* ui, QMainWindow* window)
{
	ATTRIBUTES_INIT_ALL

	this->ui = ui;

	// Set standard values
	ui->horizontalSlider_soundVolume_music->setValue(SETTINGS->soundVolume_music * 100.0f);
	ui->horizontalSlider_soundVolume_effects->setValue(SETTINGS->soundVolume_effects * 100.0f);

	connect(ui->checkBox_Sound_Mute,				  SIGNAL(clicked()),			this, SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_soundVolume_music,	  SIGNAL(valueChanged(int)),	this, SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_soundVolume_effects, SIGNAL(valueChanged(int)),	this, SLOT(settingsMenuUpdated()));
}

Menu_Sound::~Menu_Sound()
{

}

void Menu_Sound::settingsMenuUpdated()
{
	//Attribute_SoundSettings* soundSettings;
	//while(itrSoundSettings.hasNext())
	//{
	//	soundSettings = itrSoundSettings.getNext();

	//	soundSettings->soundMuted = ui->checkBox_Sound_Mute->isChecked();
	//	soundSettings->soundVolume = static_cast<float>(ui->horizontalSlider_Sound_Volume->value()) / 100.0f;		
	//}

	SETTINGS->soundMuted = ui->checkBox_Sound_Mute->isChecked();
	SETTINGS->soundVolume_music = static_cast<float>(ui->horizontalSlider_soundVolume_music->value()) / 100.0f;
	SETTINGS->soundVolume_effects = static_cast<float>(ui->horizontalSlider_soundVolume_effects->value()) / 100.0f;		

	SEND_EVENT(&Event(EVENT_UPDATESOUNDSETTINGS));
}

