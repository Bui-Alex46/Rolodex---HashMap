
/**
 * DO NOT EDIT THIS FILE
 */

//
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


//
#include "BookStore.hpp"
#include "DoublyLinkedList.hpp"
#include "puhp-tests/PuhPTests.hpp"


//
using std::cout, std::cin, std::endl;
using std::string;
using std::vector;


//
using PuhPTests::Tests;
using PuhPTests::Test;
using PuhPTests::OutputCapture;


//
using CPSC131::BookStore::BookStore, CPSC131::BookStore::Book;


//	PROTO
BookStore makeBookStore(bool empty = false);
std::vector<Book> makeBooks();
CPSC131::DoublyLinkedList::DoublyLinkedList<int> makeList();
bool checkBookVectorsEqual(const std::vector<Book>& v1, const std::vector<Book>& v2);
bool checkIntegerListsEqual(const CPSC131::DoublyLinkedList::DoublyLinkedList<int>& list1, const CPSC131::DoublyLinkedList::DoublyLinkedList<int>& list2);
//
template<class T>
void printList(CPSC131::DoublyLinkedList::DoublyLinkedList<T>& list);
//
//
void testPrinting(Tests& tests);
void testBasicLists(Tests& tests);
void testListAssignment(Tests& tests);
void testListReverse(Tests& tests);
void testIterators(Tests& tests);
void testStockSales(Tests& tests);


//
int main()
{
	//
	Tests tests(50);
	
	//
	std::vector<std::function<void(Tests&)>> fs = {
		testPrinting,
		testBasicLists,
		testListAssignment,
		testListReverse,
		testIterators,
		testStockSales,
	};
	
	//
	for ( auto f : fs ) {
		f(tests);
	}
	
	//
	tests.run();
	
	//
	cout << "Tests complete" << endl;
	
	return 0;
}

//
BookStore makeBookStore(bool empty)
{
	//
	BookStore store;
	
	//
	auto books = makeBooks();
	size_t quantity = 50;
	for ( Book b : books ) {
		
		//
		if ( !empty ) {
			b.adjustStockAvailable(quantity);
			quantity--;
		}
		
		store.purchaseInventory(b);
	}
	
	return store;
}

//
std::vector<Book> makeBooks()
{
	//
	std::vector<Book> v = {
		Book("Brainwashing is a Cinch!", "James Maratta", "B001NDNB7C", 900),
		Book("Catflexing - A Catlover's Guide to Weight Training, Aerobics, and Stretching", "Stephanie Jackson", "0898159407", 550),
		Book("How to Enjoy Your Weeds", "Audrey Wynne Hatfield", "080693042X", 1200),
		Book("How to Teach Quantum Physics to Your Dog", "Chad Orzel", "1416572295", 2000),
		Book("How to Train Goldfish Using Dolphin Training Techniques", "C. Scott Johnson", "0533112923", 100)
	};
	
	return v;
}

//
CPSC131::DoublyLinkedList::DoublyLinkedList<int> makeList()
{
	//
	CPSC131::DoublyLinkedList::DoublyLinkedList<int> list;
	
	//
	for ( size_t i = 0; i < 20; i++ ) {
		int value = (i * 3) - 30;
		list.push_back(value);
	}
	
	return list;
}

//
bool checkBookVectorsEqual(const std::vector<Book>& v1, const std::vector<Book>& v2)
{
	//
	if ( v1.size() != v2.size() ) {
		return false;
	}
	
	for ( size_t i = 0; i < v1.size(); i++ ) {
		if ( v1.at(i) != v2.at(i) ) {
			return false;
		}
	}
	
	return true;
}

//
bool checkIntegerListsEqual(CPSC131::DoublyLinkedList::DoublyLinkedList<int>& list1, CPSC131::DoublyLinkedList::DoublyLinkedList<int>& list2)
{
	//
	if ( list1.size() != list2.size() ) {
		return false;
	}
	
	//
	for ( size_t i = 0; i < list1.size(); i++ ) {
		if ( list1.at(i) != list2.at(i) ) {
			return false;
		}
	}
	
	return true;
}

