//Primary author: Jonathan Bedard
//Confirmed working: 10/28/2014

#ifndef PROPERTIESFORM_H
#define PROPERTIESFORM_H

#include "glLibrary.h"
#include "WifiRC.h"

class propertiesForm_button_struct
{
public:
	glButton btnID;
	glElement indLAN;
	glElement indServer;
	glElement indRadio;
	bool selected;

	ID_module* mdl;
};
class propertiesForm:public glForm
{
	protected:
		BaseController* myInfo;
		glLabel lblTitle;

		//Name label
		glLabel lblName;
		glTextbox tbxName;

		//Connection screening
		glLabel lblConScreen;
		glButton btnAutoSelect;
		glButton btnPreferedAuto;
		glButton btnListSelection;
		glButton btnPreferedListSelection;
		glButton btnOneSelection;

		//ID List
		glLabel lblIDList;
		glLabel lblLANind;
		glLabel lblServerind;
		glLabel lblRadioind;
		glButton btnChoiceStatus;
		glButton btnDelete;

		int btn_update_count;
		list<propertiesForm_button_struct*>* btn_list;

		//Buttons
		glButton btnBack;

	public:
		//Global color variables
		color col_red;
		color col_yell;
		color col_oran;
		color col_green;
		color col_blue;
		color col_black;
		color col_white;

		propertiesForm(glForm* parent, BaseController* i);
		virtual ~propertiesForm();

		//Update Functions
		void virtual virtual_update();
		void updateButtonList();

		//Action Functions
		void virtual virtualFormResize();
		void button_pressed(glElement* source);
};

#endif
