
/**
 * TODO: Implement all the BookStore methods below.
 * Note you'll have to remain inside the CPSC131::BookStore namespace.
 */

//
#include "BookStore.hpp"


//
#include <iostream>
#include <sstream>
#include <string>


//
using std::cout, std::cin, std::endl;
using std::string, std::to_string;
using std::stringstream;


//
namespace CPSC131::BookStore
{

	/// Your welcome
	BookStore::BookStore() {}
	
	/**
	 * Adjust the store's account balance
	 * Should accept positive or negative adjustments
	 */
	void BookStore::adjustAccountBalance(int adjustment)
	{
		account_balance_ += adjustment;
	}
	
	/**
	 * Return the store's current account balance
	 */
	int BookStore::getAccountBalance() {
		return account_balance_;
	}
	/**
	 * Find a book by its ISBN
	 * 
	 * Return this->bookList.end() if the book isn't found.
	 * 
	 * Return an interator pointing to the Book if it is found.
	 */
	DoublyLinkedList::DoublyLinkedList<Book>::Iterator BookStore::findBook(std::string isbn)
	{
		DoublyLinkedList::DoublyLinkedList<CPSC131::BookStore::Book>::Iterator it(bookList.head(), bookList.tail(), bookList.head());
		
		for(auto it = bookList.begin(); it != bookList.end(); it++){		//Traversing through the list
				if( it.getCursor() -> getElement().getIsbn() == isbn){		//Checks if isbn is found
					return it;
				}
			}
		
			return this -> bookList.end();
		}
	
	/**
	 * Check whether a book exists, by its ISBN
	 * 
	 * Return true if it exists, or false otherwise
	 */
	bool BookStore::bookExists(std::string isbn)
	{
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it1(bookList.head(), bookList.tail(), bookList.head());
		for(auto it1 = bookList.begin(); it1 != bookList.end(); it1++){
			if( it1.getCursor() -> getElement().getIsbn() == isbn){
				return true;
			}
		}
		
			return false;
	}
	
	/**
	 * Check the quantity of stock we have for a particular book, by ISBN
	 * 
	 * If the book doesn't exist, just return 0
	 */
	size_t BookStore::getBookStockAvailable(std::string isbn)
	{
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it2(bookList.head(), bookList.tail(), bookList.head());
		for(auto it2 = bookList.begin(); it2 != bookList.end(); it2++){
			if( it2.getCursor() -> getElement().getIsbn() == isbn){
				return it2.getCursor() -> getElement().getStockAvailable();
			}
		}
		return 0;
	}
	
	/**
	 * Locate a book by ISBN and return a reference to the Book
	 * 
	 * If the book doesn't exist, throw an exception
	 */
	Book& BookStore::getBook(std::string isbn)
	{
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it3(bookList.head(), bookList.tail(), bookList.head());
		for(auto it3 = bookList.begin(); it3 != bookList.end(); it3++){
			if( it3.getCursor() -> getElement().getIsbn() == isbn){
				return it3.getCursor() -> getElement();
			}
		}
		throw("Can't find book");
	}
	
	/**
	 * Take a Book instance and add it to inventory
	 * 
	 * If the book's ISBN already exists in our store,
	 * 	simply adjust account balance by the book's price and quantity,
	 * 	but ignore other details like title and author.
	 * 
	 * If the book's ISBN doesn't already exist in our store,
	 * 	adjust our account balance and push the book into our list
	 */
	void BookStore::purchaseInventory(const Book& book)
	{
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it4 = findBook(book.getIsbn());
		
		if(it4.getCursor()!=nullptr){
			it4.getCursor() -> getElement().adjustStockAvailable(book.getStockAvailable());
		}
		else{
			bookList.push_back(book);
		}
		if(bookExists(book.getIsbn())){
			adjustAccountBalance(-(book.getPriceCents() * book.getStockAvailable()));
		}
		
	}
	
	/**
	 * Take some book details and add the book to our inventory.
	 * 
	 * Use the same rules as the other overload for this function.
	 * 
	 * You might want to avoid repeating code by simply building a Book
	 * 	object from the details, then calling the other overload
	 * 	with the new Book object.
	 */
	void BookStore::purchaseInventory(
		std::string title, std::string author, std::string isbn,
		size_t price_cents,
		size_t unit_count
	)
	{
		Book book1(title, author, isbn, price_cents, unit_count);
		purchaseInventory(book1);
	}
	
	/**
	 * Print out inventory.
	 * Should be in a particular way to earn unit test points.
	 * Example (ignore the asterisks at the left of this comment block):
	 * 
	 * *** Book Store Inventory ***
	 * "Book1", by Author1 [123] (5 in stock)
	 * "Book2", by Author2 [456] (19 in stock)
	 * 
	 * Should print a trailing std::endl after the last book line, for the unit tests
	 */
	void BookStore::printInventory()
	{
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it(bookList.head(), bookList.tail(), bookList.head());
		cout << "*** Book Store Inventory ***" << std::endl;
		for(auto it = bookList.begin(); it != bookList.end(); it++){
			cout <<"\"" << it.getCursor() -> getElement().getTitle() << "\", by "
			<<it.getCursor() -> getElement().getAuthor() << " ["
			<< it.getCursor() -> getElement().getIsbn() << "] (" 
			<<it.getCursor() -> getElement().getStockAvailable() << " in stock)" << std::endl;
		}

	}
	
	/**
	 * Sell a book to a customer!
	 * 
	 * Takes the ISBN of the book, the selling price of the book, and the quantity of books sold
	 * 
	 * Uses the same rules as the other overload.
	 * 
	 * You may wish to just grab a reference to the book and call the other overload,
	 * 	to avoid repeating code
	 */
	void BookStore::sellToCustomer(std::string isbn, size_t price_cents, size_t quantity)
	{
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it = findBook(isbn);
		if(quantity > it.getCursor() -> getElement().getStockAvailable()){
			throw ("Not enough books in the store");
		}
			it.getCursor() -> getElement().adjustStockAvailable(-quantity);
			adjustAccountBalance(quantity * price_cents);
	}
	
	/**
	 * Sell a book to a customer!
	 * 
	 * Takes a Book reference, the selling price of the book, and the quantity of books sold
	 * 
	 * If we don't have enough of this book in stock for the quantity the customer wants to purchase,
	 * 	throw an std::range_error
	 * 
	 * Otherwise, adjust the stock available in our store, and update our account balance.
	 */
	void BookStore::sellToCustomer(Book& book, size_t price_cents, size_t quantity)
	{
		if(quantity > book.getStockAvailable()){
			throw("Not enough books");
		}
			book.adjustStockAvailable(-quantity);
			adjustAccountBalance(quantity * price_cents);
	}
}







