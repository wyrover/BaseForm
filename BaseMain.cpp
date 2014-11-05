//Primary author: Jonathan Bedard
//Confirmed working: 9/29/2014

#ifndef BASEMAIN_CPP
#define BASEMAIN_CPP

#include <stdio.h>
#include <iostream>
#include "glLibrary.h"

//Load external cpp files
#include "WifiRCComplete.h"
#include "glLibraryComplete.h"
#include "CryptoGatewayComplete.h"

#include "baseForm.h"

using namespace std;

glForm* dis;

//Display Function
void display() 
{
	dis->clear();
	dis->background_draw();
	dis->draw();
	dis->top_draw();
	dis->display();

	//The Update Function
	dis->update();
}
//Handles a key click
void keyboard ( unsigned char key, int mousePositionX, int mousePositionY )		
{
	dis->keyboard(key,mousePositionX,mousePositionY);
}
void key_up ( unsigned char key, int mousePositionX, int mousePositionY )		
{
	dis->keyUp(key,mousePositionX,mousePositionY);
}
//Handles the mouse click
void mouseClick(int button, int state, int mousePositionX, int mousePositionY)
{
	dis->mouse(button,state,mousePositionX,mousePositionY);
}
//Handles the mouse movement
void mouseMove(int mouseX, int mouseY)
{
	dis->mouseMove(mouseX,mouseY);
}
//Handles a special key pressed
void special_key_pressed(int key, int mouseX, int mouseY)
{
	dis->specialKeyDown(key,mouseX,mouseY);
}
//Handles a special key released
void special_key_released(int key, int mouseX, int mouseY)
{
	dis->specialKeyUp(key,mouseX,mouseY);
}

#ifndef MAIN
#define MAIN

int main(int argc, char **argv) 
{
	srand(time(NULL));
	startInternet();
	dis=new baseForm(argc,argv,
				&display,
				&keyboard,
				&mouseClick,
				&mouseMove,
				&key_up,
				&special_key_pressed,
				&special_key_released
			);
	glRenderBegin();
	closeInternet();
	return 0;
}

#endif

#endif