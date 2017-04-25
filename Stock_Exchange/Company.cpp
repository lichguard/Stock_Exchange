/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/
#include "Company.h"
#include "PVL.h"
#include "Stock.h"
#include <iostream>
using namespace std;

Security* Company::IssueNewStock() const
{
	double coupon = 0;
	bool PublicTraded = false;
	Date date;
	string datestring;

	switch (this->type)
	{
	case Type_Private:
		return new Share(this->name, this->value);
		break;
	case Type_Goverment:
		cout << "Coupon: ";
		cin >> coupon;
		while (coupon < 0)
		{
			cout << "coupon cannot be negative!" << endl << "Coupon: ";
			cin >> coupon;
		}
		cout << "Maturity Date (DD.MM.YYYY): ";
		cin >> datestring; 
		date = Date(datestring);
		while (!date.validate_date())
		{
			cout << "Invalid date..." << endl << "Maturity Date (DD.MM.YYYY): ";
			cin >> datestring;
			date = Date(datestring);
		}
		return new Bond(this->name, this->value, coupon, date);
		break;
	case Type_Goverment_owned:
		cout << "Coupon: ";
		cin >> coupon;
		while (coupon < 0)
		{
			cout << "coupon cannot be negative!" << endl << "Coupon: ";
			cin >> coupon;
		}
		cout << "Maturity Date (DD.MM.YYYY): ";
		cin >> datestring;
		date = Date(datestring);
		while (!date.validate_date())
		{
			cout << "Invalid date..." << endl << "Maturity Date (DD.MM.YYYY): ";
			cin >> datestring;
			date = Date(datestring);
		}

		char answer;
		cout << "Public (y/n): ";
		cin >> answer;
		if (answer == 'y')
			PublicTraded = true;
		return new State_Stock(this->name, this->value, coupon, date, PublicTraded);
		break;
	default:
		return NULL;
		break;
	}
}

Company Company::operator+(const Company& company) const
{
	if (this->type != company.type)
		throw ExpDifferentCompaniesType();

	return Company(this->name + "&" + company.name, this->industry + "&" + company.industry, this->type, (this->value + company.value) / 2.0, this->amount + company.amount);
}

ostream& Company::print(ostream& os) const
{
	string companytypestring = "";
	if (this->type == Type_Private)
		companytypestring = "LTD";
	else if (this->type == Type_Goverment)
		companytypestring = "Goverment";
	else if (this->type == Type_Goverment_owned)
		companytypestring = "SOE";

	return os << (pvl::padString(name, 16) +
		pvl::padString("<" + companytypestring + ">", 17) +
		pvl::padString(pvl::convertToString(amount), 10) +
		pvl::padString(pvl::convertToString(value), 10) +
		industry);
}

ostream& operator<<(ostream& os, const Company& company)
{
	return company.print(os);
}
