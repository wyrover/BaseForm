//Primary author: Jonathan Bedard
//Confirmed working: 10/28/2014

#ifndef PROPERTIESFORM_CPP
#define PROPERTIESFORM_CPP

#include "propertiesForm.h"
#include "glLibrary.h"
#include "WifiRC.h"
#include "basePopUps.h"
#include "baseForm.h"

//Constructor/Destructor------------------------------------------------------------------------

	//Raised when a button is clicked
	static void button_event_properties_form(glElement* input, void* source)
	{
		propertiesForm* master = (propertiesForm*) source;
		master->button_pressed(input);
	}
	//Constructor
	propertiesForm::propertiesForm(glForm* parent, BaseController* i):
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
		
		col_oran.alpha  = .7f;
		col_oran.red = .9f;
		col_oran.green = .6f;
		col_oran.blue = .2f;

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

		lblTitle.setText((char*) "Local Properties");
		lblTitle.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
		pushElement(&lblTitle);

		//Set name UI
		lblName.setText((char*) "Name:");
		pushElement(&lblName);


		tbxName.setText((char*) i->getName().c_str());
		tbxName.setClickEvent(&button_event_properties_form);
		tbxName.pushBadInput(' ');
		tbxName.setMaxChars(ID_SIZE);
		tbxName.setWidth(225);
		pushElement(&tbxName);
		addMouseListener(&tbxName);

		//Connection screening
		lblConScreen.setText((char*) "Screen Connections");
		lblConScreen.setWidth(180);
		lblConScreen.setCentered(true);
		pushElement(&lblConScreen);

		btnAutoSelect.setText((char*) AUTO_SELECT_UI);
		btnAutoSelect.setWidth(180);
		btnAutoSelect.setClickEvent(&button_event_properties_form);
		addMouseListener(&btnAutoSelect);
		pushElement(&btnAutoSelect);

		btnPreferedAuto.setText((char*) PREFFERED_AUTO_UI);
		btnPreferedAuto.setWidth(180);
		btnPreferedAuto.setClickEvent(&button_event_properties_form);
		addMouseListener(&btnPreferedAuto);
		pushElement(&btnPreferedAuto);

		btnListSelection.setText((char*) LIST_SELECTION_UI);
		btnListSelection.setWidth(180);
		btnListSelection.setClickEvent(&button_event_properties_form);
		addMouseListener(&btnListSelection);
		pushElement(&btnListSelection);

		btnPreferedListSelection.setText((char*) PREFFERED_LIST_UI);
		btnPreferedListSelection.setWidth(180);
		btnPreferedListSelection.setClickEvent(&button_event_properties_form);
		addMouseListener(&btnPreferedListSelection);
		pushElement(&btnPreferedListSelection);

		btnOneSelection.setText((char*) ONE_SELECTION_UI);
		btnOneSelection.setWidth(180);
		btnOneSelection.setClickEvent(&button_event_properties_form);
		addMouseListener(&btnOneSelection);
		pushElement(&btnOneSelection);

		//ID List Header
		lblIDList.setText((char*) "Identifier");
		pushElement(&lblIDList);
		lblLANind.setText((char*) "LAN");
		pushElement(&lblLANind);
		lblServerind.setText((char*) "Server");
		pushElement(&lblServerind);
		lblRadioind.setText((char*) "Radio");
		pushElement(&lblRadioind);
		btnDelete.setText((char*) "Delete");
		btnDelete.setWidth(130);

		btn_update_count = 0;

		//Set back button
		btnBack.setText((char*) "<- Back");
		btnBack.setX(20);
		btnBack.setY(20);
		btnBack.setClickEvent(&button_event_properties_form);
		pushElement(&btnBack);
		addMouseListener(&btnBack);
	}
	//Destructor
	propertiesForm::~propertiesForm()
	{
		//Delete button list
		if(btn_list != NULL)
		{
			for (list<propertiesForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
				delete (*it);
		}
	}

//Update Functions------------------------------------------------------------------------------

	//Triggers every time the form updates
	void propertiesForm::virtual_update()
	{
		//Limits the size of the form
		if(height<BASEFORM_MIN_HEIGHT)
			forceSize(width, BASEFORM_MIN_HEIGHT);
		if(width<BASEFORM_MIN_WIDTH)
			forceSize(BASEFORM_MIN_WIDTH,height);

		//Set the colors of all the buttons
		if(myInfo->getDicernType() == AUTO_SELECT)
		{
			btnAutoSelect.setColor(col_green);
			btnAutoSelect.setTextColor(col_black);
		}
		else
		{
			btnAutoSelect.setColor(col_red);
			btnAutoSelect.setTextColor(col_white);
		}
		if(myInfo->getDicernType() == PREFFERED_AUTO)
		{
			btnPreferedAuto.setColor(col_green);
			btnPreferedAuto.setTextColor(col_black);
		}
		else
		{
			btnPreferedAuto.setColor(col_red);
			btnPreferedAuto.setTextColor(col_white);
		}
		if(myInfo->getDicernType() == LIST_SELECTION)
		{
			btnListSelection.setColor(col_green);
			btnListSelection.setTextColor(col_black);
		}
		else
		{
			btnListSelection.setColor(col_red);
			btnListSelection.setTextColor(col_white);
		}
		if(myInfo->getDicernType() == PREFFERED_LIST)
		{
			btnPreferedListSelection.setColor(col_green);
			btnPreferedListSelection.setTextColor(col_black);
		}
		else
		{
			btnPreferedListSelection.setColor(col_red);
			btnPreferedListSelection.setTextColor(col_white);
		}
		if(myInfo->getDicernType() == ONE_SELECTION)
		{
			btnOneSelection.setColor(col_green);
			btnOneSelection.setTextColor(col_black);
		}
		else
		{
			btnOneSelection.setColor(col_red);
			btnOneSelection.setTextColor(col_white);
		}

		//Trigger indicators
		if(btn_list != NULL && !btn_list->empty())
		{
			for (list<propertiesForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
			{
				//Set button color
				if((*it)->mdl->getConnectionStatus() > 0)
				{
					(*it)->btnID.setColor(col_green);
					(*it)->btnID.setTextColor(col_black);
				}
				else
				{
					//No connection at all
					if((*it)->mdl->LANCounter == 0)
					{
						(*it)->btnID.setColor(col_red);
						(*it)->btnID.setTextColor(col_white);
					}
					else
					{
						//Check if the connection is approved on our side
						if(myInfo->getInetConnections()->willConnect((*it)->mdl))
							(*it)->btnID.setColor(col_yell);
						else
							(*it)->btnID.setColor(col_oran);
						(*it)->btnID.setTextColor(col_black);
					}
				}


				//Set LAN color
				if((*it)->mdl->addresses_ascociated.empty())
					(*it)->indLAN.setColor(col_red);
				else if((*it)->mdl->LANCounter)
					(*it)->indLAN.setColor(col_green);
				else
					(*it)->indLAN.setColor(col_yell);

				//Set server indicator color
				(*it)->indServer.setColor(col_red);

				//Set radio indicator color
				(*it)->indRadio.setColor(col_red);

				//Set type integer
				if((*it)->selected)
				{
					//Default choice
					if((*it)->mdl->choice_status == DEFAULT_CHOICE)
					{
						btnChoiceStatus.setText((char*)DEFAULT_CHOICE_UI);
						btnChoiceStatus.setColor(col_red);
						btnChoiceStatus.setTextColor(col_white);
					}
					//List choice
					else if((*it)->mdl->choice_status == LIST_CHOICE)
					{
						btnChoiceStatus.setText((char*)LIST_CHOICE_UI);
						btnChoiceStatus.setColor(col_green);
						btnChoiceStatus.setTextColor(col_black);
					}
					//Prefered choice
					else if((*it)->mdl->choice_status == PREFERED_CHOICE)
					{
						btnChoiceStatus.setText((char*)PREFERED_CHOICE_UI);
						btnChoiceStatus.setColor(col_blue);
						btnChoiceStatus.setTextColor(col_white);
					}
					//Blocked choice
					else
					{
						btnChoiceStatus.setText((char*)BLOCKED_CHOICE_UI);
						btnChoiceStatus.setColor(col_black);
						btnChoiceStatus.setTextColor(col_white);
					}

					//Set delete button details
					if((*it)->mdl->canDelete())
					{
						btnDelete.setColor(col_red);
						btnDelete.setTextColor(col_white);
						btnDelete.setClickedColor(btnBack.getClickedColor());
					}
					else
					{
						btnDelete.setColor(btnBack.getColor());
						btnDelete.setTextColor(btnBack.getTextColor());
						btnDelete.setClickedColor(btnBack.getColor());
					}
				}
			}
		}

		if(btn_update_count==0)
			updateButtonList();
		btn_update_count = (btn_update_count+1) % 200;
	}
	//Reloads the button list
	void propertiesForm::updateButtonList()
	{
		//Ensure that there is a connection manager
		if(myInfo->getInetConnections() == NULL)
			return;

		//Load in the first element
		AVLTree<ID_module>* AVL_Tree_trc = myInfo->getInetConnections()->getIDData();
		AVL_Tree_trc->resetTraverse();
		AVLNode<ID_module>* trace = AVL_Tree_trc->getFirst();
		propertiesForm_button_struct* temp;

		//Ensure that the AVL tree is not empty
		if(trace==NULL)
		{
			myInfo->getInetConnections()->returnIDData();
			return;
		}

		list<propertiesForm_button_struct*>* temp_list = new list<propertiesForm_button_struct*>;
		bool is_new = false;

		//Create a new list
		while(trace!=NULL)
		{
			temp = new propertiesForm_button_struct;
			temp->selected = false;
			temp->mdl = trace->getData();
			temp_list->push_back(temp);
			trace = trace->getNext();
		}
		//Return permissons for the tree
		myInfo->getInetConnections()->returnIDData();

		//Compare to list in memory
		if(btn_list == NULL)
			is_new = true;
		else
		{
			list<propertiesForm_button_struct*>::iterator it1 = btn_list->begin();
			list<propertiesForm_button_struct*>::iterator it2 = temp_list->begin();

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
				for (list<propertiesForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
				{
					removeElement(&(*it)->btnID);
					removeElement(&(*it)->indLAN);
					removeElement(&(*it)->indServer);
					removeElement(&(*it)->indRadio);
					delete (*it);
				}
				delete(btn_list);
			}
			btn_list = temp_list;
			for (list<propertiesForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
			{
				(*it)->btnID.setWidth(200);
				(*it)->btnID.setClickEvent(&button_event_properties_form);
				pushElement(&(*it)->btnID);
				addMouseListener(&(*it)->btnID);

				(*it)->indLAN.setWidth(35);
				(*it)->indLAN.setHeight(35);
				pushElement(&(*it)->indLAN);

				(*it)->indServer.setWidth(35);
				(*it)->indServer.setHeight(35);
				pushElement(&(*it)->indServer);

				(*it)->indRadio.setWidth(35);
				(*it)->indRadio.setHeight(35);
				pushElement(&(*it)->indRadio);
			}
		}
		else
		{
			//Delete the temp list
			for (list<propertiesForm_button_struct*>::iterator it = temp_list->begin(); it!=temp_list->end(); ++it)
				delete (*it);
			delete (temp_list);
		}

		int test = 0;
		//Set the button variables
		for (list<propertiesForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
		{
			(*it)->btnID.setText((char*) (*it)->mdl->name_ID.c_str());
		}

		//Resize the form
		if(is_new)
			formResize();
	}

//Action Functions------------------------------------------------------------------------------

	//Triggers whenever the form resizes
	void propertiesForm::virtualFormResize()
	{
		lblTitle.setX(50);
		lblTitle.setY(height-50);
		lblName.setX(20);
		lblName.setY(height-100);
		tbxName.setX(80);
		tbxName.setY(height-100);

		//Connection screening
		lblConScreen.setX(width - 200);
		lblConScreen.setY(height - 50);
		btnAutoSelect.setX(width - 200);
		btnAutoSelect.setY(height-100);
		btnPreferedAuto.setX(width - 200);
		btnPreferedAuto.setY(height-150);
		btnListSelection.setX(width - 200);
		btnListSelection.setY(height-200);
		btnPreferedListSelection.setX(width - 200);
		btnPreferedListSelection.setY(height-250);
		btnOneSelection.setX(width - 200);
		btnOneSelection.setY(height-300);

		//ID List locations
		lblIDList.setX(30);
		lblIDList.setY(height-135);
		lblLANind.setX(240);
		lblLANind.setY(height-135);
		lblServerind.setX(300);
		lblServerind.setY(height-135);
		lblRadioind.setX(370);
		lblRadioind.setY(height-135);

		if(btn_list!=NULL)
		{
			int x = 20;
			int y = height-200;
			for (list<propertiesForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
			{
				(*it)->btnID.setX(x);
				(*it)->btnID.setY(y);

				(*it)->indLAN.setX(x+225);
				(*it)->indLAN.setY(y+2);

				(*it)->indServer.setX(x+295);
				(*it)->indServer.setY(y+2);

				(*it)->indRadio.setX(x+365);
				(*it)->indRadio.setY(y+2);

				if((*it)->selected)
				{
					y = y - 50;
					btnChoiceStatus.setX(x+100);
					btnChoiceStatus.setY(y);
					btnDelete.setX(x+270);
					btnDelete.setY(y);
				}

				//Change the place location
				y = y-50;
			}
		}
	}
	//Raised whenever a button is pushed on this form
	void propertiesForm::button_pressed(glElement* source)
	{
		//Return to the previous form
		if(source==&btnBack)
		{
			return_form();
			return;
		}

		//Check button list actions
		if(source==&btnChoiceStatus)
		{
			if(btn_list == NULL)
				return;

			propertiesForm_button_struct* sel = NULL;
			list<propertiesForm_button_struct*>::iterator it;
			it = btn_list->begin();
			while( it!=btn_list->end() && sel==NULL)
			{
				if((*it)->selected)
					sel = (*it);
				it++;
			}

			bool preferedFlag = false;
			if(sel!=NULL)
			{
				sel->mdl->choice_status = (sel->mdl->choice_status + 1) % 4;
				if(sel->mdl->choice_status == PREFERED_CHOICE)
					preferedFlag = true;
				myInfo->getInetConnections()->save_IDs();
			}
			it = btn_list->begin();
			while(it!=btn_list->end()&&preferedFlag)
			{
				if((*it)!=sel && (*it)->mdl->choice_status == PREFERED_CHOICE)
					(*it)->mdl->choice_status = LIST_CHOICE;
				it++;
			}
			return;
		}

		//Delete ID
		if(source==&btnDelete)
		{
			propertiesForm_button_struct* sel = NULL;
			list<propertiesForm_button_struct*>::iterator it;
			it = btn_list->begin();
			while( it!=btn_list->end() && sel==NULL)
			{
				if((*it)->selected)
					sel = (*it);
				it++;
			}
			if(sel==NULL || !sel->mdl->canDelete())
				return;

			myInfo->getInetConnections()->deleteIDModule(sel->mdl);
		}

		bool was_added = false;
		//Check the button list
		if(btn_list!=NULL)
		{
			for (list<propertiesForm_button_struct*>::iterator it = btn_list->begin(); it!=btn_list->end(); ++it)
			{
				if(source == &(*it)->btnID && !(*it)->selected)
				{
					if(!(*it)->selected)
					{
						btnChoiceStatus.setClickEvent(&button_event_properties_form);
						pushElement(&btnChoiceStatus);
						addMouseListener(&btnChoiceStatus);
						btnDelete.setClickEvent(&button_event_properties_form);
						pushElement(&btnDelete);
						addMouseListener(&btnDelete);
						was_added=true;
					}
					(*it)->selected = true;
				}
				else
				{
					if((*it)->selected && !was_added)
					{
						myInfo->getInetConnections()->save_IDs();
						removeElement(&btnChoiceStatus);
						removeElement(&btnDelete);
					}
					(*it)->selected = false;
				}
			}
		}

		//Save name change
		if(source==&tbxName)
		{
			if(tbxName.getText()!=myInfo->getName())
			{
				myInfo->setName(tbxName.getText());
				myInfo->genSave();
				setPopUp((glForm*) new newNamePopUp(this, (char*) "You have changed your name!"));
			}
			return;
		}

		//Check all connection types
		if(source==&btnAutoSelect)
			myInfo->setDicernType(AUTO_SELECT);
		else if(source==&btnPreferedAuto)
			myInfo->setDicernType(PREFFERED_AUTO);
		else if(source==&btnListSelection)
			myInfo->setDicernType(LIST_SELECTION);
		else if(source==&btnPreferedListSelection)
			myInfo->setDicernType(PREFFERED_LIST);
		else if(source==&btnOneSelection)
			myInfo->setDicernType(ONE_SELECTION);

		formResize();
	}

#endif