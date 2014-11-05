//Primary author: Jonathan Bedard
//Confirmed working: 9/30/2014

#ifndef BASEPOPUPS_H
#define BASEPOPUPS_H

#include "glAdvanced.h"
//#include "baseForm.h"

class badNamePopUp:
	public glPopUp
{
protected:
	glLabel lblLine2;
	glLabel lblLine3;
	glLabel lblLine4;

	glButton btnYes;
	glButton btnNo;

public:
	badNamePopUp(glForm* f, char* str);
	void virtual virtualFormResize();
	void button_pressed(glElement* x);
};

class newNamePopUp:
	public glPopUp
{
protected:
	glLabel lblLine2;
	glLabel lblLine3;
	glLabel lblLine4;

	glButton btnYes;
	glButton btnNo;

public:
	newNamePopUp(glForm* f, char* str);
	void virtual virtualFormResize();
	void button_pressed(glElement* x);
};

#endif