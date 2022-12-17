#ifndef MY_ROLODEX_HPP
#define MY_ROLODEX_HPP


/**
 * You probably don't need to work on this class declaration.
 * 
 * Don't modify any existing prototypes.
 * 
 * You may add more helper functions, if you wish.
 */


//
#include "MyHashTable.hpp"
#include "Address.hpp"


//
#include <string>


//
namespace CPSC131::MyRolodex
{
	//
	class MyRolodex
	{
		//
		public:
			
			//
			MyRolodex();
			
			//
			bool exists(std::string name) const;
			void add(std::string name, std::string line1, std::string line2, std::string city, std::string state, int zip, std::string country);
			void add(std::string name, Address address);
			Address get(std::string name) const;
			void remove(std::string name);
			void clear();
			
			//
			bool empty();
			size_t size();
			
			
		//
		private:
			
			//
			::CPSC131::MyHashTable::MyHashTable<Address> table_;
	};
}















#endif
