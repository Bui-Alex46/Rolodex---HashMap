
/**
 * DO NOT EDIT THIS FILE
 */

//
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>


//
#include "MyHashTable.hpp"
#include "MyRolodex.hpp"
#include "puhp-tests/PuhPTests.hpp"


//
using std::cout, std::cin, std::endl;
using std::ifstream;
using std::string, std::to_string;
using std::tuple;
using std::vector;

//
typedef int LEADERBOARD_VTYPE;

//
using PuhPTests::Tests;
using PuhPTests::Test;
using PuhPTests::OutputCapture;
using PuhPTests::Timer;
using PuhPTests::Complexity;
using PuhPTests::Random;
using PuhPTests::Leaderboard;
using PuhPTests::LeaderboardSortDirection;


//	Include student stuff
using CPSC131::MyHashTable::MyHashTable;
using CPSC131::MyRolodex::MyRolodex;
using CPSC131::MyRolodex::Address;


//	PROTO
void preventPrototypeTampering();
void loadWords();
string increaseWordLength(string word);
MyHashTable<string> createWordTable(size_t capacity = 1000000, bool longWords = false, bool shortDict = false);
//
void testHashingFunctions(Tests& tests);
void testTableManipulation(Tests& tests);
void testTableComplexity(Tests& tests);
void testRolodex(Tests& tests);
void testLeaderboard(Tests& tests);


//	Globals
string g_words_filename = "words.txt";
vector<string> g_words;


//
int main()
{
	//
	Tests tests(50);
	
	///	Your ..... welcome
	tests.setFailFast(false);
	
	//
	preventPrototypeTampering();
	loadWords();
	
	//
	std::vector<std::function<void(Tests&)>> fs = {
		testHashingFunctions,
		testTableManipulation,
		testTableComplexity,
		testRolodex,
		testLeaderboard,
	};
	
	//
	for ( auto f : fs ) {
		f(tests);
	}
	
	//
	tests.run(true);
	
	//
	cout << "Tests complete" << endl;
	
	return 0;
}


//
void preventPrototypeTampering()
{
	//	MyHashTable
	{
		//
		MyHashTable<int> t;
		
		//	Hashing functions
		{
			std::function<unsigned long long int(std::string)> f;
			f = std::bind(&MyHashTable<int>::hash, &t, std::placeholders::_1);
			f = std::bind(&MyHashTable<int>::myCustomHashFunction1, &t, std::placeholders::_1);
			f = std::bind(&MyHashTable<int>::myCustomHashFunction2, &t, std::placeholders::_1);
			f = std::bind(&MyHashTable<int>::myCustomHashFunction3, &t, std::placeholders::_1);
			f = std::bind(&MyHashTable<int>::myCustomHashFunction4, &t, std::placeholders::_1);
			f = std::bind(&MyHashTable<int>::midSquareHash, &t, std::placeholders::_1);
		}
		
		//	Getters
		{
			std::function<size_t()> f;
			f = std::bind(&MyHashTable<int>::capacity, &t);
			f = std::bind(&MyHashTable<int>::size, &t);
			f = std::bind(&MyHashTable<int>::n_collisions, &t);
			
			std::function<bool(std::string)> b;
			b = std::bind(&MyHashTable<int>::exists, &t, std::placeholders::_1);
			
			std::function<bool()> e;
			e = std::bind(&MyHashTable<int>::empty, &t);
			
			std::function<int&(std::string)> g;
			g = std::bind(&MyHashTable<int>::get, &t, std::placeholders::_1);
		}
		
		//	Setters
		{
			std::function<void(std::string, int)> f;
			f = std::bind(&MyHashTable<int>::add, &t, std::placeholders::_1, std::placeholders::_2);
			
			std::function<void(std::string)> r;
			r = std::bind(&MyHashTable<int>::remove, &t, std::placeholders::_1);
			
			std::function<void()> c;
			r = std::bind(&MyHashTable<int>::clear, &t);
		}
	}
	
	//	MyRolodex
	{
		MyRolodex r;
		
		//
		std::function<bool(std::string)> a = std::bind(&MyRolodex::exists, &r, std::placeholders::_1);
		std::function<void(std::string)> b = std::bind(&MyRolodex::remove, &r, std::placeholders::_1);
		
		// Having trouble binding overloaded ::add, so just letting it be
		
		std::function<Address(std::string)> e = std::bind(&MyRolodex::get, &r, std::placeholders::_1);
		
		std::function<void()> f = std::bind(&MyRolodex::clear, &r);
		std::function<bool()> g = std::bind(&MyRolodex::empty, &r);
		std::function<size_t()> h = std::bind(&MyRolodex::size, &r);
	}
}


