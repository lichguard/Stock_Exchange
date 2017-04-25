/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/

#include "Market.h"
#include <string>

//extra
#ifndef FILEDATABASEACCOUNTS
#define FILEDATABASEACCOUNTS "stockaccountsdb"  VERSION ".stk"
#endif

#ifndef MAXCOMMANDS
#define MAXCOMMANDS 16  //must be even number
#endif
class Menu;

class System
{
	Market market;

	void prepare_command(const std::string& title, std::string* name_input = NULL);
	void execute_command(int cmd);
	void main_loop();

#pragma region Extra
	//extra
	std::string userName;
	Menu* main_menu;
	Menu* current_menu;
	void gui(); 
	void SplashScreen() const; 
	bool Login(); 
	void register_user() const; 
	void main_loop_admin_panel();
	char verfiy_user(const std::string& user_name, const std::string& password) const;
#pragma endregion

public:
	System();
	void initiate_system();
	~System();

};
