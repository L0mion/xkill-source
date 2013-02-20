#include "Menu_Sound.h"

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeManager.h>

ATTRIBUTES_DECLARE_ALL

Menu_Sound::Menu_Sound(Ui::MainWindow* ui, QMainWindow* window)
{
	ATTRIBUTES_INIT_ALL

	this->ui = ui;

	connect(ui->checkBox_Sound_Mute,			SIGNAL(clicked()),			this, SLOT(settingsMenuUpdated()));
	connect(ui->horizontalSlider_Sound_Volume,	SIGNAL(valueChanged(int)),	this, SLOT(settingsMenuUpdated()));

	// Set standard values
	ui->horizontalSlider_Sound_Volume->setValue(settings->soundVolume * 100.0f);
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

	settings->soundMuted = ui->checkBox_Sound_Mute->isChecked();
	settings->soundVolume = static_cast<float>(ui->horizontalSlider_Sound_Volume->value()) / 100.0f;		

	SEND_EVENT(&Event(EVENT_UPDATESOUNDSETTINGS));
}