//
void loadWords()
{
	ifstream f(g_words_filename);
	if ( !f ) {
		throw std::runtime_error("Unable to load words file: " + g_words_filename);
	}
	
	g_words.clear();
	string word;
	while ( true )
	{
		f >> word;
		if ( f.eof() ) {
			break;
		}
		g_words.push_back(word);
	}
	
	//
	cout << "Loaded " << g_words.size() << " words from " << g_words_filename << endl;
	if ( g_words.size() != 102774 ) {
		throw std::runtime_error("Expected 102774 words from the word list but got " + to_string(g_words.size()));
	}
}


//
string increaseWordLength(string word)
{
	//
	word += word;
	word += word;
	word += word;
	word += word;
	
	return word;
}


//
MyHashTable<string> createWordTable(size_t capacity, bool longWords, bool shortDict)
{
	//
	MyHashTable<string> table(capacity);
	
	//
	size_t i = 0;
	for ( auto word : g_words ) {
		
		//
		if ( longWords == true ) {
			word = increaseWordLength(word);
		}
		
		//
		table.add(word, "Value for word: " + word);
		i++;
		
		//
		if ( shortDict == true && i >= 1000 ) {
			break;
		}
	}
	
	return table;
}


//
void testHashingFunctions(Tests& tests)
{
	//
	auto t = tests.createTest("Hashing");
	
	t->setFixedPointsPossible(10);
	t->setNormalizedPointsPossibleTarget(5);
	
	//	Check that the modified midSquare hash function produces in-bounds codes
	{
		//
		MyHashTable<int> table1(10000), table2(1000), table3(10);
		
		//
		t->log("Checking midSquare hash stays in-bounds");
		for ( auto word : g_words ) {
			
			//
			word = increaseWordLength(word);
			
			//
			unsigned long long int
				code1 = table1.midSquareHash(word),
				code2 = table2.midSquareHash(word),
				code3 = table3.midSquareHash(word)
				;
			
			//
			if (code1 >= table1.capacity() ) {
				t->assertTrue(code1 < table1.capacity(), 0, "Modified midSquare hash code (" + to_string(code1) + ") should be in-bounds of table capacity (" + to_string(table1.capacity()) + ")");
			}
			if (code2 >= table1.capacity() ) {
				t->assertTrue(code2 < table2.capacity(), 0, "Modified midSquare hash code (" + to_string(code2) + ") should be in-bounds of table capacity (" + to_string(table2.capacity()) + ")");
			}
			if (code3 >= table1.capacity() ) {
				t->assertTrue(code3 < table3.capacity(), 0, "Modified midSquare hash code (" + to_string(code3) + ") should be in-bounds of table capacity (" + to_string(table3.capacity()) + ")");
			}
		}
		t->assertTrue(true, 2, "Modified midSquare hash code bounds-checks were successful");
	}
	
	//	Test the modified midSquare hashing function
	{
		//
		MyHashTable<int> table1(10000), table2(1000), table3(100);
		
		//	Test some key/value combos
		vector<tuple<string, unsigned long long int, unsigned long long int, unsigned long long int>> combos;
		combos.push_back({"a",										9986ULL,	986ULL,		86ULL});
		combos.push_back({"aa",										1627ULL,	627ULL,		27ULL});
		combos.push_back({"zz",										4403ULL,	403ULL,		3ULL});
		combos.push_back({"iuI1U11W1EH3F3I4U4b4w4eiun",				3093ULL, 	93ULL,		93ULL});
		combos.push_back({"2d8dnfqwef23f f23f23 2f4a3c 3a34 g",		6072ULL,	72ULL,		72ULL});
		combos.push_back({"2d8dnfqwef23f f23f23 2f4a3c 3a34 h",		7880ULL,	880ULL,		80ULL});
		combos.push_back({"2d8dnfqwef23f f23f23 2f4a3c 3a34 gg",	5372ULL,	372ULL,		72ULL});
		combos.push_back({"oO(@#*(*FJjfhfhf fh  hh  h hh     ",		442ULL,		442ULL,		42ULL});
		combos.push_back({"wefi2q3",								191ULL,		191ULL,		91ULL});
		combos.push_back({"242",									4579ULL,	579ULL,		79ULL});
		combos.push_back({"11110000",								4120ULL,	120ULL,		20ULL});
		
		//
		t->log("Checking midSquare hash produces corect results");
		
		//
		unsigned long long int hashCode1_actual, hashCode2_actual, hashCode3_actual;
		for ( auto tup : combos) {
			
			//
			string word = std::get<0>(tup);
			unsigned long long int hashCode1_expected = std::get<1>(tup);
			unsigned long long int hashCode2_expected = std::get<2>(tup);
			unsigned long long int hashCode3_expected = std::get<3>(tup);
			
			//
			string word_longer = increaseWordLength(word);
			
			//
			hashCode1_actual = table1.midSquareHash(word_longer);
			hashCode2_actual = table2.midSquareHash(word_longer);
			hashCode3_actual = table3.midSquareHash(word_longer);
			
			//	Check all codes are correct
			if ( hashCode1_actual != hashCode1_expected ) {
				t->assertEqual(hashCode1_actual, hashCode1_expected, 0, "Checking midSquareHash for: " + word_longer + " (" + word + ")");
			}
			if ( hashCode2_actual != hashCode2_expected ) {
				t->assertEqual(hashCode2_actual, hashCode2_expected, 0, "Checking midSquareHash for: " + word_longer + " (" + word + ")");
			}
			if ( hashCode3_actual != hashCode3_expected ) {
				t->assertEqual(hashCode3_actual, hashCode3_expected, 0, "Checking midSquareHash for: " + word_longer + " (" + word + ")");
			}
		}
		t->assertTrue(true, 5, "Modified midSquare hash code output tests passed");
	}
	
	//	Check that the custom hash functions produce in-bounds codes
	{
		//
		MyHashTable<int> table1(10000), table2(1000), table3(10);
		
		//
		t->log("Checking that custom hash functions produce in-bounds codes.");
		
		//
		for ( size_t i = 0; i < g_words.size(); i++ ) {
			
			//	No need to do the full dict
			if ( i == 1000 ) {
				break;
			}
			
			//
			auto word = increaseWordLength(g_words[i]);
			
			//
			unsigned long long int
				code11 = table1.myCustomHashFunction1(word),
				code12 = table1.myCustomHashFunction2(word),
				code13 = table1.myCustomHashFunction3(word),
				code14 = table1.myCustomHashFunction4(word),
				
				code21 = table2.myCustomHashFunction1(word),
				code22 = table2.myCustomHashFunction2(word),
				code23 = table2.myCustomHashFunction3(word),
				code24 = table2.myCustomHashFunction4(word),
				
				code31 = table3.myCustomHashFunction1(word),
				code32 = table3.myCustomHashFunction2(word),
				code33 = table3.myCustomHashFunction3(word),
				code34 = table3.myCustomHashFunction4(word)
				;
			
			//
			vector<tuple<MyHashTable<int>&, unsigned long long int, unsigned long long int, unsigned long long int, unsigned long long int>> items = {
				{table1, code11, code12, code13, code14},
				{table2, code21, code22, code23, code24},
				{table3, code31, code32, code33, code34},
			};
			for ( auto tup : items ) {
				
				//
				auto table = std::get<0>(tup);
				auto code1 = std::get<1>(tup);
				auto code2 = std::get<2>(tup);
				auto code3 = std::get<3>(tup);
				auto code4 = std::get<4>(tup);
				
				//
				if ( code1 >= table.capacity() ) {
					t->assertTrue(code1 < table.capacity(), 0, "Custom hash code 1 (" + to_string(code1) + ") should be in-bounds of table capacity (" + to_string(table.capacity()) + ")");
				}
				if ( code2 >= table.capacity() ) {
					t->assertTrue(code2 < table.capacity(), 0, "Custom hash code 2 (" + to_string(code2) + ") should be in-bounds of table capacity (" + to_string(table.capacity()) + ")");
				}
				if ( code3 >= table.capacity() ) {
					t->assertTrue(code3 < table.capacity(), 0, "Custom hash code 3 (" + to_string(code3) + ") should be in-bounds of table capacity (" + to_string(table.capacity()) + ")");
				}
				if ( code4 >= table.capacity() ) {
					t->assertTrue(code4 < table.capacity(), 0, "Custom hash code 4 (" + to_string(code4) + ") should be in-bounds of table capacity (" + to_string(table.capacity()) + ")");
				}
			}
		}
		t->assertTrue(true, 2, "Custom hash code bounds-checks were successful");
	}
	
	//	Check that the custom hash functions are deterministic and not random, lol
	{
		//
		MyHashTable<int> table(100000);
		
		//
		for ( auto word : g_words ) {
			
			//
			auto code1 = table.myCustomHashFunction1(word);
			auto code2 = table.myCustomHashFunction2(word);
			auto code3 = table.myCustomHashFunction3(word);
			auto code4 = table.myCustomHashFunction4(word);
			
			//
			if ( code1 != table.myCustomHashFunction1(word) ) {
				t->assertEqual(code1, table.myCustomHashFunction1(word), 0, "Custom code 1 should be deterministic!");
			}
			if ( code2 != table.myCustomHashFunction2(word) ) {
				t->assertEqual(code2, table.myCustomHashFunction2(word), 0, "Custom code 2 should be deterministic!");
			}
			if ( code3 != table.myCustomHashFunction3(word) ) {
				t->assertEqual(code3, table.myCustomHashFunction3(word), 0, "Custom code 3 should be deterministic!");
			}
			if ( code4 != table.myCustomHashFunction4(word) ) {
				t->assertEqual(code4, table.myCustomHashFunction4(word), 0, "Custom code 4 should be deterministic!");
			}
		}
		t->assertTrue(true, 1, "Extra point for not trying to hack the Leaderboard");
	}
}

