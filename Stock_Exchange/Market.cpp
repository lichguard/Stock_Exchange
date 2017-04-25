/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/
#include "Market.h"

#include "PVL.h"
#include "Company.h"
#include "Stock.h"

#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

#pragma region Constructors

Market::Market() : sort_setting(1) { }

Market::~Market()
{
	for (vector<Security*>::const_iterator it = stocks.begin(); it != stocks.end(); it++)
		delete (*it);
}

Market Market::operator=(const Market&) { return Market(); }
Market::Market(const Market&) {}

#pragma endregion

#pragma region Entities

const vector<Company>::const_iterator Market::getCompanyIt(const string& _company_name) const
{
	vector<Company>::const_iterator it = find(companies.begin(), companies.end(), _company_name);
	if (it == companies.end())
		throw ExpEntityMissing();
	return it;
}

vector<Company>::iterator Market::getCompanyIt(const string& _company_name)
{
	vector<Company>::iterator it = find(companies.begin(), companies.end(), _company_name);
	if (it == companies.end())
		throw ExpEntityMissing();
	return it;
}

vector<Security*>::iterator Market::getSecurityIt(const string& _company_name)
{
	
	vector<Security*>::iterator it = stocks.begin();
	for (; it != stocks.end(); it++)
	{
		if ((*(*it)) == _company_name)
			break;
	}

	if (it == stocks.end())
		throw ExpEntityMissing();

	return it;
}

const vector<Security*>::const_iterator Market::getSecurityIt(const string& _company_name) const
{
	vector<Security*>::const_iterator it = stocks.begin();
	for (; it != stocks.end(); it++)
	{
		if ((*(*it)) == _company_name)
			break;
	}

	if (it == stocks.end())
		throw ExpEntityMissing();

	return it;
}

bool Market::doesCompanyExist(const string& _company_name) const
{
	vector<Company>::const_iterator it = find(companies.begin(), companies.end(), _company_name);
	return (it != companies.end());
}

bool Market::doesSecurityExists(const std::string& _company_name) const
{
	vector<Security*>::const_iterator it = stocks.begin();
	for (; it != stocks.end(); it++)
	{
		if ((*(*it)) == _company_name)
			return true;
	}
	return false;
}

void Market::SuggestEntity(const std::string& _company_name) const
{
	double relevance = 1;
	string suggest = _company_name;
	for each (Company com in companies)
	{
		string name = com.getName();
		double local_relevance = 1;
		for (size_t i = 0; i < _company_name.length(); i++)
		{
			if (name.find(_company_name[i]) != string::npos)
			{
				local_relevance *= 1.2;
				local_relevance += 5.0 / (1.0+abs(static_cast<double>(name.find(_company_name[i])) - static_cast<double>(i)));
			}
			else
				local_relevance /= 1.5;
		}

		local_relevance += 1.0 / (2.0 + abs(static_cast<double>(_company_name.length()) - static_cast<double>(name.length())));
		if (local_relevance > relevance)
		{
			relevance = local_relevance;
			suggest = name;
		}
	}

	if (relevance > 6.0)
		cout << endl << endl << "Did you mean: " << suggest << endl; // <<" (" << relevance << ")" << endl;
}

#pragma endregion

#pragma region Companies 

