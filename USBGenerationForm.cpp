//Primary author: Jonathan Bedard
//Confirmed working: 10/9/2014

#ifndef USBGENERATIONFORM_CPP
#define USBGENERATIONFORM_CPP

#include "USBGenerationForm.h"
#include "glLibrary.h"
#include "WifiRC.h"

//Constructor/Destructor------------------------------------------------------------------------

	//Raised when a button is clicked
	static void button_event_USBGeneration_form(glElement* input, void* source)
	{
		USBGenerationForm* master = (USBGenerationForm*) source;
		master->button_pressed(input);
	}
	//Constructor
	USBGenerationForm::USBGenerationForm(glForm* parent, BaseController* i):
		glForm(parent)
	{
		myInfo = i;

		//Set back button
		btnBack.setText((char*) "<- Back");
		btnBack.setX(20);
		btnBack.setY(20);
		btnBack.setClickEvent(&button_event_USBGeneration_form);
		pushElement(&btnBack);
		addMouseListener(&btnBack);
	}
	//Destructor
	USBGenerationForm::~USBGenerationForm()
	{
		//Nothing needed right now (this is the destructor)
	}

//Action Functions------------------------------------------------------------------------------

	//Triggers on every form update
	void USBGenerationForm::virtual_update()
	{
		//Code here will execute on every form draw (~60 hz)
	}
	//Triggers whenever the form resizes
	void USBGenerationForm::virtualFormResize()
	{
		//Triggered anytime a form changes size
		//Use this to place static buttons based on form size (particularly top or left side)
	}
	//Raised whenever a button is pushed on this form
	void USBGenerationForm::button_pressed(glElement* source)
	{
		//Return to the previous form
		if(source==&btnBack)
		{
			return_form();
			return;
		}
	}

//Drawing Functions-----------------------------------------------------------------------------

	//Bottom layer drawing
	void USBGenerationForm::virtual_background_draw()
	{
		//Nothing right now
	}
	//Middle layer drawing
	void USBGenerationForm::virtual_draw()
	{
		
		
	}
	//Top layer drawing
	void USBGenerationForm::virtual_top_draw()
	{
		//Nothing right now
	}

#endif