//
template <class T>
void printList(CPSC131::DoublyLinkedList::DoublyLinkedList<T>& list)
{
	//
	auto node = list.head();
	size_t index = 0;
	
	//
	cout << "{";
	while (node != nullptr)
	{
		//
		if ( index > 0 ) {
			cout << ", ";
		}
		cout << index << ":" << node->getElement();
		
		//
		node = node->getNext();
		index++;
	}
	cout << "}" << endl;
}

//
void testPrinting(Tests& tests)
{
	//
	auto t = tests.createTest("Printing");
	
	//
	t->setFixedPointsPossible(2);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	t->assertNoException(
		[t]()
		{
			//
			auto store = makeBookStore();
			
			//
			OutputCapture cap;
			store.printInventory();
			cap.endCapture();
			
			//
			string expected = "*** Book Store Inventory ***\n"
				"\"Brainwashing is a Cinch!\", by James Maratta [B001NDNB7C] (50 in stock)\n"
				"\"Catflexing - A Catlover's Guide to Weight Training, Aerobics, and Stretching\", by Stephanie Jackson [0898159407] (49 in stock)\n"
				"\"How to Enjoy Your Weeds\", by Audrey Wynne Hatfield [080693042X] (48 in stock)\n"
				"\"How to Teach Quantum Physics to Your Dog\", by Chad Orzel [1416572295] (47 in stock)\n"
				"\"How to Train Goldfish Using Dolphin Training Techniques\", by C. Scott Johnson [0533112923] (46 in stock)\n"
				;
			
			//
			t->assertEqual(cap.getStdout(), expected, 1, "Basic store printout");
			
		}, 1, "Print tests"
	);
}

//
void testBasicLists(Tests& tests)
{
	//
	auto t = tests.createTest("List basics");
	
	//
	t->setFixedPointsPossible(15);
	t->setNormalizedPointsPossibleTarget(10);
	
	//
	t->assertNoException(
		[t]()
		{
			//
			{
				//
				auto list = makeList();
				
				//	Check some values
				t->assertEqual(
					list.at(0), static_cast<int>(-30),
					1, "Checking random access"
				);
				t->assertEqual(
					list.at(3), static_cast<int>(-21),
					1, "Checking random access"
				);
				t->assertEqual(
					list.at(16), static_cast<int>(18),
					1, "Checking random access"
				);
				
				//	Size
				t->assertEqual(
					list.size(), static_cast<size_t>(20),
					1, "Checking size"
				);
				
				//	Add a couple items at random access
				list.insert_after(7, 119);
				list.insert_after(13, 181);
				t->assertEqual(
					list.size(), static_cast<size_t>(22),
					1, "Checking size after arbitrary insertions"
				);
				t->assertEqual(
					list.at(8), static_cast<int>(119),
					1, "Checking arbitrary insertion"
				);
				t->assertEqual(
					list.at(14), static_cast<int>(181),
					1, "Checking arbitrary insertion"
				);
				
				//	Push back and push front
				list.push_front(1011);
				list.push_back(1012);
				t->assertEqual(
					list.size(), static_cast<size_t>(24),
					1, "Checking size after push front/back"
				);
				t->assertEqual(
					list.at(0), static_cast<int>(1011),
					1, "Checking value after push front"
				);
				t->assertEqual(
					list.at(list.size() - 1), static_cast<int>(1012),
					1, "Checking value after push back"
				);
				
				//	Check the front and back
				t->assertEqual(
					list.front(), static_cast<int>(1011),
					1, "Checking value at front()"
				);
				t->assertEqual(
					list.back(), static_cast<int>(1012),
					1, "Checking value at back()"
				);
			}
			
		}, 1, "Basic add/remove tests"
	);
	
	//
	t->assertNoException(
		[t]()
		{
			{
				CPSC131::DoublyLinkedList::DoublyLinkedList<int> list;
				list.assign(6, 999);
				bool b = true;
				for ( size_t i = 0; i < 6; i++ ) {
					if ( list.at(i) != 999 ) {
						b = false;
					}
				}
				if ( list.size() != 6 ) {
					b = false;
				}
				t->assertTrue(b, 1, "Checking assign() method");
			}
			
		}, 1, "Checking assign()"
	);
}

