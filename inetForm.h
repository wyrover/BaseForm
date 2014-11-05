//Primary author: Jonathan Bedard
//Confirmed working: 10/9/2014

#ifndef INETFORM_H
#define INETFORM_H

#include "glLibrary.h"
#include "WifiRC.h"

class inetForm_button_struct
{
public:
	glLabel lblIP;
	glElement elmStatus;
	glButton btnIsPolling;

	glButton btnDelete;
	bool del_flag;

	address_module* mdl;
};

class inetForm:public glForm
{
	protected:
		ConnectionManager* myInfo;

		//Internet on
		glButton btnInternetState;

		//Server label
		bool internet_state;
		glLabel lblServer;
		glElement ltServer;
		glLabel lblKeyStatus;
		glElement ltKey;
		glButton btnNewKeys;

		//Add IP
		string last_IP;
		bool valid_IP;
		glLabel lblAddIP;
		glTextbox tbxAddIP;
		glButton btnAddIP;

		//Button list
		glLabel lblIPTitle;
		glLabel lblIPStatus;
		int btn_update_count;

		list<inetForm_button_struct*>* btn_list;

		//Back button
		glButton btnBack;

		//Functions
		void setbtnAddIPAttributes();

	public:
		//Global color variables
		color col_red;
		color col_yell;
		color col_green;
		color col_blue;
		color col_black;
		color col_white;

		color col_grey;
		color col_lite_grey;
		color col_clicked;

		inetForm(glForm* parent, ConnectionManager* i);
		virtual ~inetForm();

		//Update Functions
		void virtual virtual_update();
		void updateButtonList();

		//Action Functions
		void virtual virtualFormResize();
		void button_pressed(glElement* source);
};

#endif
