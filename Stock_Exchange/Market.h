/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/
#ifndef _MARKET_H
#define _MARKET_H

//extra
#ifndef VERSION
#define VERSION "v084"
#endif

//extra
#ifndef FILEDATABASE
#define FILEDATABASE "stockdb" VERSION
#endif

#include <string>
#include <vector>

class ExpEntityMissing : public std::exception
{
public:
	virtual const char* what() const throw() { return "No entity by that name!"; }
};

class Company;
class Security;

struct Log_Structure
{
	std::string last_action;
	double current_balance;
	Log_Structure(const std::string& data, double current_balance) : last_action(data), current_balance(current_balance)
	{

	}

};

class Market
{
	std::vector<Company> companies;
	std::vector<Security*> stocks;

	std::vector<Log_Structure> log;

	//extra
	int sort_setting;

	//private constructors, to make sure no one uses them
	Market operator=(const Market&);
	Market(const Market&);

#pragma region Entities_Operations

	bool doesCompanyExist(const std::string& _company_name) const;
	const std::vector<Company>::const_iterator getCompanyIt(const std::string& _company_name) const;
	std::vector<Company>::iterator getCompanyIt(const std::string& _company_name);

	bool doesSecurityExists(const std::string& _company_name) const;
	std::vector<Security*>::iterator getSecurityIt(const std::string& _company_name);
	const std::vector<Security*>::const_iterator getSecurityIt(const std::string& _company_name) const;

	void SuggestEntity(const std::string& _company_name) const;

#pragma endregion

public:
	Market();
	~Market();

#pragma region Miscellaneous
	void SaveDatabase(const std::string& _username) const;
	bool LoadDatabase(const std::string& _username);
	void SortDatabase();
	void SortBySetting();
	void Market::WipeDatabase();
	std::string printLog(size_t item) const;
	void logcommand(const std::string& data, double current_balance);
	void Draw_Graph() const;
#pragma endregion

#pragma region companies 

	void Add_Comapny(const std::string& _company_name);
	void Remove_Comapny(const std::string&  _company_name);
	void print_Company(const std::string& _company_name, bool chart_format = 1) const;
	void print_all_Company() const;
	void Modifiy_Company(const std::string& _company_name);
	void Request_merge_companies();
	void Merge_Companies(const std::string& company1, const std::string& company2);

#pragma endregion

#pragma region securities

	void BuyStocks(const std::string& _company_name);
	void SellStocks();
	void incMaturityDate();
	void Print_Stock(const std::string& _company_name) const;
	void Print_All_Stocks() const;
	void Print_Market_Cap() const;
	void Modify_Stock(const std::string& _company_name);
	double getMarketCap() const;
#pragma endregion

};
#endif