//
void testListAssignment(Tests& tests)
{
	//
	auto t = tests.createTest("List assignment");
	
	//
	t->setFixedPointsPossible(7);
	t->setNormalizedPointsPossibleTarget(10);
	
	//
	t->assertNoException(
		[t]()
		{
			//
			{
				//
				CPSC131::DoublyLinkedList::DoublyLinkedList<int> list;
				list.push_front(19);
				list.push_front(12);
				list.push_front(-129878);
				list.push_front(228764364);
				list.push_front(119991111);
				list.push_front(0);
				list.push_front(1900000000);
				
				//
				auto list2 = list;
				t->assertTrue(checkIntegerListsEqual(list, list2), 1, "Assignment operator should work");
				
				//
				CPSC131::DoublyLinkedList::DoublyLinkedList<int> list3(list);
				t->assertTrue(checkIntegerListsEqual(list, list3), 1, "Copy constructore should work");
				
				//	Check comparison
				t->assertTrue(list == list2 && list2.size() > 0, 1, "Comparison operator check");
				t->assertFalse(list != list2 || list2.size() == 0, 1, "Inequality operator check");
				
				//	Make list3 different, then recheck comparisons
				list3.push_back(0);
				t->assertFalse(list == list3 || list3.size() == 0, 1, "Comparison operator check");
				t->assertTrue(list != list3 && list3.size() > 0, 1, "Inequality operator check");
			}
			
		}, 1, "List assignment tests"
	);
}

//
void testListReverse(Tests& tests)
{
	//
	auto t = tests.createTest("List reverse");
	
	//
	t->setFixedPointsPossible(3);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	t->assertNoException(
		[t]()
		{
			{
				auto list1 = makeList();
				auto list2 = list1;
				
				list2.clear();
				list2.push_back(27);
				list2.push_back(24);
				list2.push_back(21);
				list2.push_back(18);
				list2.push_back(15);
				list2.push_back(12);
				list2.push_back(9);
				list2.push_back(6);
				list2.push_back(3);
				list2.push_back(0);
				list2.push_back(-3);
				list2.push_back(-6);
				list2.push_back(-9);
				list2.push_back(-12);
				list2.push_back(-15);
				list2.push_back(-18);
				list2.push_back(-21);
				list2.push_back(-24);
				list2.push_back(-27);
				list2.push_back(-30);
				
				list1.reverse();
				
				t->assertTrue(
					list1.size() == 20 && list1 == list2, 2, "Correctly reverse a list"
				);
			}
		}, 1, "Reverse tests"
	);
}

//
void testIterators(Tests& tests)
{
	//
	auto t = tests.createTest("Iterators");
	
	//
	t->setFixedPointsPossible(10);
	t->setNormalizedPointsPossibleTarget(10);
	
	//
	t->assertNoException(
		[t]()
		{
			{
				//
				auto list = makeList();
				auto itr = list.begin();
				
				//
				itr++;
				t->assertEqual(
					(*itr), static_cast<int>(-27),
					1, "Postfix iterator increment"
				);
				++itr;
				t->assertEqual(
					(*itr), static_cast<int>(-24),
					1, "Prefix iterator increment"
				);
				
				//
				itr += 10;
				t->assertEqual(
					(*itr), static_cast<int>(6),
					1, "Iterator addition assignment"
				);
				
				//
				itr -= 4;
				t->assertEqual(
					(*itr), static_cast<int>(-6),
					1, "Iterator subtraction assignment"
				);
				
				//
				itr--;
				t->assertEqual(
					(*itr), static_cast<int>(-9),
					1, "Postfix iterator decrement"
				);
				--itr;
				t->assertEqual(
					(*itr), static_cast<int>(-12),
					1, "Prefix iterator decrement"
				);
				
				//	Assign and compare iterators
				auto itr2 = itr;
				t->assertTrue(
					itr == itr2,
					1, "Iterator assignment and comparison operators"
				);
				itr2++;
				t->assertTrue(
					itr != itr2,
					1, "Iterators should not be equal after has is advanced"
				);
				
				//	Try to get the cursor out-of-bounds
				itr += 5000;
				t->assertTrue(
					itr == list.end(),
					1, "Depleted iterator should become end()"
				);
			}
			
		}, 1, "Iterator tests"
	);
}