void Market::Add_Comapny(const string& _company_name)
{
	//makes sure the company does not exist
	if (this->doesCompanyExist(_company_name))
	{
		cout << "Company already exists!";
		return;
	}

	string _industry;int _CompanyType = 0;double _stock_value;int _amount_of_stock;
	cout << "Industry: ";
	cin >> _industry;

	//user must submit a valid type
	cout << "Type(1-Private/2-Goverment/3-goverment-owned(SOE)): ";
	cin >> _CompanyType;
	while (_CompanyType != CompanyType::Type_Goverment && _CompanyType != CompanyType::Type_Goverment_owned && _CompanyType != CompanyType::Type_Private)
	{
		cout << "Invalid type!" << endl << "Type(1-Private/2-Goverment/3-goverment-owned(SOE)): ";
		cin >> _CompanyType;
	}

	cout << "Available stocks: ";
	cin >> _amount_of_stock;
	while (_amount_of_stock < 0)
	{
		cout << "Amount cannot be negaive!" << endl << "Available stocks: ";
		cin >> _amount_of_stock;
	}

	cout << "Stock Value: ";
	cin >> _stock_value;
	while (_stock_value < 0)
	{
		cout << "Value cannot be negaive!" << endl << "Stock Value: ";
		cin >> _stock_value;
	}
	companies.push_back(Company(_company_name, _industry, static_cast<CompanyType>(_CompanyType), _stock_value, _amount_of_stock));
	this->SortDatabase();

	cout << endl;
	cout << "========================== New company info ===============================" << endl;
	this->print_Company(_company_name);
}

void Market::Remove_Comapny(const string&  _company_name)
{
	try
	{
		companies.erase(this->getCompanyIt(_company_name));
		cout << endl << _company_name << " had been removed.";
	}
	catch (ExpEntityMissing& e) { 
		cout << e.what(); 
		SuggestEntity(_company_name);
		return; }


	try
	{
		vector<Security*>::iterator sec = this->getSecurityIt(_company_name);
		delete (*sec);
		stocks.erase(sec);
		cout << endl << _company_name << " stock had been removed.";
	}
	catch (ExpEntityMissing&) {}
}

void Market::print_Company(const string& _company_name, bool chart_format) const {

	//If chart_format, the chart title will be displayed
	if (chart_format)
	{
		cout << pvl::padString("NAME", 16) <<
			pvl::padString("TYPE", 17) <<
			pvl::padString("SHARES", 10) <<
			pvl::padString("VALUE", 10) <<
			"INDUSTRY" << endl;
	}

	try {cout << *(this->getCompanyIt(_company_name));}
	catch (ExpEntityMissing& e) 
	{
		cout << e.what();
		SuggestEntity(_company_name);
	}
}

void Market::print_all_Company() const {

	cout << pvl::padString("NAME", 16) <<
		pvl::padString("TYPE", 17) <<
		pvl::padString("SHARES", 10) <<
		pvl::padString("VALUE", 10) <<
		"INDUSTRY" << endl;

	if (!companies.size()) //== 0
	{
		cout << "No companies in DB!";
		return;
	}

	for each (Company com in companies)
		cout << com << endl;
}

void Market::Modifiy_Company(const string& _company_name)
{
	Company *com = NULL;
	Security* sec = NULL;
	try { com = &(*this->getCompanyIt(_company_name)); }
	catch (ExpEntityMissing& exp)
	{
		cout << exp.what();
		SuggestEntity(_company_name);
		return;
	}
	try{sec = *(this->getSecurityIt(_company_name));}
	catch (ExpEntityMissing&) { sec = NULL; }

	string new_name;
	string new_industry;
	double value;
	int amount;
	cout << "New name (" << _company_name << "): ";
	cin >> new_name;
	(new_name)[0] = static_cast<char>(toupper((new_name)[0]));
	if (this->doesCompanyExist(new_name) && new_name != _company_name)
	{
		new_name = _company_name;
		cout << "Company with that name already exists!";
		return;
	}

	cout << endl << "Industry (current: " << com->getIndustry() << "): ";
	cin >> new_industry;
	cout << endl << "Stock Value (current: " << com->getValue() << "): ";
	cin >> value;
	while (value < 0)
	{
		cout << "Amount cannot be negative!" << endl << "Stock Value (current: " << com->getValue() << "): ";
		cin >> value;
	}
	cout << endl << "Available Stocks (current: " << com->getAmount() << "): ";
	cin >> amount;
	while (amount < 0)
	{
		cout << "Amount cannot be negative!" << endl << "Available Stocks (current: " << com->getAmount() << "): ";
		cin >> amount;
	}
	if (sec != NULL) 
	{
		while (sec->getAmount() > amount)
		{
			cout << "The company has already issued " << sec->getAmount() << " stocks!" << endl;
			cout << "Available Stocks (current: " << com->getAmount() << "): ";
			cin >> amount;
			while (amount < 0)
			{
				cout << "Amount cannot be negative!" << endl << "Available Stocks (current: " << com->getAmount() << "): ";
				cin >> amount;
			}
		}
	}

	com->setValue(value);
	com->setAmount(amount);
	com->setName(new_name);
	com->setIndustry(new_industry);
	this->SortDatabase();

	cout << endl  <<  "============================== Updated Company Info ==============================" << endl;
	this->print_Company(new_name);

	//securities
	if (sec != NULL)
	{
		sec->setName(new_name);
		sec->setValue(value);
		sec->BuyStocks(0, amount);

		cout << endl << endl << endl << "============================ Updated stock Info =================================" << endl;
		this->Print_Stock(new_name);
	}
}

