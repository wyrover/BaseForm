//Primary author: Jonathan Bedard
//Confirmed working: 11/4/2014

#ifndef INETFORM_CPP

#define INETFORM_CPP

#include "inetForm.h"
#include "glLibrary.h"
#include "WifiRC.h"
#include "baseForm.h"
	
//Constructor/Destructor------------------------------------------------------------------------

	//Raised when a button is clicked
	static void button_event_properties_form(glElement* input, void* source)
	{
		inetForm* master = (inetForm*) source;
		master->button_pressed(input);
	}
	//Constructor
	inetForm::inetForm(glForm* parent, ConnectionManager* i):
		glForm(parent)
	{
		myInfo = i;
		btn_list = NULL;

		//Set global colors
		col_red.red = 1;
		col_red.alpha = .7f;
		
		col_yell.alpha = .7f;
		col_yell.red = .9f;
		col_yell.green = .9f;
		
		col_green.green = 1;
		col_green.alpha = .7f;

		col_blue.blue = 1;
		col_blue.alpha = .7f;
		
		col_black.red = 0;
		col_black.green = 0;
		col_black.blue = 0;
		col_black.alpha = 1;

		col_white.red = 1;
		col_white.green = 1;
		col_white.blue = 1;
		col_white.alpha = 1;

		col_grey = btnNewKeys.getColor();
		col_clicked = btnNewKeys.getClickedColor();

		col_lite_grey.red = .8f;
		col_lite_grey.green = .8f;
		col_lite_grey.blue = .8f;
		col_lite_grey.alpha = .7f;

		//Internet power button
		internet_state = myInfo->getActive();
		if(myInfo->getActive())
		{
			btnInternetState.setText((char*) "Internet Active");
			btnInternetState.setColor(col_green);
			btnInternetState.setTextColor(col_black);
		}
		else
		{
			btnInternetState.setText((char*) "Internet Off");
			btnInternetState.setColor(col_red);
			btnInternetState.setTextColor(col_white);
		}
		btnInternetState.setClickEvent(&button_event_properties_form);
		pushElement(&btnInternetState);
		addMouseListener(&btnInternetState);

		//Set server UI
		lblServer.setText((char*)"Server Status:");
		pushElement(&lblServer);
		ltServer.setHeight(35);
		ltServer.setWidth(35);
		pushElement(&ltServer);

		//Set crypto UI
		lblKeyStatus.setText((char*) "Key Structure:");
		pushElement(&lblKeyStatus);
		ltKey.setHeight(35);
		ltKey.setWidth(35);
		pushElement(&ltKey);
		btnNewKeys.setText((char*) "Generate Keys");
		btnNewKeys.setClickEvent(&button_event_properties_form);
		pushElement(&btnNewKeys);
		addMouseListener(&btnNewKeys);

		//Update public key function
		if(myInfo->getPublicKey()==NULL)
		{
			btnNewKeys.setColor(col_lite_grey);
			btnNewKeys.setClickedColor(col_lite_grey);
		}
		else
		{
			btnNewKeys.setColor(col_grey);
			btnNewKeys.setClickedColor(col_clicked);
		}

		//Set IP title
		lblIPTitle.setText((char*)"IP Address List:");
		pushElement(&lblIPTitle);
		lblIPStatus.setText((char*)"Status");
		pushElement(&lblIPStatus);

		//Set back button
		btnBack.setText((char*) "<- Back");
		btnBack.setX(20);
		btnBack.setY(20);
		btnBack.setClickEvent(&button_event_properties_form);
		pushElement(&btnBack);
		addMouseListener(&btnBack);

		//Set IP Adding
		lblAddIP.setText((char*) "Add IP Address:");
		pushElement(&lblAddIP);

		last_IP = "NONE";
		valid_IP = false;
		tbxAddIP.pushBadInput(' ');
		tbxAddIP.pushBadInput(':');
		tbxAddIP.pushBadInput(';');
		tbxAddIP.setWidth(160);
		pushElement(&tbxAddIP);
		addMouseListener(&tbxAddIP);

		btnAddIP.setClickEvent(&button_event_properties_form);
		pushElement(&btnAddIP);
		addMouseListener(&btnAddIP);
		setbtnAddIPAttributes();

		//Resize the form
		btn_update_count = 0;
		virtualFormResize();
	}
	//Destructor
	inetForm::~inetForm()
	{
		//Save the form
		myInfo->save_addresses();
		//Delete button list
		if(btn_list != NULL)
		{
			for (list<inetForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
				delete (*it);
		}
	}

//Update Functions------------------------------------------------------------------------------

	//Triggers every time the form updates
	void inetForm::virtual_update()
	{
		//Limits the size of the form
		if(height<BASEFORM_MIN_HEIGHT)
			forceSize(width, BASEFORM_MIN_HEIGHT);
		if(width<BASEFORM_MIN_WIDTH)
			forceSize(BASEFORM_MIN_WIDTH,height);

		//Set the server flag
		if(myInfo->getServerOn())
			ltServer.setColor(col_green);
		else
			ltServer.setColor(col_red);

		//Check activity
		if(internet_state != myInfo->getActive())
		{
			internet_state = myInfo->getActive();
			btn_update_count = 0;

			//Update activity
			if(myInfo->getActive())
			{
				btnInternetState.setText((char*) "Internet Active");
				btnInternetState.setColor(col_green);
				btnInternetState.setTextColor(col_black);
			}
			else
			{
				btnInternetState.setText((char*) "Internet Off");
				btnInternetState.setColor(col_red);
				btnInternetState.setTextColor(col_white);
			}

			//Update public key function
			if(myInfo->getPublicKey()==NULL)
			{
				btnNewKeys.setColor(col_lite_grey);
				btnNewKeys.setClickedColor(col_lite_grey);
			}
			else
			{
				btnNewKeys.setColor(col_grey);
				btnNewKeys.setClickedColor(col_clicked);
			}
		}

		setbtnAddIPAttributes();

		//Public key check
		if(myInfo->getPublicKey()==NULL)
			ltKey.setColor(col_red);
		else
			ltKey.setColor(col_green);

		if(btn_update_count==0)
			updateButtonList();
		btn_update_count = (btn_update_count+1) % 200;
	}
	//Reloads the button list
	void inetForm::updateButtonList()
	{
		//Load in the first element
		AVLTree<address_module>* AVL_Tree_trc = myInfo->getClientData();
		AVL_Tree_trc->resetTraverse();
		AVLNode<address_module>* trace = AVL_Tree_trc->getFirst();
		inetForm_button_struct* temp;

		//Ensure that the AVL tree is not empty
		if(trace==NULL)
		{
			myInfo->returnClientData();
			return;
		}

		list<inetForm_button_struct*>* temp_list = new list<inetForm_button_struct*>;
		bool is_new = false;

		//Create a new list
		while(trace!=NULL)
		{
			temp = new inetForm_button_struct;
			temp->mdl = trace->getData();
			temp_list->push_back(temp);
			trace = trace->getNext();
		}
		//Return permission for the AVL tree
		myInfo->returnClientData();

		//Compare to list in memory
		if(btn_list == NULL)
			is_new = true;
		else
		{
			list<inetForm_button_struct*>::iterator it1 = btn_list->begin();
			list<inetForm_button_struct*>::iterator it2 = temp_list->begin();

			while(!is_new && it1 != btn_list->end() && it2 != temp_list->end())
			{
				if((*it1)->mdl != (*it2)->mdl)
					is_new = true;
				it1++;
				it2++;
			}
			if(!is_new  && (it1 != btn_list->end() || it2 != temp_list->end()))
				is_new = true;
		}
		
		//Deal with the new/old one
		if(is_new)
		{
			//Delete the old list
			if(btn_list != NULL)
			{
				for (list<inetForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
				{
					removeElement(&(*it)->lblIP);
					removeElement(&(*it)->elmStatus);
					removeElement(&(*it)->btnIsPolling);
					removeElement(&(*it)->btnDelete);
					delete (*it);
				}
				delete(btn_list);
			}
			btn_list = temp_list;
			for (list<inetForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
			{
				pushElement(&(*it)->lblIP);
				(*it)->elmStatus.setHeight(35);
				(*it)->elmStatus.setWidth(35);
				pushElement(&(*it)->elmStatus);
				(*it)->btnIsPolling.setPressedEvent(&button_event_properties_form);
				pushElement(&(*it)->btnIsPolling);
				addMouseListener(&(*it)->btnIsPolling);

				(*it)->del_flag = false;
				(*it)->btnDelete.setColor(col_red);
				(*it)->btnDelete.setTextColor(col_white);
				(*it)->btnDelete.setText((char*) "Delete");
				(*it)->btnDelete.setPressedEvent(&button_event_properties_form);
			}
		}
		else
		{
			//Delete the temp list
			for (list<inetForm_button_struct*>::iterator it = temp_list->begin(); it!=temp_list->end(); ++it)
				delete (*it);
			delete (temp_list);
		}

		int test = 0;
		//Set the button variables
		for (list<inetForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
		{
			(*it)->lblIP.setText((*it)->mdl->identifier.printAddress());

			(*it)->mdl->module_lock.acquire();

			//Status indicator
			if((*it)->mdl->is_me)
				(*it)->elmStatus.setColor(col_blue);
			else if(myInfo->getActive() && (*it)->mdl->is_polling && !(*it)->mdl->is_receiving)
				(*it)->elmStatus.setColor(col_yell);
			else if((*it)->mdl->is_receiving)
				(*it)->elmStatus.setColor(col_green);
			else
				(*it)->elmStatus.setColor(col_red);

			//Polling button
			if(!(*it)->mdl->is_polling)
			{
				(*it)->btnIsPolling.setText((char*) "Blocked");
				(*it)->btnIsPolling.setColor(col_black);
				(*it)->btnIsPolling.setTextColor(col_white);
			}
			else if(myInfo->getActive())
			{
				(*it)->btnIsPolling.setText((char*) "Polling");
				(*it)->btnIsPolling.setColor(col_green);
				(*it)->btnIsPolling.setTextColor(col_black);
			}
			else
			{
				(*it)->btnIsPolling.setText((char*) "Off");
				(*it)->btnIsPolling.setColor(col_red);
				(*it)->btnIsPolling.setTextColor(col_white);
			}

			//Delete button
			if((*it)->mdl->is_me || (*it)->mdl->is_polling)
			{
				if((*it)->del_flag)
				{
					removeElement(&(*it)->btnDelete);
					(*it)->del_flag = false;
				}
			}
			else
			{
				if(!(*it)->del_flag)
				{
					pushElement(&(*it)->btnDelete);
					addMouseListener(&(*it)->btnDelete);
					(*it)->del_flag = true;
				}
			}

			(*it)->mdl->module_lock.release();
		}

		//Resize the form
		if(is_new)
			formResize();
	}

//Action Functions------------------------------------------------------------------------------

	//Triggers whenever the form resizes
	void inetForm::virtualFormResize()
	{
		//Internet power button
		btnInternetState.setX(20);
		btnInternetState.setY(height-50);

		//Server UI
		lblServer.setX(180);
		lblServer.setY(height-50);
		ltServer.setX(310);
		ltServer.setY(height-50);

		//Crypto UI
		lblKeyStatus.setX(350);
		lblKeyStatus.setY(height-50);
		ltKey.setX(480);
		ltKey.setY(height-50);
		btnNewKeys.setX(560);
		btnNewKeys.setY(height-50);

		//IP Address adding
		lblAddIP.setX(20);
		lblAddIP.setY(height-90);
		tbxAddIP.setX(170);
		tbxAddIP.setY(height-95);
		btnAddIP.setX(350);
		btnAddIP.setY(height-95);

		//IP addresses
		lblIPTitle.setX(20);
		lblIPTitle.setY(height-140);
		lblIPStatus.setX(180);
		lblIPStatus.setY(height-140);

		if(btn_list!=NULL)
		{
			int x = 20;
			int y = height-200;
			for (list<inetForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
			{
				(*it)->lblIP.setX(x);
				(*it)->lblIP.setY(y);

				(*it)->elmStatus.setX(x+165);
				(*it)->elmStatus.setY(y);

				(*it)->btnIsPolling.setX(x+240);
				(*it)->btnIsPolling.setY(y);

				(*it)->btnDelete.setX(x+400);
				(*it)->btnDelete.setY(y);

				//Change the place location
				y = y-50;
				if(y<200)
					x = x + 280;
			}
		}
	}
	//Raised whenever a button is pushed on this form
	void inetForm::button_pressed(glElement* source)
	{
		//Turn internet on/off
		if(source==&btnInternetState)
		{
			if(!myInfo->getActive())
				myInfo->setActive(true);
			else
				myInfo->setActive(false);
			updateButtonList();
			return;
		}

		//Build new keys
		if(source==&btnNewKeys)
		{
			if(myInfo->getPublicKey() == NULL)
			{
				updateButtonList();
				return;
			}
			spawnThread(&generate_new_key,myInfo);
		}

		//Adds an IP address
		if(source==&btnAddIP)
		{
			setbtnAddIPAttributes();
			if(valid_IP)
			{
				tbxAddIP.setText((char*)"");
				IPAddress temp(last_IP);
				myInfo->addIPAddress(temp);
				setbtnAddIPAttributes();
			}
		}

		//Return to the previous form
		if(source==&btnBack)
		{
			return_form();
			return;
		}

		//Blocks certain IP addresses
		if(btn_list == NULL)
		{
			updateButtonList();
			return;
		}
		for (list<inetForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
		{
			if(source == &(*it)->btnIsPolling)
			{
				if((*it)->mdl->is_polling)
					(*it)->mdl->is_polling = false;
				else
					(*it)->mdl->is_polling = true;

				//Draw the button
				if(!(*it)->mdl->is_polling)
				{
					(*it)->btnIsPolling.setText((char*) "Blocked");
					(*it)->btnIsPolling.setColor(col_black);
					(*it)->btnIsPolling.setTextColor(col_white);
				}
				else if(myInfo->getActive())
				{
					(*it)->btnIsPolling.setText((char*) "Polling");
					(*it)->btnIsPolling.setColor(col_green);
					(*it)->btnIsPolling.setTextColor(col_black);
				}
				else
				{
					(*it)->btnIsPolling.setText((char*) "Off");
					(*it)->btnIsPolling.setColor(col_red);
					(*it)->btnIsPolling.setTextColor(col_white);
				}
				updateButtonList();
				return;
			}

			if(source == &(*it)->btnDelete && !(*it)->mdl->is_polling)
			{
				myInfo->deleteModule((*it)->mdl);
				return;
			}
		}
	}

//Private Funcitons-----------------------------------------------------------------------------

	//Sets the AddIP button's attributes
	void inetForm::setbtnAddIPAttributes()
	{
		string temp(tbxAddIP.getText());
		if(temp!=last_IP)
			last_IP = temp;
		else
			return;

		//Test IP
		int round = 0;
		int str_loc = 0;
		if(last_IP.length()<=0)
			valid_IP = false;
		else
			valid_IP = true;
		string hld;

		//Test first 3 pieces
		while(valid_IP && round<3)
		{
			hld = "";
			while(valid_IP&&last_IP.at(str_loc)!='.')
			{
				if(isdigit(last_IP.at(str_loc)))
					hld = hld + last_IP.at(str_loc);
				else
					valid_IP = false;
				str_loc++;

				if(str_loc>=last_IP.length())
					valid_IP = false;
			}
			if(valid_IP)
			{
				int x = atoi(hld.c_str());
				if(x>255 || x<0 || hld == "")
					valid_IP = false;
			}
			str_loc++;
			round++;
		}
		hld = "";
		while(valid_IP&&str_loc<last_IP.length())
		{
			if(isdigit(last_IP.at(str_loc)))
				hld = hld + last_IP.at(str_loc);
			else
				valid_IP = false;
			str_loc++;
		}
		if(valid_IP)
		{
			int x = atoi(hld.c_str());
			if(x>255 || x<0 || hld == "")
				valid_IP = false;
		}

		//Build add IP button based off of the above information
		if(valid_IP)
		{
			btnAddIP.setText((char*) "Add Address");
			btnAddIP.setColor(col_green);
			btnAddIP.setTextColor(col_black);
		}
		else
		{
			btnAddIP.setText((char*) "Cannot Add");
			btnAddIP.setColor(col_red);
			btnAddIP.setTextColor(col_white);
		}
	}

#endif