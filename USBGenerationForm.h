//Primary author: Jonathan Bedard
//Confirmed working: 10/7/2014

#ifndef USBGENERATIONFORM_H
#define USBGENERATIONFORM_H

#include "glLibrary.h"
#include "WifiRC.h"

class USBGenerationForm:public glForm
{
protected:
	BaseController* myInfo;

	//Buttons
	glButton btnBack;

public:
	USBGenerationForm(glForm* parent, BaseController* i);
	virtual ~USBGenerationForm();

	//Action Functions
	void virtual virtual_update();
	void virtual virtualFormResize();
	void button_pressed(glElement* source);

	//Drawing Functions
	void virtual virtual_background_draw();
	void virtual virtual_draw();
	void virtual virtual_top_draw();
};

#endif