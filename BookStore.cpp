

#include "BookStore.hpp"



#include <iostream>
#include <sstream>
#include <string>



using std::cout, std::cin, std::endl;
using std::string, std::to_string;
using std::stringstream;



namespace CPSC131::BookStore
{


	BookStore::BookStore() {}
	
	
	void BookStore::adjustAccountBalance(int adjustment)
	{
		account_balance_ += adjustment;
	}
	
	
	int BookStore::getAccountBalance() {
		return account_balance_;
	}
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
	
	void BookStore::purchaseInventory(
		std::string title, std::string author, std::string isbn,
		size_t price_cents,
		size_t unit_count
	)
	{
		Book book1(title, author, isbn, price_cents, unit_count);
		purchaseInventory(book1);
	}
	
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
	
	void BookStore::sellToCustomer(std::string isbn, size_t price_cents, size_t quantity)
	{
		DoublyLinkedList::DoublyLinkedList<Book>::Iterator it = findBook(isbn);
		if(quantity > it.getCursor() -> getElement().getStockAvailable()){
			throw ("Not enough books in the store");
		}
			it.getCursor() -> getElement().adjustStockAvailable(-quantity);
			adjustAccountBalance(quantity * price_cents);
	}
	
	void BookStore::sellToCustomer(Book& book, size_t price_cents, size_t quantity)
	{
		if(quantity > book.getStockAvailable()){
			throw("Not enough books");
		}
			book.adjustStockAvailable(-quantity);
			adjustAccountBalance(quantity * price_cents);
	}
}







