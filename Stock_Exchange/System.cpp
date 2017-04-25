/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/
#include "System.h"
#include "PVL.h"
#include "Menu.h"
#include <conio.h> //used for key detection
#include <fstream>
#include <iostream>
#include <ctime> // used by srand, password


using namespace std;


//extra
void System::gui()
{
	if (!this->main_menu) // == NULL
		main_menu = current_menu = create_menu();

	system("cls");
	call_draw_menu(main_menu, current_menu);
	cout << endl << ">  Balance: $" << pvl::convertToString(20000.0 - this->market.getMarketCap()) << "    Date 15.06.2016" << endl;
	cout << "===========================";
	cout << endl << endl;
	cout << "Controls: backspace/esc, enter, arrows up/down";

	//gui action returns 0 to exit the gui
	int cmd = commands::Passive;
	do
	{
		cmd = current_menu->register_action(_getch(), current_menu);
		system("cls");
		call_draw_menu(main_menu, current_menu);
		cout << endl << ">  Balance: $" << pvl::convertToString(20000.0 - this->market.getMarketCap()) << " Date 15.06.2016" << endl;
		cout << "===========================";
		this->execute_command(cmd);

		if (cmd != commands::Passive)
			cout << endl << endl << "Press any key to continue...";


	} while (cmd != commands::Exit);
}


void System::prepare_command(const string& title, string* name_input)
{
	cout << endl << pvl::padString("", 24, '=') << " " << title << " " << pvl::padString("", 24, '=') << endl;
	if (name_input != NULL)
	{
		cout << "Name: ";
		cin >> (*name_input);
		//first letter in upper
		(*name_input)[0] = static_cast<char>(toupper((*name_input)[0]));
	}
	this->market.logcommand(title + (name_input != NULL ? ": " + *name_input : ""), 20000.0 - this->market.getMarketCap());
}

void System::execute_command(int cmd)
{
	string name_input = "Name: ";
	switch (cmd)
	{
	case commands::Add_Company:
		//ADD COMPANY
		prepare_command("Add Company", &name_input);
		this->market.Add_Comapny(name_input);
		break;
	case commands::Remove_Company:
		// REMOVE COMPANY
		prepare_command("Remove Company", &name_input);
		this->market.Remove_Comapny(name_input);
		break;
	case commands::Merge_Companies:
		//MERGE COMPANIES
		prepare_command("Merge Companies");
		this->market.Request_merge_companies();
		break;
	case commands::Print_Company:
		// Print a company
		prepare_command("Print Company", &name_input);
		this->market.print_Company(name_input);
		break;
	case commands::Print_All_Companies:
		// Print all companies
		prepare_command("Print all Companies");
		this->market.print_all_Company();
		break;
	case commands::Modifiy_Company:
		// Restock
		prepare_command("Modifiy Company", &name_input);
		this->market.Modifiy_Company(name_input);
		break;
	case commands::BuyStocks:
		// Buy Stocks
		prepare_command("Buy Stocks", &name_input);
		this->market.BuyStocks(name_input);
		break;
	case commands::SellStocks:
		//Sell stocks
		prepare_command("Sell Stocks");
		this->market.SellStocks();
		break;
	case commands::Add_experation:
		// Add expiration date
		prepare_command("Add year to maturitiy date ");
		this->market.incMaturityDate();
		break;
	case commands::Print_Stock:
		//Print stock
		prepare_command("Print Stock", &name_input);
		this->market.Print_Stock(name_input);
		break;
	case commands::Print_All_Stock:
		//Print all stocks
		prepare_command("Print all Stocks");
		this->market.Print_All_Stocks();
		break;
	case commands::Calculate_All_Stocks_Worth:
		// calculate total stocks value
		prepare_command("Market capitalization");
		this->market.Print_Market_Cap();
		break;
	case commands::Sort_By_Setting:
		// Sort database by user preferance
		prepare_command("Sort Database By");
		this->market.SortBySetting();
		break;
	case commands::Modify_Stock:
		// Modify stock
		prepare_command("Modify Stock", &name_input);
		this->market.Modify_Stock(name_input);
		break;
	case commands::GUI:
		//GUI
		gui();
		break;
	case commands::Print_Information:
		// Print help
		prepare_command("Information");
		cout << "We're sorry, this feature has not yet been fully implemented.";
		break;
	case commands::Show_Graph:
		// Print help
		prepare_command("Show Graph");
		this->market.Draw_Graph();
		break;
	case commands::Exit:
	case commands::Passive:
		break;
	default:
		cout << endl << endl << "invalid command";
		break;
	}
}