void Market::Request_merge_companies()
{
	string c1;
	string c2;
	cout << "1st company: ";
	cin >> c1;
	(c1)[0] = static_cast<char>(toupper((c1)[0]));
	cout << "2nd company: ";
	cin >> c2;
	(c2)[0] = static_cast<char>(toupper((c2)[0]));
	if (this->doesCompanyExist(c1 + "&" + c2))
	{
		cout << "Company with that name already exists!";
		return;
	}

	try
	{
		Merge_Companies(c1, c2);

		this->SortDatabase();
		cout << endl << endl;
		cout << "============================= Merged company info ==============================" << endl;
		this->print_Company(c1 + "&" + c2);

		if (doesSecurityExists(c1 + "&" + c2))
		{
			cout << endl << endl << "============================ Merged Stock info =================================" << endl;
			this->Print_Stock(c1 + "&" + c2);
		}
	}
	catch (ExpEntityMissing& e)
	{
		cout << e.what();
		SuggestEntity(c1);
		SuggestEntity(c2);
	}
	catch (exception& e)
	{
		cout << e.what();
	}
}

void Market::Merge_Companies(const string& company1, const string& company2)
{
	//Companies 
	Company com1 = *(getCompanyIt(company1));
	Company com2 = *(getCompanyIt(company2));
	//adds the new company and return error if not the same type
	Company new_com = com1 + com2;
	companies.push_back(new_com);

	Security* sec1 = NULL;
	Security* sec2 = NULL;
	Security* new_sec = NULL;

	//adds stocks if one of them is not found to merge them later on
	try { sec1 = *(getSecurityIt(company1)); }
	catch (ExpEntityMissing&) { sec1 = NULL; }
	try { sec2 = *(getSecurityIt(company2)); }
	catch (ExpEntityMissing&) { sec2 = NULL; }


	if (sec1 != NULL && sec2 != NULL)
	{
		new_sec = (*sec1 + *sec2);
		new_sec->BuyStocks(0, new_com.getAmount());
		stocks.push_back(new_sec);
	}
	else if (sec1 != NULL || sec2 != NULL)
	{
		if (sec1 == NULL)
			sec1 = sec2;

		sec1->setName(new_com.getName());
		sec1->setValue(new_com.getValue());
		sec1->BuyStocks(0, new_com.getAmount());
	}

	Remove_Comapny(company1);
	Remove_Comapny(company2);
}

#pragma endregion

#pragma region Securities