//
void testTableManipulation(Tests& tests)
{
	//
	auto t = tests.createTest("Table Manip");
	
	t->setFixedPointsPossible(23);
	t->setNormalizedPointsPossibleTarget(5);
	
	//	Fill the table with words
	{
		//
		t->log("Filling table 1 (10000000) with " + to_string(g_words.size()) + " words...");
		auto table1 = createWordTable(10000000, false);
		
		//
		t->log("Filling table 2 (1000000) with " + to_string(g_words.size()) + " words...");
		auto table2 = createWordTable(1000000, false);
		
		//
		t->log("Filling table 3 (1000) with " + to_string(g_words.size()) + " words...");
		auto table3 = createWordTable(1000, false);
		
		//	Check the capacity, size
		t->assertEqual(table1.capacity(),	10000000UL, 1, "Checking correct capacity hash table");
		t->assertEqual(table1.size(),		102774UL, 1, "Checking correct size of hash table after adding dictionary");
		
		//	Check number of collisions
		t->assertEqual(table1.n_collisions(), 6193UL, 1, "Checking correct number of collisions in hash table (capacity " + to_string(table1.capacity()) + ") after adding dictionary");
		t->assertEqual(table2.n_collisions(), 10318UL, 1, "Checking correct number of collisions in hash table (capacity " + to_string(table2.capacity()) + ") after adding dictionary");
		t->assertEqual(table3.n_collisions(), 101774UL, 1, "Checking correct number of collisions in hash table (capacity " + to_string(table3.capacity()) + ") after adding dictionary");
	}
	
	//	Check add/exist/get/remove
	{
		//
		MyHashTable<int> table;
		Random r;
		
		//
		for ( size_t i = 0; i < 1000; i++ ) {
			
			//
			int rKey = r.get(0, 1000000);
			int value = r.get(0, 1000000);
			
			//
			string key = "zzzz_" + to_string(rKey) + "_" + to_string(value) + "_" + to_string(i);
			
			//	Entry shouldn't exist, at first
			if ( table.exists(key) ) {
				t->assertFalse(table.exists(key), 0, "Key \"" + key + "\" should not already exist!");
			}
			
			//	Add to the table, and make sure it does exist now
			table.add(key, value);
			if ( !table.exists(key) ) {
				t->assertTrue(table.exists(key), 0, "Key \"" + key + "\" should now exist!");
			}
			
			//	Get the value and make sure it is correct
			if ( table.get(key) != value ) {
				t->assertEqual(table.get(key), value, 0, "Key \"" + key + "\" should have the value: " + to_string(value));
			}
			
			//	Remove, and make sure it no longer exists
			table.remove(key);
			if ( table.exists(key) ) {
				t->assertFalse(table.exists(key), 0, "Key \"" + key + "\" should not exist after removal!");
			}
		}
		t->assertTrue(true, 5, "Add/exist/get/remove tests seem to have passed");
	}
	
	//	Check collisions accurately recorded
	{
		//
		t->log("Checking collisions on a table of capacity=1");
		auto table = createWordTable(1, true, true);
		
		//
		t->assertEqual(table.n_collisions(), 999UL, 1, "Table of capacity=1 should be nothing but collisions");
	}
	
	//	Clear a table
	{
		//
		auto table = createWordTable(1000000);
		if ( table.empty() ) {
			t->assertFalse(table.empty(), 0, "Checking if word table thinks it is empty");
		}
		
		//
		t->log("Clearing a table item by item");
		size_t wantedSize = table.size();
		size_t j = wantedSize / 2;
		for ( size_t i = 0; i < j; i++ ) {
			
			//
			table.remove(g_words[i]);
			
			//
			wantedSize--;
			if ( table.size() != wantedSize ) {
				t->assertEqual(table.size(), wantedSize, 0, "Size after removing word " + g_words[i] + " should be: " + to_string(wantedSize));
			}
		}
		t->assertTrue(true, 1, "Check size changes as table is progressively emptied");
		
		//
		t->log("Clearing the rest of the table instantly with clear()");
		table.clear();
		t->assertTrue(table.empty(), 1, "Table should be empty() after clearing");
	}
	
	//	Check double-add exception
	t->assertException(
		[]()
		{
			//
			auto table = createWordTable();
			table.add("hello", "Whoops, duplicate value for word: Hello");
			
		}, 1, "Attempting to add an existing key should throw an exception"
	);
	
	//	Check remove exception
	t->assertException(
		[]()
		{
			//
			MyHashTable<int> table;
			
			//
			table.remove("Does not exist");
			
		}, 1, "Attempting to remove key that doesn't exist should throw an exception"
	);
	
	//	Check get exception
	t->assertException(
		[]()
		{
			//
			MyHashTable<int> table;
			
			//
			table.get("Does not exist");
			
		}, 1, "Attempting to get value for key that doesn't exist should throw an exception"
	);
	
	//	Check that resizing a table doesn't destroy data
	//	(but does change the number of collisions)
	{
		//	Start with table of 1m
		auto table = createWordTable(1000000);
		
		//	Check n_collissions
		t->assertEqual(table.n_collisions(), 10318UL, 0, "Table of capacity " + to_string(table.capacity()) + " should have resulted in 10325 collisions");
		
		//	Change capacity to only 100k
		table.setCapacity(100000);
		
		//	Check n_collisions again
		t->assertEqual(table.capacity(), 100000UL, 0, "Check that capacity was actually modified");
		t->assertEqual(table.n_collisions(), 40673UL, 1, "Table of capacity " + to_string(table.capacity()) + " should have resulted in 40673 collisions");
		
		//
		t->assertTrue(true, 3, "Table resize seems to be working (n_collisions).");
		
		//	Make sure every item survived the resize
		for ( auto word : g_words ) {
			if ( !table.exists(word) ) {
				t->assertTrue(table.exists(word), 0, "Word is missing after table resize: " + word);
			}
		}
		t->assertTrue(true, 3, "Table resize seems to be working (data survival).");
	}
}

