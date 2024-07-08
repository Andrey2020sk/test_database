#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <set>
#include <vector>
#include <exception>
//----------------------------------------------------------------------------------------------------
void ParseCommand(const std::string & command)
{
	std::stringstream str(command);
	std::vector<std::string> inputs;
	std::string temp;
	while(str>>temp)
	{
		inputs.push_back(temp);
	}
}



class Date
{
public:
	Date() : year(1900), month(01), day(01) { }

	Date(int new_year, int new_month, int new_day)
	{
		if (new_month > 12 || new_month < 1)
		{
			std::string error = "Month value is invalid: " + std::to_string(new_month);
			throw std::logic_error(error);
		}
		else
		{
			month = new_month;
		}

		if (new_day > 31 || new_day < 1)
		{
			std::string error = "Day value is invalid: " + std::to_string(new_day);
			throw std::logic_error(error);
		}
		else
		{
			day = new_day;
		}
		year = new_year;
	}

	int GetYear() const
	{
		return year;
	}

	int GetMonth() const
	{
		return month;
	}

	int GetDay() const
	{
		return day;
	}
private:
	int year;
	int month;
	int day;
};

std::istream & operator >> (std::istream & is, Date & date)
{
	std::string str_date;
	is >> str_date;

	std::stringstream str_stream_date(str_date);

	int year = -1;
	str_stream_date >> year;

	if (str_stream_date.fail() || str_stream_date.peek() != '-')
	{
		throw std::logic_error("Wrong date format: " + str_date);
	}
	str_stream_date.ignore();

	int month = -1;
	str_stream_date >> month;

	if (str_stream_date.fail() || str_stream_date.peek() != '-' )
	{
		throw std::logic_error("Wrong date format: " + str_date);
	}
	str_stream_date.ignore();

	int day = -1;
	str_stream_date >> day;

	if (str_stream_date.fail() || !str_stream_date.eof())
	{
		throw std::logic_error("Wrong date format: " + str_date);
	}
	date = Date(year, month, day);
	return is;
}
//----------------------------------------------------------------------------------------------------
bool operator < (const Date & lhs, const Date & rhs)
{
	if (lhs.GetYear() == rhs.GetYear())
	{
		if (lhs.GetMonth() == rhs.GetMonth())
		{
			return lhs.GetDay() < rhs.GetDay();
		}
		return lhs.GetMonth() < rhs.GetMonth();
	}
	return lhs.GetYear() < rhs.GetYear();
}
//----------------------------------------------------------------------------------------------------
class Database
{
public:
	Database(){}
	Database(std::map<Date, std::set<std::string>> DB)
	{
		db=DB;
	}
	void AddEvent(const Date & date, const std::string & event)
	{
		db[date].insert(event);
	}

	bool DeleteEvent(const Date & date, const std::string & event)
	{
		if (db.count(date) != 0 && db[date].count(event) != 0)
		{
			db[date].erase(event);
			return true;
		}
		return false;
	}

	int DeleteDate(const Date & date)
	{
		const int count = db[date].size();
		if (count != 0)
		{
			db.erase(date);
		}
		return count;
	}

	std::set<std::string> FindDate(const Date & date) const
	{
		if (db.count(date) != 0)
		{
			return db.at(date);
		}
		return {};
	}

