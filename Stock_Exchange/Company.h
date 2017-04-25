/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/
#ifndef _COMPANY_H
#define _COMPANY_H

#include <string>
class Security;

enum CompanyType { Type_Private = 1, Type_Goverment = 2, Type_Goverment_owned = 3 };

class ExpDifferentCompaniesType : public std::exception
{
public:
	virtual const char* what() const throw() { return "Cant merge 2 comapnies of different types!"; }
};

class Company
{
	std::string name;
	std::string industry;
	CompanyType type;
	double value;
	int amount;
public:
	Company(const std::string& _name, const std::string& _industry, CompanyType _type, double _value, int _amount)
	{
		this->name = _name;
		this->industry = _industry;
		this->type = _type;
		this->value = _value;
		this->amount = _amount;
	}
	void setName(const std::string& _name){this->name = _name;}
	const std::string& getName() const { return this->name; }
	void setValue(double _value){this->value = _value;}
	void setAmount(int _amount){this->amount = _amount;}
	void setIndustry(const std::string& _industry) { this->industry = _industry; }
	CompanyType getType() const{return type;}
	double getValue() const{return value;}
	int getAmount() const{return amount;}
	Security* IssueNewStock() const;
	const std::string& getIndustry() const { return industry; }
	Company operator+(const Company& company) const;
	std::ostream& print(std::ostream& os) const;
	bool operator==(const std::string& other) const { return (this->name == other); }
};

std::ostream& operator<<(std::ostream& os, const Company& company);

#endif // !