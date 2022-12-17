
/**
 * You don't need to edit this file, although you can if you wish.
 * This source file will not be graded.
 * 
 * This is a live TUI for the BookStore you're working on. You can
 * use it to manually play around in your store.
 */

//
#include "BookStore.hpp"


//
#include <iostream>
#include <string>


//
using std::cout, std::cin, std::endl;
using std::string;


//
using CPSC131::BookStore::BookStore, CPSC131::BookStore::Book;


//	PROTO
void menuLoop();
void purchaseInventory();
void viewInventory();
void sellToCustomer();
string getLine();

//	Globals: Not a great practice, but very good for a lazy professor making starter code :)
BookStore store;

	//  int CustomFunction(){
	// 	return 0;
	// }
		
//
int main()
{
	//
	menuLoop();
	
	//
	cout << "Program exiting" << endl;
	
	return 0;
}

	//  void CustomFunction(){
	// 	Node a;

	//  }

//
void menuLoop()
{
	//
	while (true)
	{
		//
		auto balance = store.getAccountBalance();
		
		//
		cout
			<< endl << endl << endl
			<< "*** Book Store - Main Menu ***" << endl
			<< endl
			<< "Account balance: " << balance << " cents" << (balance >= 0 ? "" : " (Oh nyo)") << endl
			<< endl
			<< "1. Purchase inventory (add Book to store)" << endl
			<< "2. View inventory" << endl
			<< "3. Sell to customer" << endl
			<< endl
			<< "Q. Quit" << endl
			<< endl
			<< "Enter your selection ==> "
			;
		
		//
		string choice = getLine();
		cout << endl;
		
		//
		if ( choice == "1" ) {
			purchaseInventory();
		}
		else if ( choice == "2" ) {
			viewInventory();
		}
		else if ( choice == "3" ) {
			sellToCustomer();
		}
		else if ( choice == "Q" || choice == "q" ) {
			break;
		}
		else {
			cout << "Invalid choice: " << choice << endl;
		}
	}
}

//
void purchaseInventory()
{
	string title, author, isbn;
	size_t price_cents, quantity;
	
	//
	cout
		<< "Purchasing inventory ..." << endl
		<< endl
		<< "Enter book ISBN: "
		;
	isbn = getLine();
	
	//
	if ( !store.bookExists(isbn) ) {
		cout << "Enter title: ";
		title = getLine();
		cout << "Enter author: ";
		author = getLine();
	}
	
	//
	cout << "Enter wholesale price (in cents): ";
	price_cents = stoi(getLine());
	cout << "Enter quantity: ";
	quantity = stoi(getLine());
	
	//
	store.purchaseInventory(
		title, author, isbn,
		price_cents,
		quantity
	);
}


//
void viewInventory()
{
	store.printInventory();
}


//
void sellToCustomer()
{
	cout << "Selling to customer ..." << endl;
	
	string isbn;
	cout << "Enter the ISBN of the book to sell: ";
	isbn = getLine();

	size_t price;
	cout << "Enter the price per copy, in cents: ";
	price = stoi(getLine());
	
	size_t quantity;
	cout << "Enter the quantity to sell: ";
	quantity = stoi(getLine());
	
	try
	{
		store.sellToCustomer(isbn, price, quantity);
		cout << "Sale was successful" << endl;
		cout << "Sold " << quantity << " copies of ISBN:" << isbn << " at " << price << " cents each" << endl;
	}
	catch( const std::exception& e )
	{
		cout << "Failed to sell to customer: " << e.what() << endl;
	}
}

//
string getLine()
{
	const size_t BUFFER_SIZE = 8192;
	char buffer[BUFFER_SIZE];
	
	cin.clear();
	cin.getline(buffer, BUFFER_SIZE);
	
	string s = buffer;
	
	return s;
}







