/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/
#include "Menu.h"

#include "PVL.h"
#include <iostream>
using namespace std;

Menu::Menu(const string& Title, Menu* parent, int command)
{
	for (int i = 0; i < MAX_SUB_MENUS; i++)
		this->child[i] = NULL;

	this->selected = 0;
	this->title = Title;
	this->parent = parent;
	this->command = command;
}

void draw_menu(Menu* menu,Menu*& current_menu, int level)
{
	//recursion
	if (menu == current_menu)
	{
		//deepest level of the menu, highlights the selected also the stop condition
		for (int i = 0; i < MAX_SUB_MENUS && menu->child[i] != NULL; i++)
		{
			if (i == menu->selected)
				cout << pvl::padString("", 22 + level * 5, '-');

			cout << endl << pvl::padString("", level * 5, ' ') << "> " << menu->child[i]->title << endl;

			if (i == menu->selected)
				cout << pvl::padString("", 22 + level * 5, '-');

		}
	}
	else
	{
		for (int i = 0; i < MAX_SUB_MENUS && menu->child[i] != NULL; i++)
		{
			cout << endl << pvl::padString("", level * 5, ' ') << "> " << menu->child[i]->title << endl;
			if (menu->selected == i) break;
		}

		draw_menu(menu->child[menu->selected], current_menu, level + 1);

		for (int i = menu->selected + 1; i < MAX_SUB_MENUS && menu->child[i] != NULL; i++)
			cout << endl << pvl::padString("", level * 5, ' ') << "> " << menu->child[i]->title << endl;
	}

}

void call_draw_menu(Menu* main_menu,Menu*& current_menu)
{
	cout << "======= " << "Main Menu" << " =======" << endl;
	draw_menu(main_menu, current_menu, 0);
	cout << endl << "===========================";
}

int Menu::register_action(int action, Menu*& current_menu)
{
	//down
	if (action == 72)
	{
		if (this->selected > 0)
			this->selected--;
		else
		{
			while (this->selected < MAX_SUB_MENUS - 1 && this->child[(this->selected) + 1] != NULL)
				this->selected++;
		}
	}
	//up
	else if (action == 80)
	{

		if (this->selected < MAX_SUB_MENUS - 1 && this->child[(this->selected) + 1] != NULL)
			this->selected++;
		else
			this->selected = 0;

	}
	//enter
	else if (action == 13)
	{
		if (this->child[this->selected] != NULL)
		{
			if (this->child[this->selected]->command != commands::Passive) //if (this->child[this->selected]->function != NULL)
			{
				if (this->child[this->selected]->command == commands::GUI)
					return 0;

				return (this->child[this->selected]->command);
			}
			else
				current_menu = this->child[this->selected];
		}

	}
	//backspace or escape
	else if (action == 8 || action == 27)
	{
		if (this->parent != NULL)
		{
			current_menu = (this->parent);
		}
		else
		{
			return 0;
		}
	}

	return commands::Passive;
}

Menu* create_menu()
{

	Menu* menu = new Menu("Main Menu", NULL);
	Menu& main_menu = *menu;

	main_menu.child[0] = new Menu("Companies", &main_menu);

	main_menu.child[0]->child[0] = new Menu("Company Wizard", main_menu.child[0]);
	main_menu.child[0]->child[0]->child[0] = new Menu("Add", main_menu.child[0]->child[0], commands::Add_Company);
	main_menu.child[0]->child[0]->child[1] = new Menu("Merge", main_menu.child[0]->child[0], commands::Merge_Companies);
	main_menu.child[0]->child[0]->child[2] = new Menu("Modify", main_menu.child[0]->child[0], commands::Modifiy_Company);
	main_menu.child[0]->child[0]->child[3] = new Menu("Remove", main_menu.child[0]->child[0], commands::Remove_Company);

	main_menu.child[0]->child[1] = new Menu("Browse Company DB", main_menu.child[0]);
	main_menu.child[0]->child[1]->child[0] = new Menu("Print", main_menu.child[0]->child[1], commands::Print_Company);
	main_menu.child[0]->child[1]->child[1] = new Menu("Print all", main_menu.child[0]->child[1], commands::Print_All_Companies);

	main_menu.child[1] = new Menu("Stocks", &main_menu);

	main_menu.child[1]->child[0] = new Menu("Trade Stocks", main_menu.child[1]);
	main_menu.child[1]->child[0]->child[0] = new Menu("Buy", main_menu.child[1]->child[0], commands::BuyStocks);
	main_menu.child[1]->child[0]->child[1] = new Menu("Sell", main_menu.child[1]->child[0], commands::SellStocks);
	main_menu.child[1]->child[0]->child[2] = new Menu("Modify", main_menu.child[1]->child[0], commands::Modify_Stock);
	main_menu.child[1]->child[0]->child[3] = new Menu("Add year to maturity date", main_menu.child[1]->child[0], commands::Add_experation);

	main_menu.child[1]->child[1] = new Menu("Browse Stocks DB", main_menu.child[1]);
	main_menu.child[1]->child[1]->child[0] = new Menu("Calculate market cap", main_menu.child[1]->child[1], commands::Calculate_All_Stocks_Worth);
	main_menu.child[1]->child[1]->child[1] = new Menu("Print", main_menu.child[1]->child[1], commands::Print_Stock);
	main_menu.child[1]->child[1]->child[2] = new Menu("Print all", main_menu.child[1]->child[1], commands::Print_All_Stock);

	main_menu.child[2] = new Menu("Sort Database by", &main_menu, commands::Sort_By_Setting);

	main_menu.child[3] = new Menu("Text Interface", &main_menu, commands::GUI);

	return menu;
}

Menu::~Menu()
{
	for (int i = 0; i < MAX_SUB_MENUS; i++)
	{
		if (!this->child[i]) // == NULL
			break;

		delete this->child[i];
	}
}