void testTableComplexity(Tests& tests)
{
	//
	auto t = tests.createTest("Complexity");
	
	t->setFixedPointsPossible(4);
	t->setNormalizedPointsPossibleTarget(5);
	
	//	Fast vs. Slow
	{
		//	Create a fast table
		Timer timer1;
		auto table1 = createWordTable(100000, false, false);
		timer1.stop();
		
		//	Create a slow table
		Timer timer2;
		auto table2 = createWordTable(100, false, false);
		timer2.stop();
		
		//	The fast table should have been way faster to populate than the slow table
		t->assertTrue(
			timer1.microseconds() * 5 < timer2.microseconds(), 1,
			"Larger table (less collisions) should be much faster to populate than a smaller table (more collisions). Larger table took " + timer1.milliseconds_s()
				+ "ms while smaller table took " + timer2.milliseconds_s() + "ms."
				+ " Collisions = " + to_string(table1.n_collisions()) + " vs. " + to_string(table2.n_collisions()) + "."
				+ " Factor = " + to_string(static_cast<double>(timer2.microseconds()) / static_cast<double>(timer1.microseconds()))
		);
		
		//	The fast table should be much faster to access than the slow table
		Timer timer1_access;
		for ( auto word : g_words ) {
			table1.get(word);
		}
		timer1_access.stop();
		//
		Timer timer2_access;
		for ( auto word : g_words ) {
			table2.get(word);
		}
		timer2_access.stop();
		t->assertTrue(timer1_access.microseconds() * 5 < timer2_access.microseconds(), 1, "Larger table (less collisions) should be much faster to access than a smaller table (more collisions)");
	}
	
	//	Insertion and Access: Constant time for a large table
	{
		//
		MyHashTable<string> table(10000000);
		Timer timer1, timer2, timer3, timer4;
		Complexity complexity;
		
		//	Check Insertion
		for ( size_t i = 0; i < g_words.size(); i++ ) {
			table.add(g_words[i], g_words[i]);
			if ( i == 10000 ) {
				timer2.start();
			}
		}
		timer1.stop();
		timer2.stop();
		t->log("Time for timer1: " + to_string(timer1.microseconds()) + "; Time for timer2: " + to_string(timer2.microseconds()));
		t->assertTrue(complexity.checkConstantTime(timer1, 10000, timer2, g_words.size()), 1, "Add/Insert to a large-enough table should be about constant time");
		
		//	Check Insertion
		timer3.start();
		for ( size_t i = 0; i < g_words.size(); i++ ) {
			table.get(g_words[i]);
			if ( i == 10000 ) {
				timer4.start();
			}
		}
		timer3.stop();
		timer4.stop();
		t->log("Time for timer3: " + to_string(timer3.microseconds()) + "; Time for timer4: " + to_string(timer4.microseconds()));
		t->assertTrue(complexity.checkConstantTime(timer3, 10000, timer4, g_words.size()), 1, "Access with a large-enough table should be about constant time");
	}
}