//
void testStockSales(Tests& tests)
{
	//
	auto t = tests.createTest("Stock/Sales manipulation");
	
	//
	t->setFixedPointsPossible(13);
	t->setNormalizedPointsPossibleTarget(10);
	
	//
	t->assertNoException(
		[t]()
		{
			//
			{
				//
				auto store = makeBookStore();
				
				//	Check initial balance
				//	(50×900)+(49×550)+(48×1200)+(47×2000)+(46×100) == -228150
				t->assertEqual(
					store.getAccountBalance(), static_cast<int>(-228150),
					1, "Correct balance after adding initial books"
				);
				
				//	Add additional book to check balance
				//	-228150 - (999*3) == -231147
				store.purchaseInventory(Book("Ninja Mind Control", "Ashida Kim", "1435769244", 999, 3));
				t->assertEqual(
					store.getAccountBalance(), static_cast<int>(-231147),
					1, "Correct balance after adding new book"
				);
				
				//	Check stock on the new book
				t->assertEqual(
					store.getBookStockAvailable("1435769244"), static_cast<size_t>(3),
					1, "Correct stock on newly added book"
				);
				
				//	Add more of an existing book to check balance
				//	-231147 - (1000*2) == -233147
				store.purchaseInventory(Book("Bla", "Bla", "1435769244", 1000, 2));
				t->assertEqual(
					store.getAccountBalance(), static_cast<int>(-233147),
					1, "Correct balance after adding more of an existing book at a different price"
				);
				t->assertEqual(
					store.getBookStockAvailable("1435769244"), static_cast<size_t>(5),
					1, "Correct stock after adding 2 more"
				);
				
				//	Increase funds after customer purchases one book
				//store.printInventory();
				store.sellToCustomer("1435769244", 2000, 3);
				t->assertEqual(
					store.getAccountBalance(), static_cast<int>(-227147),
					1, "Correct balance after selling some books"
				);
			}
			
			{
				//	Starts with balance -228150
				auto store = makeBookStore();
				
				//	Adjust balance
				store.adjustAccountBalance(12983);
				t->assertEqual(
					store.getAccountBalance(), static_cast<int>(-215167),
					1, "Adjust account balance"
				);
				store.adjustAccountBalance(-228);
				t->assertEqual(
					store.getAccountBalance(), static_cast<int>(-215395),
					1, "Adjust account balance"
				);
				
				//	Check inventory
				t->assertTrue(
					store.bookExists("0533112923"), 1, "Calling bookExists() on existing book"
				);
				t->assertFalse(
					store.bookExists("053zzz3112923"), 1, "Calling bookExists() on non-existing book"
				);
			}
			
		}, 2, "Account balance tests"
	);
	
	//	Separate test to check overselling so we don't run into the "dead local reference".
	//	that happens when trying to nest tests too deeply.
	//	We should see an exception if we try to oversell a book.
	t->assertException(
		[t]()
		{
			//
			auto store = makeBookStore();
			
			//	Purchase some inventory to avoid default values
			store.purchaseInventory(Book("Ninja Mind Control", "Ashida Kim", "1435769244", 999, 3));
			store.purchaseInventory(Book("Bla", "Bla", "1435769244", 1000, 2));
			
			//store.printInventory();
			store.sellToCustomer("1435769244", 2000, 6);
		}, 1, "Should see an exception if we try to oversell a book."
	);
	
}





