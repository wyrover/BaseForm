//Primary author: Jonathan Bedard
//Confirmed working: 1/11/2015

#ifndef BASEFORM_CPP
#define BASEFORM_CPP

#include "baseForm.h"
#include "glLibrary.h"
#include "propertiesForm.h"
#include "USBGenerationForm.h"
#include "inetForm.h"
#include "basePopUps.h"

//Event Functions--------------------------------------------------------------------------------------------------------------------------

	//Raised when a button is clicked
	static void button_event_base_form(glElement* input, void* source)
	{
		baseForm* master = (baseForm*) source;
		master->button_pressed(input);
	}
	//Raised when a button is pressed
	static void button_down_base_form(glElement* input, void* source)
	{
		baseForm* master = (baseForm*) source;
		
		
		//Up/down/left/right
		if(input==master->getUpElm())
			master->getController()->getControls()->setForward(true);
		if(input==master->getDownElm())
			master->getController()->getControls()->setBackward(true);
		if(input==master->getLeftElm())
			master->getController()->getControls()->setLeft(true);
		if(input==master->getRightElm())
			master->getController()->getControls()->setRight(true);
	}
	//Raised when a button is lifted
	static void button_up_base_form(glElement* input, void* source)
	{
		baseForm* master = (baseForm*) source;
		
		//Up/down/left/right
		if(input==master->getUpElm())
			master->getController()->getControls()->setForward(false);
		if(input==master->getDownElm())
			master->getController()->getControls()->setBackward(false);
		if(input==master->getLeftElm())
			master->getController()->getControls()->setLeft(false);
		if(input==master->getRightElm())
			master->getController()->getControls()->setRight(false);
	}

//Constructor/Destructors------------------------------------------------------------------------------------------------------------------