void System::main_loop()
{
	string commands_strings[MAXCOMMANDS] = { pvl::convertToString(commands::Add_Company) + ". Add" ,
		pvl::convertToString(commands::Merge_Companies) + ". Merge",
		pvl::convertToString(commands::Modifiy_Company) + ". Modifiy",
		pvl::convertToString(commands::Remove_Company) + ". Remove",
		pvl::convertToString(commands::Sort_By_Setting) + ". Sort by",
		pvl::convertToString(commands::Print_Company) + ". Print",
		pvl::convertToString(commands::Print_All_Companies) + ". Print all",
		pvl::convertToString("") + "",

		pvl::convertToString(commands::BuyStocks) + ". Buy",
		pvl::convertToString(commands::SellStocks) + ". Sell",
		pvl::convertToString(commands::Modify_Stock) + ". Modfiy stock",
		pvl::convertToString(commands::Add_experation) + ". Add year",
		pvl::convertToString(commands::Calculate_All_Stocks_Worth) + ". Market cap",
		pvl::convertToString(commands::Print_Stock) + ". Print",
		pvl::convertToString(commands::Print_All_Stock) + ". Print all",
		pvl::convertToString(commands::Show_Graph) + ". Show Graph"
	};
	int cmd = commands::Passive; // command index

	do
	{
		system("cls");
		cout << ">                         Main menu - Stock-Exchange " << VERSION << "          Logged as: " << (this->userName == "" ? "Guest" : this->userName) << endl;
		cout << "> |=====" << pvl::padString(" Company Section ", 20, '=') << "|=======" << pvl::padString(" Stocks Section ", 20, '=') << "|=========" << pvl::padString(" History Log ", 21, '=') << "|" << endl;
		
		for (size_t i = 0; i < MAXCOMMANDS/2; i++)
			cout << "> |     " << pvl::padString(commands_strings[i], 20) << "|       " << pvl::padString(commands_strings[i+ MAXCOMMANDS/2], 20) << "|    " << pvl::padString(this->market.printLog((MAXCOMMANDS /2)-1-i), 26) << "|" << endl;
		
		cout << "> |-----" << pvl::padString("", 20, '-') << "|-------" << pvl::padString("", 20, '-') << "|----" << pvl::padString("", 26, '-') << "|" << endl;
		cout << ">                                " << commands::Exit << ". Exit       " << commands::GUI << ". GUI       " << commands::Print_Information << ". Information" <<   endl;
		cout << ">     Balance: $" << pvl::convertToString(20000.0 - this->market.getMarketCap()) << "                                          Date 15.06.2016" <<  endl;

		this->execute_command(cmd);

		if (cmd == commands::GUI)
		{
			cmd = commands::Passive;
			continue;
		}

		cout << endl << endl << "Please enter your choice: ";
		cin >> cmd;

		//  cin string validity check
		//	string input;
		//	cin >> input;
		//	char* ptrinput = const_cast<char*>(input.c_str());
		//	cmd = strtol(ptrinput,&ptrinput,10);

	} while (cmd != commands::Exit);

	this->market.SaveDatabase(userName);
	this->market.WipeDatabase();
}

//extra
#pragma region Login_System

