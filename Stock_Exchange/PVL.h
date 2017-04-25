/*************************************************
* This file is part of the Stock-Exchange Project. *
* Notes: - *
* Version: 0.84 *
***************************************************/

#ifndef _PVL_H
#define _PVL_H

#include <string>
#include <sstream>

enum commands {
	Passive = -1,
	Exit, 
	Add_Company,
	Merge_Companies,
	Modifiy_Company,
	Remove_Company, 
	Sort_By_Setting,
	Print_Company, 
	Print_All_Companies,
	BuyStocks,
	SellStocks,
	Modify_Stock,
	Add_experation,
	Calculate_All_Stocks_Worth, 
	Print_Stock,
	Print_All_Stock,
	Show_Graph,
	GUI,
	Print_Information
};

namespace pvl
{
	template<typename T>
	std::string convertToString(T value)
	{
		std::ostringstream strs;
		strs << value;
		return strs.str();
	}
	std::string padString(const std::string& data, size_t width, char filler = ' ');
}
#endif // !