void Market::BuyStocks(const string& _company_name)
{
	char create_new_company = 0;

	//adds new company if needed
	Company* com;
	if (!doesCompanyExist(_company_name))
	{
		cout << endl << "No entity by that name!";
		SuggestEntity(_company_name); 
		cout << "Would you like to add a new company? (y/n)" << endl;
		cin >> create_new_company;
		cout << endl;

		if (create_new_company != 0 && create_new_company != 'y')
			return;
		if (create_new_company == 'y')
		{
			this->Add_Comapny(_company_name);
			cout << endl << endl << "Buy Stock: " << endl << endl;
		}
	}
	com = &(*this->getCompanyIt(_company_name));

	// request amount
	int amount;
	cout << "Amount: ";
	cin >> amount;
	if (amount <= 0) { cout << "Invalid amount!"; return; }

	//checks if security exists
	Security* sec = NULL;
	try { sec = *this->getSecurityIt(_company_name); }
	catch (ExpEntityMissing&) { sec = NULL; }

	// checks if the company has the available amount of stocks
	int new_stocks = com->getAmount() - amount - (!sec ? 0 : sec->getAmount()); // == NULL
	if (new_stocks < 0)
	{
		cout << endl << _company_name << " company has only " << com->getAmount() - (!sec ? 0 : sec->getAmount()) << " stocks available to purchase!" << endl; // == NULL
		return;
	}

	//creates teh security if needed
	if (!sec) // == NULL
	{
		sec = com->IssueNewStock();
		stocks.push_back(sec);
	}

	//if all goes right, buy the amount requested
	sec->BuyStocks(amount, com->getAmount());
	this->SortDatabase();

	cout << endl << endl << "========================== New stock info =============================" << endl;
	this->Print_Stock(_company_name);
}

void Market::SellStocks()
{
	string _company_name;
	cout << "Name: ";
	cin >> _company_name;
	(_company_name)[0] = static_cast<char>(toupper((_company_name)[0]));

	Company* com;
	try { com = &(*this->getCompanyIt(_company_name)); }
	catch (ExpEntityMissing& exp) { cout << exp.what(); SuggestEntity(_company_name); return; }

	Security* sec = NULL;
	try { sec = *this->getSecurityIt(_company_name); }
	catch (ExpEntityMissing&) { cout << "You don't have any stocks to sell!"; return; }

	int amount;
	cout << "Amount: ";
	cin >> amount;
	if (amount <= 0) { cout << "Invalid amount!"; return; }

	int new_stocks_number = (sec->getAmount() - amount);

	if (new_stocks_number < 0)
	{
		cout << endl << _company_name << " stock has only " << sec->getAmount() << " stocks available to sell!" << endl;
		return;
	}
	else if (!new_stocks_number) // == 0
	{
		vector<Security*>::iterator it_sec = this->getSecurityIt(_company_name);
		delete (*it_sec);
		stocks.erase(it_sec);
		cout << "All stock sold!";
	}
	else
	{
		sec->SellStocks(amount, com->getAmount());

		this->SortDatabase();
		cout << endl << endl << "========================= Stock Updated ==========================" << endl;
		this->Print_Stock(_company_name);
	}
}

void Market::incMaturityDate()
{
	bool exists = false;

	cout <<
		(pvl::padString("ISSUER NAME", 16) +
			pvl::padString("ISSUE TYPE", 17) +
			pvl::padString("VOLUME", 10) +
			pvl::padString("VALUE", 10) +
			pvl::padString("EQUITY", 13) +
			pvl::padString("COUPON", 13) +
			pvl::padString("MATURITY DATE", 17) +
			"PUBLIC") << endl;

	for (vector<Security*>::const_iterator it = stocks.begin(); it != stocks.end(); it++)
		if (Bond* item = dynamic_cast<Bond*>(*it))
		{
			if (item->getMaturityDate().year < 2050)
				item->addYearToDate();

			cout << (*it) << endl;
			exists = true;
		}

	if (!exists)
		cout << "No bonds or state-stock in DB!";
}

void Market::Print_Stock(const string& _company_name) const
{
	cout <<
		(pvl::padString("ISSUER NAME", 16) +
			pvl::padString("ISSUE TYPE", 17) +
			pvl::padString("VOLUME", 10) +
			pvl::padString("VALUE", 10) +
			pvl::padString("EQUITY", 13) +
			pvl::padString("COUPON", 13) +
			pvl::padString("MATURITY DATE", 17) +
			"PUBLIC") << endl;

	try {
		cout << (*(this->getSecurityIt(_company_name)));
	}
	catch (ExpEntityMissing& e) {
		cout << e.what();
		SuggestEntity(_company_name);
	}
}

