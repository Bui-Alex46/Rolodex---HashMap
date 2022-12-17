
/**
 * You probably shouldn't edit this file.
 * It will not be graded, and provides a quick standard way to test your code.
 * 
 * If you wish to write a custom program while you develop,
 * 	use the sandbox.cpp file instead.
 */

//
#include "MyRolodex.hpp"


//
#include <climits>
#include <iostream>
#include <string>


//
using CPSC131::MyRolodex::MyRolodex;
using std::cin, std::cout, std::endl;
using std::string, std::to_string;


//	Proto
string askKey();
void addEntry(MyRolodex& dex);
void fetchEntry(const MyRolodex& dex);
void removeEntry(MyRolodex& dex);
void existsEntry(const MyRolodex& dex);


//
int main()
{
	//
	cout << "Welcome to the MyRolodex Text User Interface!" << endl;
	
	//
	cout << "Brought to you by Niles Peppertrout" << endl;
	cout << endl;
	
	//
	MyRolodex dex;
	string s;
	while (true)
	{
		//
		cout
			<< "*** MyRolodex ***" << endl
			<< "1. Add an entry" << endl
			<< "2. Fetch an entry" << endl
			<< "3. Remove an entry" << endl
			<< "4. Check whether an entry exists" << endl
			<< endl
			<< "(q to quit)" << endl
			<< "===> "
			;
		cin >> s;
		cout << "Your input: " << s << endl;
		cin.ignore(INT_MAX, '\n');
		
		//
		if ( s == "q" || s == "Q" ) {
			break;
		}
		
		//
		try
		{
			if ( s == "1" ) {
				addEntry(dex);
			}
			else if ( s == "2" ) {
				fetchEntry(dex);
			}
			else if ( s == "3" ) {
				removeEntry(dex);
			}
			else if ( s == "4" ) {
				existsEntry(dex);
			}
			else {
				cout << "Invalid input!" << endl;
			}
		}
		catch(const std::exception& e)
		{
			cout << "Whoops! Caught an exception!" << endl;
			cout << "==> " << e.what() << endl;
		}
		
		cout << endl;
	}
	
	//
	cout << "Thank you for using this program. Goodbyte!" << endl;
	
	return 0;
}

string askKey()
{
	string key;
	
	cout << "Please enter a name: ";
	std::getline(cin, key);
	
	return key;
}

void addEntry(MyRolodex& dex)
{
	string key = askKey();
	
	string line1, line2, city, state, country;
	int zip;
	
	cout << "Enter address line 1: ";
	std::getline(cin, line1);
	
	cout << "Enter address line 2 (or enter none): ";
	std::getline(cin, line2);
	
	cout << "Enter the city: ";
	std::getline(cin, city);
	
	cout << "Enter the state: ";
	std::getline(cin, state);
	
	cout << "Enter the zip: ";
	cin >> zip;
	cin.ignore(INT_MAX, '\n');
	
	cout << "Enter the country code: ";
	std::getline(cin, country);
	
	dex.add(key, line1, line2, city, state, zip, country);
}

void fetchEntry(const MyRolodex& dex)
{
	string key = askKey();
	cout << dex.get(key).toString();
}

void removeEntry(MyRolodex& dex)
{
	string key = askKey();
	dex.remove(key);
}

void existsEntry(const MyRolodex& dex)
{
	string key = askKey();
	
	cout << "That person " << (dex.exists(key) ? "exists" : "does not exist") << " in the rolodex" << endl;
}