	std::map<Date,std::set<std::string>>Find(const std::string & command)
	{
		std::map<Date,std::set<std::string>>result;
		std::stringstream str(command);
		std::vector<std::string> inputs;
		std::string temp;
		while(str>>temp)
		{
			inputs.push_back(temp);
		}

		std::stringstream check_date(inputs[0]);
		Date date;
		try
		{
			check_date>>date;
			FindDate(date);
			return 0;
		}catch(std::logic_error & e)
		{
			if(inputs[3]=="AND"||inputs[3]=="OR")
			{

			}
			else
			{
				if(inputs[0]=="event")
				{
					for (const auto &pair:db)
					{
						auto set0 = pair.second;
						if(inputs[1]=="!=")
						{
							if(set0.find(inputs[2])!=end(set0))
							{
								set0.erase(inputs[2]);
							}
							result[pair.first]=set0;
						}
						else if(inputs[1]=="=")
						{
							if(set0.find(inputs[2])!=end(set0))
							{
								result[pair.first]={inputs[2]};
							}
						}

					}
				}
				else if(inputs[0]=="date")
				{
					std::stringstream datestr(inputs[2]);
					Date date;
					datestr>>date;
					if(inputs[1]=="<")
					{

					}
				}
			}
		}

	}
	/*std::map<Date,std::set<std::string>>Find_condition(std::string condition)//(const std::string & first, const std::string & oper,const std::string & last)const
	{
		std::map<Date,std::set<std::string>>result;
		std::istringstream s(condition);
		std::string first,oper,last;
		s>>first;
		s>>oper;
		s>>last;
		Date date;
		if(first=="event")
		{

				for (const auto &pair:db)
				{
					auto set0 = pair.second;
					if(oper=="!=")
					{
						if(set0.find(last)!=end(set0))
						{
							set0.erase(last);
						}
						result[pair.first]=set0;
					}
					else if(oper=="=")
					{
						if(set0.find(last)!=end(set0))
						{
							result[pair.first]={last};
						}
					}

				}


		}
		else if(first=="date")
		{

		}
		return result;
	}
	*/
	void Print() const
	{
		for (const auto & pair : db)
		{
			for (const auto & event : pair.second)
			{
				std::cout << std::setw(4) << std::setfill('0') << pair.first.GetYear() << '-';
				std::cout << std::setw(2) << std::setfill('0') << pair.first.GetMonth() << '-';
				std::cout << std::setw(2) << std::setfill('0') << pair.first.GetDay();
				std::cout << " " << event << std::endl;
			}
		}
	}
private:
	std::map<Date, std::set<std::string>> db;
};

std::map<Date, std::set<std::string>> OR(std::map<Date, std::set<std::string>>& db1, const std::map<Date, std::set<std::string>>& db2)
{
	std::map<Date, std::set<std::string>> result;
	for (auto pair:db2)
	{
		db1[pair.first].insert((pair.second).begin(),(pair.second).end());
	}
	return db1;
}
//----------------------------------------------------------------------------------------------------
int main()
{
	try
	{
		Database db;

		std::string command;
		while (getline(std::cin, command))
		{

			std::stringstream str_stream_cmd(command);
			std::string operation;
			str_stream_cmd >> operation;

			if (operation == "Add")
			{
				Date date_str;
				str_stream_cmd >> date_str;
				std::string full_event,event_str;
				//std::string event_str=command.substr();
				while(str_stream_cmd >> event_str)
				{full_event+=(" "+event_str);}

				db.AddEvent(date_str, full_event.substr(1,full_event.length()-1));
			}
			else
			if (operation == "Del")
			{
				Date date_str;
				str_stream_cmd >> date_str;

				if (!str_stream_cmd.eof())
				{
					std::string event_str;
					str_stream_cmd >> event_str;

					if (db.DeleteEvent(date_str, event_str))
					{
						std::cout << "Deleted successfully" << std::endl;
					}
					else
					{
						std::cout << "Event not found" << std::endl;
					}
				}
				else
				{
					const int count = db.DeleteDate(date_str);
					std::cout << "Deleted " << count << " events" << std::endl;
				}
			}
			else
			if (operation == "Find")
			{
				std::string instruction;
				str_stream_cmd>>instruction;
				if(instruction=="event"||instruction=="date")
				{
					 std::string condition = command.substr(5,command.length()-5);
					 auto result = db.Find_condition(condition);
					 Database new_db(result);
					 new_db.Print();

				}
				else{
				Date date_str;
				str_stream_cmd >> date_str;

				for (const auto & event : db.Find(date_str))
				{
					std::cout << event << std::endl;
				}
				}
			}
			else
			if (operation == "Print")
			{
				db.Print();
			}
			else
			if (operation.empty())
			{

			}
			else
			{
				throw std::logic_error("Unknown command: " + operation);
			}
		}
	}
	catch (const std::exception & exc)
	{
		std::cout << exc.what();
	}
	return 0;
}

