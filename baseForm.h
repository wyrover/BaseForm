//Primary author: Jonathan Bedard
//Confirmed working: 10/27/2014

#ifndef BASEFORM_H
#define BASEFORM_H

//Constant definitions
#define BASEFORM_MIN_HEIGHT 600
#define BASEFORM_MIN_WIDTH 800

#include "glLibrary.h"
#include "WifiRC.h"

class baseForm:public glForm
{
protected:
	BaseController myInfo;
	int message_status;
	
	//Menu bar
	glFormMenu menu_bar;
	glButton* barProperties;
	glButton* barUSBGen;
	glButton* barExit;

	glButton* barInet;
	char* menu_title_array;
	
	//Connection Control labels
	glLabel lblStaticIP;
	glLabel lblIP;
	
	glLabel lblLAN;
	glElement ltLAN;
	
	glLabel lblServer;
	glElement ltServer;
	
	glLabel lblStaticConName;
	glLabel lblDynamicConName;

	//Control Items
	glButton btnPower;
	glButton btnPicture;
	
	glElement elUp;
	glElement elLeft;
	glElement elRight;
	glElement elDown;


public:
	//Global color variables
	color col_red;
	color col_yell;
	color col_green;
	color col_black;
	color col_haze_black;
	color col_white;
	
	//Constructor/destructor
	baseForm(int argv, char ** addr,
			void (*dispFunc)(),void (*keyBoard)(unsigned char, int, int),void (*mouse)(int,int,int,int),
			void (*mouseMove)(int,int),void (*kU)(unsigned char, int, int),
			void (*spc_down)(int,int,int),void (*spc_up)(int,int,int));
	virtual ~baseForm();

	//Update Functions
	void virtual virtual_update();
	void virtual sendMessage(int x);

	//Actions
	void virtual virtualFormResize();
	void button_pressed(glElement* source);
	void virtual virtual_draw();
	
	//Action Functions
	void virtual virtualKeyboard(unsigned char key, int mousePositionX, int mousePositionY);
	void virtual virtualKeyUp(unsigned char key, int mousePositionX, int mousePositionY);
	void virtual virtualSpecialKeyDown(int key, int mousePositionX, int mousePositionY);
	void virtual virtualSpecialKeyUp(int key, int mousePositionX, int mousePositionY);
	
	//Get Functions
	BaseController* getController();
	glElement* getUpElm();
	glElement* getDownElm();
	glElement* getLeftElm();
	glElement* getRightElm();
};

#endif