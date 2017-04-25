/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/

#ifndef _STOCK_H
#define _STOCK_H

#include <string>

class ExpDifferentStocksType : public std::exception
{
public:
	virtual const char* what() const throw() { return "Cant merge 2 securities of different types!"; }
};

struct Date
{
	size_t day;
	size_t month;
	size_t year;
	Date(size_t day = 0, size_t month = 0, size_t year = 0) : day(day), month(month), year(year) {}
	Date(const std::string& date);

	bool validate_date() const;

	bool operator>(const Date& other) const;
	std::string get_string() const;
};

class Security
{
protected:
	std::string name;
	double value;
	int amount;
public:
	Security(const std::string& _name, double _value) : name(_name), amount(0), value(_value) {}
	virtual void BuyStocks(int _amount, int _total_amount) = 0;
	virtual void SellStocks(int _amount, int _total_amount) = 0;
	virtual void setValue(double _value) { this->value = _value; }
	virtual void setName(const std::string& _name) { this->name = _name; }
	virtual const std::string& getName() const { return this->name; }
	virtual double getValue() const { return (this->value); }
	virtual double getStocksValue() const { return (this->value * this->amount); }
	virtual int getAmount() const { return amount; }
	virtual void setAmount(int _amount) { this->amount = _amount; }
	virtual std::ostream& print(std::ostream&) const = 0;
	virtual Security* operator+(const Security& sec) const = 0;
	virtual bool operator==(const std::string& other) const { return (this->name == other); }
	virtual ~Security() {}
};

class Share : virtual public Security
{
protected:
	double equity;
public:
	Share(const std::string& _name, double _value) : Security(_name, _value), equity(0) {}
	virtual void BuyStocks(int _amount, int _total_amount);
	virtual void SellStocks(int _amount, int _total_amount);
	virtual Security* operator+(const Security& sec) const;
	virtual std::ostream& print(std::ostream&  os) const;
	virtual ~Share() {}
};

class Bond : virtual public Security
{
protected:
	double coupon;
	Date maturityDate;

public:
	Bond(const std::string& _name, double _value, double _coupon, const Date& _date) : Security(_name, _value), maturityDate(_date), coupon(_coupon) { }
	virtual void setCoupon(double _coupon) { this->coupon = _coupon; }
	virtual double getCoupon() const { return this->coupon; }
	virtual void addYearToDate() { this->maturityDate.year++; }
	virtual void BuyStocks(int _amount, int) { this->amount += _amount; }
	virtual void SellStocks(int _amount, int) { this->amount -= _amount; }
	virtual void setMaturityDate(Date _date) { this->maturityDate = _date; };
	virtual const Date& getMaturityDate() const { return this->maturityDate; };
	virtual Security* operator+(const Security& sec) const;
	virtual std::ostream& print(std::ostream& os) const;
	virtual ~Bond() {}
};

class State_Stock : public Share, public Bond
{
protected:
	bool publicTraded;

public:
	State_Stock(const std::string& _name, double _value, double _coupon, const Date& _date, bool _publicTraded) : Security(_name, _value), Share(_name, _value), Bond(_name, _value, _coupon, _date), publicTraded(_publicTraded) {}
	virtual void setPublic(bool _publicTraded) { this->publicTraded = _publicTraded; }
	virtual void BuyStocks(int _amount, int _total_amount);
	virtual void SellStocks(int _amount, int _total_amount);
	virtual bool getPublic() const { return this->publicTraded; }
	virtual Security* operator+(const Security& sec) const;
	virtual std::ostream& print(std::ostream& os) const;
	virtual ~State_Stock() {}
};


std::ostream& operator<<(std::ostream& os, const Security* stock);
std::ostream& operator<<(std::ostream& os, const Date& date);
#endif // !