//
void testRolodex(Tests& tests)
{
	//
	auto t = tests.createTest("Rolodex");
	
	t->setFixedPointsPossible(5);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	std::shared_ptr<MyRolodex> dex(new MyRolodex);
	
	//
	std::shared_ptr<Address> larry_address(new Address);
	larry_address->line1 = "1212122 Lane Road";
	larry_address->line2 = "";
	larry_address->city = "Fullerton";
	larry_address->state = "CA";
	larry_address->zip = 92831;
	larry_address->country = "US";
	
	//	Add to rolodex normally
	t->assertNoException(
		[t, dex, larry_address]()
		{
			dex->add("Gary", "1212121 Lane Way", "", "Fullerton", "CA", 92831, "US");
			dex->add("Larry", *larry_address);
			dex->add("Harry", "1212123 Lane Place", "", "Fullerton", "CA", 92831, "US");
			dex->add("Sally", "1212124 Lane Blvd", "", "Fullerton", "CA", 92831, "US");
			
		}, 1, "Add to the Rolodex"
	);
	t->run();
	
	//	Check stuff
	t->assertNoException(
		[t, dex, larry_address]()
		{
			t->assertEqual(dex->size(), 4UL, 0, "Check size");
			t->assertTrue(dex->exists("Larry"), 0, "Check that an added entry does exist");
			t->assertTrue(dex->get("Larry") == *larry_address, 0, "Check fetch from the Rolodex. Expected <<<" + larry_address->toString() + ">>> and found <<<" + dex->get("Larry").toString() + ">>>");
			t->assertTrue(dex->get("Gary") != *larry_address, 0, "Check not-equal fetch from the Rolodex. Expected <<<" + dex->get("Gary").toString() + ">>> to be different from <<<" + larry_address->toString() + ">>>");
		}, 1, "Check stuff after add"
	);
	t->run();
	
	//	Make sure we can remove
	t->assertNoException(
		[t, dex, larry_address]()
		{
			//	Remove
			dex->remove("Larry");
			t->assertFalse(dex->exists("Larry"), 0, "Check that a deleted entry no longer exists");
			
		}, 1, "Removing a valid entry"
	);
	t->run();
	
	//	Expect an exception when deleting something that doesn't exist
	t->assertException(
		[dex]()
		{
			dex->remove("Busey");
			
		}, 1, "Delete an entry that doesn't exist from the Rolodex"
	);
	t->run();
	
	//	Expect an exception when adding duplicates
	t->assertException(
		[]()
		{
			//
			MyRolodex dex;
			
			dex.add("Gary", "1212121 Lane Way", "", "Fullerton", "CA", 92831, "US");
			dex.add("Larry", "1212122 Lane Road", "", "Fullerton", "CA", 92831, "US");
			dex.add("Harry", "1212123 Lane Place", "", "Fullerton", "CA", 92831, "US");
			dex.add("Harry", "1212123 Lane Place", "", "Fullerton", "CA", 92831, "US");
			dex.add("Sally", "1212124 Lane Blvd", "", "Fullerton", "CA", 92831, "US");
			
		}, 1, "Add duplicates to the Rolodex"
	);
}


