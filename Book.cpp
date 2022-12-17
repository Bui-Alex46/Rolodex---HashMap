

/**
 * You do not need to edit this file,
 * 	though you can if you wish to add additional helper methods
 */


//
#include "Book.hpp"


//
#include <iostream>
#include <string>


//
using std::cout, std::cin, std::endl;
using std::string;


//
namespace CPSC131::BookStore
{
	//
	Book::Book()
	{
		cout << "Empty Book created" << endl;
	}
	
	Book::Book(std::string title, std::string author, std::string isbn, size_t price_cents, size_t stock_available)
		: title_(title), author_(author), isbn_(isbn), price_cents_(price_cents), stock_available_(stock_available)
	{}
	
	std::string Book::getTitle() const { return this->title_; }
	std::string Book::getAuthor() const { return this->author_; }
	std::string Book::getIsbn() const { return this->isbn_; }
	size_t Book::getPriceCents() const { return this->price_cents_; }
	
	void Book::adjustStockAvailable(int adjustment)
	{
		this->stock_available_ += adjustment;
	}
	size_t Book::getStockAvailable() const { return this->stock_available_; }
	
	bool Book::operator ==(const Book& other) const
	{
		if (
			this->title_ == other.title_
			&& this->author_ == other.author_
			&& this->isbn_ == other.isbn_
			&& this->price_cents_ == other.price_cents_
			&& this->stock_available_ == other.stock_available_
		)
		{
			return true;
		}
		return false;
	}
	bool Book::operator !=(const Book& other) const
	{
		return !this->operator ==(other);
	}
}