//Default constructor
	baseForm::baseForm(int argv, char ** addr,
			void (*dispFunc)(),void (*keyBoard)(unsigned char, int, int),void (*mouse)(int,int,int,int),
			void (*mouseMove)(int,int),void (*kU)(unsigned char, int, int),
			void (*spc_down)(int,int,int),void (*spc_up)(int,int,int)):
		glForm(argv, addr, 800, 600, (char*)"Base Form",*dispFunc,*keyBoard,*mouse,*mouseMove,*kU,*spc_up,*spc_down)
	{
		//Set global colors
		col_red.red = 1;
		col_red.alpha = .7f;
		
		col_yell.alpha = .7f;
		col_yell.red = .9f;
		col_yell.green = .9f;
		
		col_green.green = 1;
		col_green.alpha = .7f;
		
		col_black.red = 0;
		col_black.green = 0;
		col_black.blue = 0;
		col_black.alpha = 1;

		col_haze_black.red = 0;
		col_haze_black.green = 0;
		col_haze_black.blue = 0;
		col_haze_black.alpha = .7f;

		col_white.red = 1;
		col_white.green = 1;
		col_white.blue = 1;
		col_white.alpha = 1;

		//Set title
		menu_title_array = new char[7+myInfo.getName().length()];
		menu_title_array[0] = 'B';
		menu_title_array[1] = 'a';
		menu_title_array[2] = 's';
		menu_title_array[3] = 'e';
		menu_title_array[4] = ':';
		menu_title_array[5] = ' ';
		int cnt = 0;
		while(cnt<myInfo.getName().length())
		{
			menu_title_array[5+cnt] = myInfo.getName().at(cnt);
			cnt++;
		}
		menu_title_array[5+cnt] = '\0';
		setTitle(menu_title_array);
		
		//Menu bar
		list <nestedButtons*>* menu_button_list = new list <nestedButtons*>();
		nestedButtons* menu_button;
		nestedButtons* tempButton;
		
		//File Section
		menu_button = new nestedButtons();
		menu_button->btn.setText((char*) "File");

		menu_button ->nested_list = new list <nestedButtons*>();

		tempButton = new nestedButtons();
		tempButton->btn.setText((char*) "Properties");
		barProperties = &tempButton->btn;
		barProperties->setClickEvent(&button_event_base_form);
		menu_button->nested_list->push_back(tempButton);

		tempButton = new nestedButtons();
		tempButton->btn.setText((char*) "USB Generation");
		barUSBGen = &tempButton->btn;
		barUSBGen->setClickEvent(&button_event_base_form);
		menu_button->nested_list->push_back(tempButton);

		tempButton = new nestedButtons();
		tempButton->btn.setText((char*) "Exit");
		barExit = &tempButton->btn;
		barExit->setClickEvent(&button_event_base_form);
		menu_button->nested_list->push_back(tempButton);
		menu_button_list->push_back(menu_button);

		//File Section
		menu_button = new nestedButtons();
		menu_button->btn.setText((char*) "Connections");

		menu_button ->nested_list = new list <nestedButtons*>();

		tempButton = new nestedButtons();
		tempButton->btn.setText((char*) "Internet");
		barInet = &tempButton->btn;
		barInet->setClickEvent(&button_event_base_form);
		menu_button->nested_list->push_back(tempButton);

		menu_button_list->push_back(menu_button);
		

		menu_bar.setNestedButtons(menu_button_list);
		pushElement(&menu_bar);
		addMouseListener(&menu_bar);
		menu_bar.setHidden(false);
		
		//Initialize connection control labels
		lblStaticIP.setText((char*) "My IP:");
		pushElement(&lblStaticIP);
		pushElement(&lblIP);

		lblLAN.setText((char*) "LAN Link:");
		pushElement(&lblLAN);
		
		ltLAN.setWidth(25);
		ltLAN.setHeight(25);
		pushElement(&ltLAN);

		lblServer.setText((char*)"Server Link:");
		pushElement(&lblServer);
		
		ltServer.setWidth(25);
		ltServer.setHeight(25);
		pushElement(&ltServer);
		
		//Initialize conneciton labels
		lblStaticConName.setText((char*) "Connected to:");
		pushElement(&lblStaticConName);
		lblDynamicConName.setText((char*) "Nothing");
		pushElement(&lblDynamicConName);

		//Initialize control interface
		if(myInfo.getControls()->getOn())
		{
			btnPower.setText((char*) "Power On");
			btnPower.setColor(col_green);
			btnPower.setTextColor(col_black);
		}
		else
		{
			btnPower.setText((char*) "Power Off");
			btnPower.setColor(col_red);
			btnPower.setTextColor(col_white);
		}
		btnPower.setClickEvent(&button_event_base_form);
		pushElement(&btnPower);
		addMouseListener(&btnPower);

		//Initialize picture button
		btnPicture.setText((char*) "Picture");
		btnPicture.setClickEvent(&button_event_base_form);
		pushElement(&btnPicture);
		addMouseListener(&btnPicture);
		
		elUp.setWidth(35);
		elUp.setHeight(35);
		elUp.setPressedEvent(&button_down_base_form);
		elUp.setReleaseEvent(&button_up_base_form);
		pushElement(&elUp);
		addMouseListener(&elUp);
		
		elLeft.setWidth(35);
		elLeft.setHeight(35);
		elLeft.setPressedEvent(&button_down_base_form);
		elLeft.setReleaseEvent(&button_up_base_form);
		pushElement(&elLeft);
		addMouseListener(&elLeft);
		
		elRight.setWidth(35);
		elRight.setHeight(35);
		elRight.setPressedEvent(&button_down_base_form);
		elRight.setReleaseEvent(&button_up_base_form);
		pushElement(&elRight);
		addMouseListener(&elRight);
		
		elDown.setWidth(35);
		elDown.setHeight(35);
		elDown.setPressedEvent(&button_down_base_form);
		elDown.setReleaseEvent(&button_up_base_form);
		pushElement(&elDown);
		addMouseListener(&elDown);

		formResize();

		setTraverse(false);
		//Test name
		message_status = 0;
		if(myInfo.getName() == "NULL")
			setPopUp((glForm*) new badNamePopUp(this, (char*) "You are using the default name."));
	}
	//Default destructor
	baseForm::~baseForm()
	{
		delete[] menu_title_array;
	}

//Update Functions-------------------------------------------------------------------------------------------------------------------------

	//Updates the form
	void baseForm::virtual_update()
	{
		//Tests to see for spawning a new form based on messages
		if(next_form == NULL)
		{
			//Message status 1, spawn properties form
			if(message_status == 1)
			{
				message_status = 0;
				setNextForm((glForm*) new propertiesForm(this, &myInfo));
			}
		}

		//Sets IP label
		lblIP.setText(myInfo.getMyAddress()->getIPString());
		
		//Sets color choices
		ID_module* mdl = myInfo.getInetConnections()->getModule();

		if(myInfo.getInetConnections()==NULL || !myInfo.getInetConnections()->isLANAuthentic())
			ltLAN.setColor(col_red);
		else if(mdl != NULL && mdl->getConnectionStatus()==1)
			ltLAN.setColor(col_green);
		else
			ltLAN.setColor(col_yell);
		
		ltServer.setColor(col_red);

		if(mdl==NULL)
			lblDynamicConName.setText((char*)"Nothing");
		else
			lblDynamicConName.setText((char*)mdl->name_ID.c_str());
		myInfo.getInetConnections()->returnModule();
		
		//Control structure
		if(myInfo.getControls()->getForward())
			elUp.setColor(col_red);
		else
			elUp.setColor(col_haze_black);
		
		if(myInfo.getControls()->getLeft())
			elLeft.setColor(col_red);
		else
			elLeft.setColor(col_haze_black);
		
		if(myInfo.getControls()->getRight())
			elRight.setColor(col_red);
		else
			elRight.setColor(col_haze_black);
		
		if(myInfo.getControls()->getBackward())
			elDown.setColor(col_red);
		else
			elDown.setColor(col_haze_black);
	}
	//Send a message to the form
	void baseForm::sendMessage(int x)
	{
		message_status = x;
	}