void System::main_loop_admin_panel()
{
	int cmd = commands::Passive; // command index
	do
	{
		system("cls");
		cout << ">              Admin Panel - Stock-Exchange " << VERSION << endl;
		cout << "> |=====" << pvl::padString(" Accounts Section ", 20, '=') << "|=======" << pvl::padString(" DB options ", 20, '=') << "|" << endl;
		cout << "> |     " << pvl::padString(pvl::convertToString(commands::Add_Company) + ". Add user", 20) << "|       " << pvl::padString(pvl::convertToString(commands::BuyStocks) + ". Print size of DB", 20) << "|" << endl;
		cout << "> |     " << pvl::padString(pvl::convertToString(commands::Merge_Companies) + ". Remove user", 20) << "|       " << pvl::padString(pvl::convertToString(commands::SellStocks) + ". Export DB", 20) << "|" << endl;
		cout << "> |     " << pvl::padString(pvl::convertToString(commands::Modifiy_Company) + ". Modify user", 20) << "|       " << pvl::padString(pvl::convertToString(commands::Modify_Stock) + ". Wipe DB", 20) << "|" << endl;
		cout << "> |     " << pvl::padString(pvl::convertToString(commands::Remove_Company) + ". Print all users", 20) << "|       " << pvl::padString("", 20) << "|" << endl;
		cout << "> |-----" << pvl::padString("", 20, '-') << "|-------" << pvl::padString("", 20, '-') << "|" << endl;
		cout << ">                 " << commands::Exit << ". Exit  " << commands::GUI << ". GUI  " << endl;

		if (cmd == commands::GUI)
		{
			cmd = commands::Passive;
			continue;
		}

		cout << endl << "NOTE: this feature has not yet been fully implemented.";

		cout << endl << endl << "Please enter your choice: ";
		//	string input;
		//	cin >> input;
		//	char* ptrinput = const_cast<char*>(input.c_str());
		//	cmd = strtol(ptrinput,&ptrinput,10);
		cin >> cmd;

	} while (cmd != commands::Exit);

}

string hashstring(const string& data)
{
	std::hash<string> hasher;
	return pvl::convertToString(hasher(data));
}

void print_in_mid(const string& text)
{
	// 120 is the char width of the console screen
	size_t add = (120 - text.length()) / 2;
	for (size_t i = 0; i < add; i++)
		cout << " ";

	cout << text;
}

void recieve_input(string& text, bool password = 0)
{
	int input;
	text = "";
	input = _getch();
	//if escaped was pressed
	if (input == 27)
		exit(0);
	while (input != 13)
	{
		// if backspace was pressed
		if (input != 8)
		{
			text += static_cast<char>(input);
			cout << (password ? '*' : static_cast<char>(input));
		}
		input = _getch();
		if (input == 8 && text.length() > 0)
		{
			_putch('\b');
			cout << " ";
			_putch('\b');
			text.erase(text.end() - 1);
		}
		if (input == 27)
			exit(0);
	}
}

void System::register_user() const
{
	this->SplashScreen();
	cout << endl << endl << endl << endl;
	print_in_mid("Register new account");
	cout << endl;
	string user_name = "";
	string password = "";
	string password2 = "";
	cout << endl;
	print_in_mid("UserName: ");
	recieve_input(user_name);
	cout << endl;
	print_in_mid("Password: ");
	recieve_input(password, 1);
	cout << endl;
	print_in_mid("Retype password: ");
	recieve_input(password2, 1);

	if (password == "" || user_name == "")
	{
		cout << endl << endl;
		print_in_mid("Registration failed, password/username cannot be blank...");

	}
	else if (this->verfiy_user(user_name, "") > 0)
	{
		cout << endl << endl;
		print_in_mid("Registration failed, user already exists...");
	}
	else if (password != password2)
	{
		cout << endl << endl;
		print_in_mid("Registration failed, passwords do not match...");
	}
	else
	{
		//checks if the file exsits to add a heaer
		bool file_exists = false;
		ifstream in = ifstream(FILEDATABASEACCOUNTS, ifstream::in);
		if (in.is_open())
		{
			file_exists = true;
			in.close();
		}

		ofstream out = ofstream(FILEDATABASEACCOUNTS, fstream::out | fstream::app);

		if (!file_exists)
			out << "Stock_Exchange_Database " << VERSION << endl;

		srand(static_cast<unsigned>(time(NULL)));
		string salt = pvl::convertToString(rand() % 1000);
		out << endl << user_name << " " << hashstring(password + salt) << " " << salt << endl;
		out.close();

		cout << endl << endl;
		print_in_mid("registered successfully");
	}
	_getch();
}