//
void testLeaderboard(Tests& tests)
{
	//
	auto t = tests.createTest("Leaderboard");
	
	t->setFixedPointsPossible(1);
	t->setNormalizedPointsPossibleTarget(1);
	
	//
	t->assertNoException(
		[&tests]()
		{
			//
			MyHashTable<int> table(100000);
			
			//
			std::map<
				int, std::map<unsigned long long int, bool>
			> existing_codes;
			
			long n_collisions[4] = {0, 0, 0, 0};
			
			bool is_std_hash[4] = {true, true, true, true};
			std::hash<std::string> stdHasher;
			
			//
			for ( string word : g_words ) {
				
				//
				unsigned long long int codes[4];
				codes[0] = table.myCustomHashFunction1(word);
				codes[1] = table.myCustomHashFunction2(word);
				codes[2] = table.myCustomHashFunction3(word);
				codes[3] = table.myCustomHashFunction4(word);
				
				//
				for ( int code_index = 0; code_index < 4; code_index++ ) {
					
					//
					unsigned long long int code = codes[code_index];
					
					//
					if ( code != stdHasher(word) ) {
						is_std_hash[code_index] = false;
					}
					
					//
					if ( existing_codes[code_index].find(code) == existing_codes[code_index].end() ) {
						existing_codes[code_index][code] = true;
					}
					else {
						n_collisions[code_index]++;
					}
				}
			}
			
			//
			int bestCodeIndex = 0;
			int bestCollisions = n_collisions[0];
			cout << "Custom code results:" << endl;
			for ( int i = 0; i < 4; i++ ) {
				cout << "- Code " << (i + 1) << ": " << n_collisions[i] << endl;
				if ( n_collisions[i] < bestCollisions ) {
					bestCodeIndex = i;
					bestCollisions = n_collisions[i];
				}
			}
			cout << "==> Best is code " << (bestCodeIndex + 1) << " with " << bestCollisions << " collisions." << endl;
			cout << "(sending to Leaderboard)" << endl;
			
			//
			bool found_cheat = false;
			for ( bool b : is_std_hash ) {
				found_cheat = (found_cheat || b);
			}
			if ( found_cheat ) {
				throw std::runtime_error("Don't try to cheat the leaderboard by using pre-made hashing functions. For shame!");
			}
			
			//
			auto board = tests.createLeaderboard();
			board->setSortDirection(LeaderboardSortDirection::Ascending);
			board->addEntry("collisions", bestCollisions);
			tests.setLeaderboard(board);
			
		}, 1, "Extra point for not crashing the Leaderboard test"
	);
}