void Market::Print_All_Stocks() const {
	
	cout <<
		(pvl::padString("ISSUER NAME", 16) +
			pvl::padString("ISSUE TYPE", 17) +
			pvl::padString("VOLUME", 10) +
			pvl::padString("VALUE", 10) +
			pvl::padString("EQUITY", 13) +
			pvl::padString("COUPON", 13) +
			pvl::padString("MATURITY DATE", 17) +
			"PUBLIC") << endl;

	if (!stocks.size()) // == 0
		cout << "No stocks in DB!";

	for each (Security* sec in stocks)
		cout << sec << endl;
}

void Market::Print_Market_Cap() const
{
	cout << "Market capitalization in DB: $" << this->getMarketCap();
}

void Market::Modify_Stock(const string& _company_name)
{
	try {
		Security* sec = *(this->getSecurityIt(_company_name));

		if (typeid(*sec) == typeid(Share))
		{
			cout << "No modification available for shares...";
			return;
		}


		double coupon = 0;
		bool PublicTraded = false;
		Date date;
		string datestring;

			cout << "Coupon (" << dynamic_cast<Bond*>(sec)->getCoupon() << "): ";
			cin >> coupon ;
			while (coupon < 0)
			{
				cout << "coupon cannot be negative!" << endl << "Coupon: ";
				cin >> coupon;
			}
			cout << "Maturity Date (DD.MM.YYYY) [" << dynamic_cast<Bond*>(sec)->getMaturityDate() << "]: ";
			cin >> datestring;
			date = Date(datestring);
			while (!date.validate_date())
			{
				cout << "Invalid date..." << endl << "Maturity Date (DD.MM.YYYY): "; 
				cin >> datestring;
				date = Date(datestring);
			}

			if (dynamic_cast<State_Stock*>(sec) != NULL)
			{
				char answer;
				cout << "Public (y/n) (" << dynamic_cast<State_Stock*>(sec)->getPublic() << "): ";
				cin >> answer;
				if (answer == 'y')
					PublicTraded = true;

				dynamic_cast<State_Stock*>(sec)->setPublic(PublicTraded);
			}

			dynamic_cast<Bond*>(sec)->setCoupon(coupon);
			dynamic_cast<Bond*>(sec)->setMaturityDate(date);


			this->SortDatabase();
			cout << endl << endl << "========================= Stock Updated ==========================" << endl;
			this->Print_Stock(_company_name);
		
	}
	catch (ExpEntityMissing& exp) 
	{ cout << exp.what();}

}

double Market::getMarketCap() const
{
	double worth = 0;

	for each (Security* sec in stocks)
		worth += sec->getStocksValue();
	
	return worth;
}

#pragma endregion

//extra
#pragma region Miscellaneous

struct CompaniesComperator {
	int sort_setting;

	CompaniesComperator(int sort_setting) { this->sort_setting = sort_setting; }
	bool operator () (const Company& com1, const Company& com2) const
	{
		switch (sort_setting)
		{
		case 1:
			return com1.getName() < com2.getName();
			break;
		case 2:
			return com1.getValue() > com2.getValue();
			break;
		case 3:
			return com1.getAmount() > com2.getAmount();
			break;
		case 4:
			return com1.getAmount() * com1.getValue() > com2.getAmount() * com2.getValue();
			break;
		case 5:
			return com1.getType() < com2.getType();
			break;
		default:
			break;
		}

		return true;
	}
};

struct SecuritiesComperator {
	int sort_setting;

