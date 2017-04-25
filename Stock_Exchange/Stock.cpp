/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/
#include "Stock.h"

#include "PVL.h"
using namespace std;



void Share::BuyStocks(int _amount, int _total_amount) {
	this->amount += _amount;
	this->equity = (100.0 * (static_cast<double>(this->amount) / static_cast<double>(_total_amount)));
}
void Share::SellStocks(int _amount, int _total_amount) {
	this->amount -= _amount;
	this->equity = (100.0 * (static_cast<double>(this->amount) / static_cast<double>(_total_amount)));
}
Security* Share::operator+(const Security& sec) const
{
	if (typeid(sec) != typeid(Share))
		throw ExpDifferentStocksType();

	const Share& other = dynamic_cast<Share&>(const_cast<Security&>(sec));

	Share* newsec = new Share(*this);
	newsec->name = this->name + "&" + other.name;
	newsec->amount = this->amount + other.amount;
	newsec-> value = (this->value + other.value) / 2.0;
	return newsec;
}
ostream& Share::print(ostream&  os) const
{
	return os <<
		(pvl::padString(this->name, 16) +
			pvl::padString("<Share>", 17) +
			pvl::padString(pvl::convertToString(this->amount), 10) +
			pvl::padString(pvl::convertToString(this->value), 10) +
			pvl::padString(pvl::convertToString(this->equity) + "%", 13));
}

Security* Bond::operator+(const Security& sec) const
{
	if (typeid(sec) != typeid(Bond))
		throw ExpDifferentStocksType();
	const Bond& other = dynamic_cast<Bond&>(const_cast<Security&>(sec));


	Bond* newsec = new Bond(*this);
	newsec->name = this->name + "&" + other.name;
	newsec->amount = this->amount + other.amount;
	newsec->value = (this->value + other.value) / 2.0;
	newsec->coupon = (this->coupon + other.coupon) / 2.0;
	newsec->maturityDate = ((other.maturityDate > this->maturityDate) ? this->maturityDate : other.maturityDate);

	return newsec;
}
ostream& Bond::print(ostream& os) const {

	return os <<
		(pvl::padString(this->name, 16) +
			pvl::padString("<Bond>", 17) +
			pvl::padString(pvl::convertToString(this->amount), 10) +
			pvl::padString(pvl::convertToString(value), 10) +
			pvl::padString("", 13) +
			pvl::padString(pvl::convertToString(coupon), 13) +
			pvl::padString(maturityDate.get_string(), 17));
}

void State_Stock::BuyStocks(int _amount, int total_stocks) {
	this->amount += _amount;
	this->equity = (100.0 * (static_cast<double>(this->amount) / static_cast<double>(total_stocks)));
}
void State_Stock::SellStocks(int _amount, int total_stocks) {
	this->amount -= _amount;
	this->equity = (100.0 * (static_cast<double>(this->amount) / static_cast<double>(total_stocks)));
}
Security* State_Stock::operator+(const Security& sec) const
{
	if (typeid(sec) != typeid(State_Stock))
		throw ExpDifferentStocksType();

	const State_Stock& other = dynamic_cast<State_Stock&>(const_cast<Security&>(sec));


	State_Stock* newsec = new State_Stock(*this);
	newsec->name = this->name + "&" + other.name;
	newsec->amount = this->amount + other.amount;
	newsec->value = (this->value + other.value) / 2.0;
	newsec->coupon = (this->coupon + other.coupon) / 2.0;
	newsec->maturityDate = ((other.maturityDate > this->maturityDate) ? this->maturityDate : other.maturityDate);
	newsec->publicTraded = this->publicTraded && other.publicTraded;

	return newsec;
}
ostream& State_Stock::print(ostream& os) const {
	return os <<
		(pvl::padString(this->name, 16) +
			pvl::padString("<State-stock>", 17) +
			pvl::padString(pvl::convertToString(this->amount), 10) +
			pvl::padString(pvl::convertToString(value), 10) +
			pvl::padString(pvl::convertToString(this->equity) + "%", 13) +
			pvl::padString(pvl::convertToString(coupon), 13) +
			pvl::padString(maturityDate.get_string(), 17) +
			(publicTraded ? "yes" : "no"));
}

Date::Date(const string& date)
{
	day = month = year = 0;
	string data = "";

	for (size_t i = 0, index = 0; i < 3; i++, index++)
	{
		while (index < date.length() && (date[index] != '.' && date[index] != '/'))
		{
			data += date[index];
			index++;
		}
		if (i == 0)
			day = atoi(data.c_str());
		else if (i == 1)
			month = atoi(data.c_str());
		else
			year = atoi(data.c_str());

		data = "";
	}
}
bool Date::validate_date() const
{
	return !(day <= 0 || day >= 32 || month <= 0 || month >= 13 || year < 2016 || year > 2050);
}
bool Date::operator>(const Date& other) const
{
	return ((this->year * 10000 + this->month * 100 + this->day) > (other.year * 10000 + other.month * 100 + other.day));
}
string Date::get_string() const
{
	return (day >= 10 ? pvl::convertToString(day) : "0" + pvl::convertToString(day)) + "." +
		(month >= 10 ? pvl::convertToString(month) : "0" + pvl::convertToString(month)) + "." +
		pvl::convertToString(year);
}


std::ostream& operator<<(std::ostream& os, const Security* stock)
{
	return stock->print(os); 
}
std::ostream& operator<<(std::ostream& os, const Date& date) 
{ 
	return os << date.get_string(); 
}