// 0 - > does not exit 1 -> exists 2-> verfied user
char System::verfiy_user(const string& user_name, const string& password) const
{
	//	hardcoded users
	if (user_name == "admin" && password == "adminpassword")
		return 2;
	//	else if (user_name == "" && password == "")
	//		return 2;

	string username_memory;
	string password_memory;
	string salt;
	ifstream in = ifstream(FILEDATABASEACCOUNTS, ifstream::in);

	if (!in.is_open())
	{

		cout << endl << endl;
		print_in_mid("Cannot locate accounts database");
		cout << endl;
		return false;
	}
	string trash;
	if (!in.eof())
		in >> trash >> trash;
	while (!in.eof())
	{
		in >> username_memory >> password_memory >> salt;
		if ((username_memory == user_name) && password_memory == hashstring(password + salt))
		{
			in.close();
			return 2;
		}
		else if ((username_memory == user_name))
		{
			in.close();
			return 1;
		}
	}

	in.close();
	return 0;
}

bool System::Login()
{
	string user_name = "";
	string password = "";
	int input;

	cout << endl << endl << endl << endl;
	print_in_mid("Welcome, Please Enter your user name and password");
	cout << endl << endl;
	print_in_mid("UserName: ");
	recieve_input(user_name);
	cout << endl;
	print_in_mid("Password: ");
	recieve_input(password, 1);

	// 2 -> verfied user password and username match in DB
	if (this->verfiy_user(user_name, password) == 2)
	{
		this->userName = user_name;

		cout << endl << endl << endl << endl;
		print_in_mid("Welcome  " + user_name);
		cout << endl << endl;
		print_in_mid(this->market.LoadDatabase(userName) ? "Database loaded successfully" : "Initialising new user parameters...");
		cout << endl;

		//logged in successfully
		return true;
	}
	else
	{
		cout << endl << endl;
		print_in_mid("UserName or password is not correct");

		cout << endl << endl;
		print_in_mid("To register new account please press 'r'");
		input = _getch();
		//if escaped is pressed
		if (input == 27)
			exit(0);

		//if 'r' is pressed
		if (input == 114)
			register_user();

		// no useranme and password match of the user
		return false;
	}
}

void System::SplashScreen() const
{
	system("cls");
	cout
		<< "	          ______                 _            _______            _                             " << endl
		<< "	         / _____) _             | |          | ______)          | |                            " << endl
		<< "	        ( (____ _| |_ ___   ____| |  _       | |___   _   _ ____| |__  _____ ____   ____ _____ " << endl
		<< "	         \\____ (_   _) _ \\ / ___) |_/ )      |  ___) ( \\ / ) ___)  _ \\(____ |  _ \\ / _  | ___ |" << endl
		<< "	         _____) )| || |_| ( (___|  _ (       | |_____ ) X ( (___| | | / ___ | | | ( (_| | ____|" << endl
		<< "	        (______/ \\__)\\___/ \\____)_| \\_)      |_______|_/ \\_)____)_| |_\\_____|_| |_|\\___ |_____)" << endl
		<< "	                                                                                  (_____|  	   " << endl;
	cout << endl << endl;
	print_in_mid("2016 Created by Dean, Shani & Lotem.  "  VERSION);
}

#pragma endregion

void System::initiate_system()
{
	// the program will exit upon detecting the esc key (exit(0))
	while (1)
	{
		//extra
		SplashScreen();
		while (!this->Login())
			SplashScreen();
		_getch();

		//calls the main loop of the program
		if (userName == "admin")
			main_loop_admin_panel();
		else
			main_loop();
	}
}


System::System()
{
	userName = "Guest";
	main_menu = current_menu = NULL;
}

System::~System()
{
	delete main_menu;
}