	SecuritiesComperator(int sort_setting) { this->sort_setting = sort_setting; }
	bool operator () (const Security* sec1, const Security* sec2) const
	{
		switch (sort_setting)
		{
		case 1:
			return sec1->getName() < sec2->getName();
			break;
		case 2:
			return sec1->getValue() > sec2->getValue();
			break;
		case 3:
			return sec1->getAmount() > sec2->getAmount();
			break;
		case 4:
			return sec1->getStocksValue() > sec2->getStocksValue();
			break;
		case 5:

			if (typeid(Share) == typeid(*sec1) && typeid(Bond) == typeid(*sec2))
				return true;
			else if (typeid(Share) == typeid(*sec1) && typeid(State_Stock) == typeid(*sec2))
				return true;
			else if (typeid(Bond) == typeid(*sec1) && typeid(State_Stock) == typeid(*sec2))
				return true;
			else
				return false;

			break;
		default:
			break;
		}
		return true;
	}
};

void Market::SortDatabase()
{
	if (!sort_setting) // == 0
		return;

	std::sort(this->companies.begin(), this->companies.end(), CompaniesComperator(this->sort_setting));
	std::sort(this->stocks.begin(), this->stocks.end(), SecuritiesComperator(this->sort_setting));

}

void Market::SortBySetting()
{
	int input;
	cout << "Choose option to sort by: " << endl;
	cout << "0 - Off   1 - Name    2 - Value  3 - Volume   4 - Market Cap   5 - Type" << endl;
	cout << "Currently: " << sort_setting << endl << endl;
	cout << "Enter your choice: ";
	cin >> input;
	if (input < 0 || input > 5)
		cout << "Invalid option..." << endl;
	else
	{
		sort_setting = input;
		this->SortDatabase();
	}
}

std::string Market::printLog(size_t item) const
{
	return this->log.size() > item ? this->log.at(item).last_action : "";
}

void Market::logcommand(const std::string& data, double current_balance)
{
	if (log.size() == 7)
		log.erase(log.end() - 1);

	this->log.insert(log.begin(), Log_Structure(data,current_balance));
}

void Market::SaveDatabase(const string& _username) const
{
	ofstream out = ofstream(FILEDATABASE + _username + ".stk", fstream::out);

	out << "Stock_Exchange_Database " << VERSION << endl << endl;
	out << "sort_settings: " << sort_setting << endl;

	for each (Company com in companies)
	{
		out << "company" << endl;
		out << com << endl;
	}
	for each (Security* sec in stocks)
	{
		out << typeid(*sec).name() << endl;
		out << sec << endl;
	}
	out.close();
}

void Market::WipeDatabase()
{
	while (companies.size() > 0)
		this->companies.erase(companies.begin());

	while (this->log.size() > 0)
		this->log.erase(log.begin());


	while (stocks.size() > 0)
	{
		delete *(stocks.begin());
		this->stocks.erase(stocks.begin());
	}
}

bool Market::LoadDatabase(const string& _username)
{
	ifstream in = ifstream(FILEDATABASE + _username + ".stk", ifstream::in);
	if (!in.is_open())
		return false;
	string datatype;

	if (!in.eof())
	{
		in >> datatype >> datatype >> datatype >> sort_setting;
	}

	while (!in.eof())
	{
		datatype = "";
		in >> datatype;
		if (datatype == "company")
		{
			string name;
			string type;
			int amount;
			CompanyType t1;
			double value;
			string industry;
			in >> name >> type >> amount >> value >> industry;

			if (type == "<LTD>")
				t1 = CompanyType::Type_Private;
			else if (type == "<SOE>")
				t1 = CompanyType::Type_Goverment_owned;
			else
				t1 = CompanyType::Type_Goverment;

			companies.push_back(Company(name, industry, t1, value, amount));

		}
		else if (datatype == "class")
		{
			in >> datatype;
			if (datatype == "Share")
			{
				string name;
				string notrelevant;
				int volume;
				double value;
				in >> name >> notrelevant >> volume >> value >> notrelevant;
				Security* sec = new  Share(name, value);
				sec->BuyStocks(volume, getCompanyIt(name)->getAmount());
				stocks.push_back(sec);
			}
			else if (datatype == "Bond")
			{
				string name;
				string notrelevant;
				int volume;
				double value;
				double coupon;
				string date;
				in >> name >> notrelevant >> volume >> value >> coupon >> date;
				Security* sec = new  Bond(name, value, coupon, Date(date));
				sec->BuyStocks(volume, getCompanyIt(name)->getAmount());
				stocks.push_back(sec);
			}
			else if (datatype == "State_Stock")
			{
				string name;
				string notrelevant;
				int volume;
				double value;
				double coupon;
				string publictraded = "no";
				string date;

				in >> name >> notrelevant >> volume >> value >> notrelevant >> coupon >> date >> publictraded;
				Security* sec = new  State_Stock(name, value, coupon, Date(date), (publictraded == "no" ? 0 : 1));
				sec->BuyStocks(volume, getCompanyIt(name)->getAmount());
				stocks.push_back(sec);
			}

		}
	}
	in.close();

	return true;
}