//Action Functions-------------------------------------------------------------------------------------------------------------------------

	//Triggers when the form is resized
	void baseForm::virtualFormResize()
	{
		//Limits the size of the form
		if(height<BASEFORM_MIN_HEIGHT)
			forceSize(width, BASEFORM_MIN_HEIGHT);
		if(width<BASEFORM_MIN_WIDTH)
			forceSize(BASEFORM_MIN_WIDTH,height);
		menu_bar.force_resize();

		//Sets the label positions
		lblStaticIP.setX(20);
		lblStaticIP.setY(height-75);
		lblIP.setX(75);
		lblIP.setY(height-75);

		lblLAN.setX(265);
		lblLAN.setY(height-75);
		ltLAN.setX(350);
		ltLAN.setY(height-75);
	
		lblServer.setX(400);
		lblServer.setY(height-75);
		ltServer.setX(505);
		ltServer.setY(height-75);

		lblStaticConName.setX(265);
		lblStaticConName.setY(height-125);
		lblDynamicConName.setX(390);
		lblDynamicConName.setY(height-125);
		
		//Sets the control positions
		btnPower.setX(20);
		btnPower.setY(height-130);

		btnPicture.setX(width-200);
		btnPicture.setY(20);
		
		elUp.setX(65);
		elUp.setY(65);
		
		elDown.setX(65);
		elDown.setY(20);
		
		elLeft.setX(20);
		elLeft.setY(20);
		
		elRight.setX(110);
		elRight.setY(20);
	}
	//Called when any button on the form is pressed
	void baseForm::button_pressed(glElement* source)
	{
		//Power button
		if(source == &btnPower)
		{
			glButton* re_cast = (glButton*) source;
			if(myInfo.getControls()->getOn())
			{
				myInfo.getControls()->setOn(false);
				re_cast->setText((char*) "Power Off");
				re_cast->setColor(col_red);
				re_cast->setTextColor(col_white);
			}
			else
			{
				myInfo.getControls()->setOn(true);
				re_cast->setText((char*) "Power On");
				re_cast->setColor(col_green);
				re_cast->setTextColor(col_black);
			}
			return;
		}

		//Picture button
		if(source == &btnPicture)
		{
			myInfo.getControls()->takePicture();
			return;
		}

		//Properties form
		if(source==barProperties)
		{
			menu_bar.setClosed();
			setNextForm((glForm*) new propertiesForm(this, &myInfo));
		}

		//USB Generation Form
		if(source==barUSBGen)
		{
			menu_bar.setClosed();
			setNextForm((glForm*) new USBGenerationForm(this, &myInfo));
		}

		//Exit
		if(source == barExit)
		{
			menu_bar.setClosed();
			exit_form();
		}

		//Inet form
		if(source==barInet)
		{
			menu_bar.setClosed();
			setNextForm((glForm*) new inetForm(this, myInfo.getInetConnections()));
		}
	}
	//Draws the wheels and power bars
	void baseForm::virtual_draw()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable( GL_BLEND );

		int power = (int) myInfo.getControls()->getPower();
		//Reverse bar
		glColor4f(.6f, .6f, .6f, col_red.alpha);
		glBegin(GL_QUADS);
		glVertex2f(160,120);
		if(power<0)
		{
			glVertex2f(160, 20-power);
			glVertex2f(175, 20-power);
		}
		else
		{
			glVertex2f(160, 20);
			glVertex2f(175, 20);
		}
		glVertex2f(175, 120);
		glEnd();

		//Reverse bar (colored)
		glColor4f(col_red.red, col_red.green, col_red.blue, col_red.alpha);
		glBegin(GL_QUADS);
		glVertex2f(160,20);
		if(power<0)
		{
			glVertex2f(160, 20-power);
			glVertex2f(175, 20-power);
		}
		else
		{
			glVertex2f(160, 20);
			glVertex2f(175, 20);
		}
		glVertex2f(175, 20);
		glEnd();

		//Forward bar
		glColor4f(.6f, .6f, .6f, col_red.alpha);
		glBegin(GL_QUADS);
		glVertex2f(185,120);
		if(power>0)
		{
			glVertex2f(185, 20+power);
			glVertex2f(200, 20+power);
		}
		else
		{
			glVertex2f(185, 20);
			glVertex2f(200, 20);
		}
		glVertex2f(200, 120);
		glEnd();

		//Forward bar (colored)
		glColor4f(col_green.red, col_green.green, col_green.blue, col_green.alpha);
		glBegin(GL_QUADS);
		glVertex2f(185,20);
		if(power>0)
		{
			glVertex2f(185, 20+power);
			glVertex2f(200, 20+power);
		}
		else
		{
			glVertex2f(185, 20);
			glVertex2f(200, 20);
		}
		glVertex2f(200, 20);
		glEnd();

		int angle = (int) myInfo.getControls()->getAngle();

		//Draw wheels
		glColor4f(col_haze_black.red,col_haze_black.green, col_haze_black.blue, col_haze_black.alpha);

		glPushMatrix();
		glTranslatef(250, 70, 0.0);
		glRotatef(-angle, 0.0, 0.0, 1.0);

		glBegin(GL_QUADS);
		glVertex2f(-10,-30);
		glVertex2f(10,-30);
		glVertex2f(10,30);
		glVertex2f(-10,30);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(320, 70, 0.0);
		glRotatef(-angle, 0.0, 0.0, 1.0);

		glBegin(GL_QUADS);
		glVertex2f(-10,-30);
		glVertex2f(10,-30);
		glVertex2f(10,30);
		glVertex2f(-10,30);
		glEnd();
		glPopMatrix();
	}

