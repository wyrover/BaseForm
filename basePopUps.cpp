//Primary author: Jonathan Bedard
//Confirmed working: 9/30/2014

#ifndef BASEPOPUPS_CPP
#define BASEPOPUPS_CPP

#include "basePopUps.h"
#include "baseForm.h"

//Bad Name Pop-Up---------------------------------------------------------------------------------------------------------------

	//Static function op up
	static void button_event_bad_name(glElement* input, void* source)
	{
		badNamePopUp* master = (badNamePopUp*) source;
		master->button_pressed(input);
	}
	//General initializer
	badNamePopUp::badNamePopUp(glForm* f, char* str):
		glPopUp(f,str)
	{
		lblLine2.setText((char*) "Your name will cause errors and");
		lblLine3.setText((char*) "unexpected functionality.");
		lblLine4.setText((char*) "Would you like to change it now?");

		pushElement(&lblLine2);
		pushElement(&lblLine3);
		pushElement(&lblLine4);

		btnYes.setText((char*) "Yes");
		btnYes.setClickEvent(&button_event_bad_name);
		btnNo.setText((char*) "No");
		btnNo.setClickEvent(&button_event_bad_name);

		pushElement(&btnYes);
		addMouseListener(&btnYes);
		pushElement(&btnNo);
		addMouseListener(&btnNo);

		virtualFormResize();
	}
	//Moves around objects during a resize
	void badNamePopUp::virtualFormResize()
	{
		//Limits the size of the form
		if(height<BASEFORM_MIN_HEIGHT)
			forceSize(width, BASEFORM_MIN_HEIGHT);
		if(width<BASEFORM_MIN_WIDTH)
			forceSize(BASEFORM_MIN_WIDTH,height);


		lblLine2.setX(lblStatus.getX());
		lblLine2.setY(lblStatus.getY()-25);
		lblLine3.setX(lblStatus.getX());
		lblLine3.setY(lblStatus.getY()-50);
		lblLine4.setX(lblStatus.getX());
		lblLine4.setY(lblStatus.getY()-85);

		btnYes.setX(width/4+20);
		btnYes.setY(height/4+50);
		btnNo.setX(3*width/4-170);
		btnNo.setY(height/4+50);
	}
	//Triggered when a button is pressed
	void badNamePopUp::button_pressed(glElement* x)
	{
		//Yes
		if(x==&btnYes)
		{
			previous_form->setPopUp(NULL);
			previous_form->sendMessage(1);
		}

		//No
		if(x==&btnNo)
			previous_form->setPopUp(NULL);
	}

//New Name Pop-Up---------------------------------------------------------------------------------------------------------------

	//Static function op up
	static void button_event_new_name(glElement* input, void* source)
	{
		newNamePopUp* master = (newNamePopUp*) source;
		master->button_pressed(input);
	}
	//General initializer
	newNamePopUp::newNamePopUp(glForm* f, char* str):
		glPopUp(f,str)
	{
		lblLine2.setText((char*) "Your new changes will not become");
		lblLine3.setText((char*) "active until an application restart.");
		lblLine4.setText((char*) "Would you like to shutdown?");

		pushElement(&lblLine2);
		pushElement(&lblLine3);
		pushElement(&lblLine4);

		btnYes.setText((char*) "Yes");
		btnYes.setClickEvent(&button_event_new_name);
		btnNo.setText((char*) "No");
		btnNo.setClickEvent(&button_event_new_name);

		pushElement(&btnYes);
		addMouseListener(&btnYes);
		pushElement(&btnNo);
		addMouseListener(&btnNo);

		virtualFormResize();
	}
	//Moves around objects during a resize
	void newNamePopUp::virtualFormResize()
	{
		//Limits the size of the form
		if(height<BASEFORM_MIN_HEIGHT)
			forceSize(width, BASEFORM_MIN_HEIGHT);
		if(width<BASEFORM_MIN_WIDTH)
			forceSize(BASEFORM_MIN_WIDTH,height);


		lblLine2.setX(lblStatus.getX());
		lblLine2.setY(lblStatus.getY()-25);
		lblLine3.setX(lblStatus.getX());
		lblLine3.setY(lblStatus.getY()-50);
		lblLine4.setX(lblStatus.getX());
		lblLine4.setY(lblStatus.getY()-85);

		btnYes.setX(width/4+20);
		btnYes.setY(height/4+50);
		btnNo.setX(3*width/4-170);
		btnNo.setY(height/4+50);
	}
	//Triggered when a button is pressed
	void newNamePopUp::button_pressed(glElement* x)
	{
		//Yes
		if(x==&btnYes)
		{
			cout<<"Click"<<endl;
			previous_form->setPopUp(NULL);
			exit(EXIT_SUCCESS);
		}

		//No
		if(x==&btnNo)
			previous_form->setPopUp(NULL);
	}

#endif