//void Market::Draw_Graph() const
//{
//	const int WIDTH = 120;
//	const int HEIGHT = 30;
//	char map[HEIGHT][WIDTH];
//
//	for (size_t i = 0; i < HEIGHT; i++)
//		for (size_t j = 0; j < WIDTH; j++)
//			map[i][j] = ' ';
//
//	for (size_t i = 0; i < WIDTH; i++)
//		map[15][i] = '-';
//
//	for (size_t i = 0; i < HEIGHT; i++)
//		map[i][HEIGHT * 2] = '|';
//
//	for (int i = HEIGHT * -2; i < HEIGHT * 2; i++)
//	{
//		int x = i;
//		int y = 0;
//
//		y = static_cast<int>((sin(static_cast<double>(x) / static_cast<double>(HEIGHT)*3.14)*static_cast<double>(HEIGHT / 2)));
//
//		//y = static_cast<int>(( (static_cast<double>(x) / 30.0)*(static_cast<double>(x) / 30.0) *30.0));
//
//		//cout << "sin(" << sin(static_cast<double>(i) / 15.0) << ") = " << y << endl;
//		//cout << "X: " << x + 60 << "  y: " << y + 15 << endl;
//		//cout << y << endl;
//
//		if (y + HEIGHT / 2 >= 0 && y + HEIGHT / 2 <= HEIGHT && x + WIDTH / 2 >= 0 && x + WIDTH / 2 <= WIDTH && map[y + 15][x + WIDTH / 2] != '-' && map[y + HEIGHT / 2][x + WIDTH / 2] != '|')
//			map[y + HEIGHT / 2][x + WIDTH / 2] = '*';
//
//	}
//
//	for (size_t i = 0; i < HEIGHT; i++)
//		for (size_t j = 0; j < WIDTH; j++)
//			cout << map[i][j];
//}

void Market::Draw_Graph() const
{
	const int WIDTH = 70;
	const int HEIGHT = 25;
	char map[HEIGHT][WIDTH];

	for (size_t i = 0; i < HEIGHT; i++)
		for (size_t j = 0; j < WIDTH; j++)
			map[i][j] = ' ';
		
	for (size_t i = 0; i < WIDTH; i++)
		map[20][i] = '-';
	
	for (size_t i = 0; i < HEIGHT; i++)
		map[i][5] = '|';
	
	for (size_t i = 1; i < this->log.size(); i++)
	{
		map[21][i * WIDTH / this->log.size()] = pvl::convertToString(this->log.size()-i)[0];

		for (int j = 19; j > rand()%17+1; j--)
		{
			map[j][i * WIDTH / this->log.size()] = '=';
		}
	}

	for (size_t i = 0; i < HEIGHT; i++)
	{
		for (size_t j = 0; j < WIDTH; j++)
			cout << map[i][j];
		cout << endl;
	}
}
#pragma endregion