//Action Functions-------------------------------------------------------------------------------------------------------------------------
	
	//Triggers when a key is typed
	void baseForm::virtualKeyboard(unsigned char key, int mousePositionX, int mousePositionY)
	{
		//Up
		if(key == 'w' || key == 'W')
		      myInfo.getControls()->setForward(true);
		//Down
		if(key == 's' || key == 'S')
		      myInfo.getControls()->setBackward(true);
		//Left
		if(key == 'a' || key == 'A')
		      myInfo.getControls()->setLeft(true);
		//Right
		if(key == 'd' || key == 'D')
		      myInfo.getControls()->setRight(true);
	}
	//Triggers when a key is released
	void baseForm::virtualKeyUp(unsigned char key, int mousePositionX, int mousePositionY)
	{
		//Up
		if(key == 'w' || key == 'W')
		      myInfo.getControls()->setForward(false);
		//Down
		if(key == 's' || key == 'S')
		      myInfo.getControls()->setBackward(false);
		//Left
		if(key == 'a' || key == 'A')
		      myInfo.getControls()->setLeft(false);
		//Right
		if(key == 'd' || key == 'D')
		      myInfo.getControls()->setRight(false);
	}
	//The special key down virtual function
	void baseForm::virtualSpecialKeyDown(int key, int mousePositionX, int mousePositionY)
	{
		//Up
		if(key == KEY_UP)
		      myInfo.getControls()->setForward(true);
		//Down
		if(key == KEY_DOWN)
		      myInfo.getControls()->setBackward(true);
		//Left
		if(key == KEY_LEFT)
		      myInfo.getControls()->setLeft(true);
		//Right
		if(key == KEY_RIGHT)
		      myInfo.getControls()->setRight(true);
	}
	//The special key up virtual function
	void baseForm::virtualSpecialKeyUp(int key, int mousePositionX, int mousePositionY)
	{
		//Up
		if(key == KEY_UP)
		      myInfo.getControls()->setForward(false);
		//Down
		if(key == KEY_DOWN)
		      myInfo.getControls()->setBackward(false);
		//Left
		if(key == KEY_LEFT)
		      myInfo.getControls()->setLeft(false);
		//Right
		if(key == KEY_RIGHT)
		      myInfo.getControls()->setRight(false);
	}

//Get Functions----------------------------------------------------------------------------------------------------------------------------

	//Returns a pointer to the base controller
	BaseController* baseForm::getController()
	{
		return &myInfo;
	}
	//Returns pointers to arrow key identifiers
	glElement* baseForm::getUpElm()
	{
		return &elUp;
	}
	glElement* baseForm::getDownElm()
	{
		return &elDown;
	}
	glElement* baseForm::getLeftElm()
	{
		return &elLeft;
	}
	glElement* baseForm::getRightElm()
	{
		return &elRight;
	}

#endif