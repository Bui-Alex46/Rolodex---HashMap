#ifndef MY_HASH_TABLE_HPP
#define MY_HASH_TABLE_HPP

#include <forward_list>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <iostream>
#include <stdexcept>


namespace CPSC131::MyHashTable
{
	
	template<class VTYPE>
	class MyHashTable
	{
		
		public:
			
			
			static constexpr size_t DEFAULT_CAPACITY = 1024;
			static constexpr size_t ULLONG_WRAP_AT = 4294967295;
			
			
			MyHashTable(size_t capacity = MyHashTable::DEFAULT_CAPACITY)
			{
				//Create the array to store the items for the Hash Table 
				//Capacity holding the rows 
				setCapacity(capacity);

			}
			
			MyHashTable(const MyHashTable& other)
			{
				capacity_ = other.capacity_;
				size_ = other.size_;
				n_collisions_ = other.n_collisions_;
			}
			
			~MyHashTable()
			{
				if(table_ != nullptr){
					delete [] table_;
				}
				table_ = nullptr;
				this -> n_collisions_ = 0;
			}
			
			size_t capacity()
			{
				return capacity_;
			}
			
			size_t size()
			{
				return size_;
			}
			
			bool empty()
			{
				if(table_ != nullptr){
					return false;
				}
				else
					return true;
				
				
			}
			
			size_t n_collisions()
			{

				return n_collisions_;
			}
			
			void setCapacity(size_t c)
			{
				// Create the array to store the items for the Hash Table 
				// Capacity holding the rows 
				auto temp = table_;
				size_t tempCapacity_ = capacity_;
				this -> capacity_ = c;
				this -> size_ = 0;
				this -> n_collisions_ = 0;
				//Making a new table
				this -> table_ = new std::forward_list<std::pair<std::string, VTYPE>>[c];
				
				//Iterate through the temp table
				//Add all the items from the old table into the temp table
				if(temp != nullptr){
					for(unsigned long long int i = 0 ; i < tempCapacity_; i ++){
						
						for(auto j = temp[i].begin(); j != temp[i].end(); j++){
							auto keyValue = (*j);
							this -> add(keyValue.first,keyValue.second);
						}
					}
					
				}
				
				delete [] temp;
			}
			
			unsigned long long int hash(std::string key) const
			{
				return this->midSquareHash(key);
			}
			

			unsigned long int midSquareHash(std::string key) const
			{
				unsigned long long int sum = 1;
				//Iterate over the key
				for(size_t i = 0; i < key.size(); i++){
					 //Casting characters into an integer
					unsigned long long int value = static_cast<unsigned long int >(key[i]);
					// Multiplying sum by the value obtained from the string 
					 sum = (sum * value) % ULLONG_WRAP_AT;
				}
				unsigned long int sumSquared = sum * sum;
				 //KEEP THE MIDDLE DIGITS OF THIS SUM
					//Converts our squared number to a string
					std::string newKey = std::to_string(sumSquared); 
					//Take half of the string as a new substring
					std::string subString = newKey.substr( ((newKey.size() / 2)/2), newKey.size()/2); 

				//Converting substring into a Unsigned long long int 
				unsigned long long int hashCode = std::stoull(subString);
				//Making sure hashCode is in bounds
				hashCode = hashCode % capacity_; 

				return hashCode; 
			}
			
			unsigned long long int myCustomHashFunction1(std::string key) const
			{
				return 0;
			}
			unsigned long long int myCustomHashFunction2(std::string key) const
			{
				return 0;
			}
			unsigned long long int myCustomHashFunction3(std::string key) const
			{
				return 0;
			}
			unsigned long long int myCustomHashFunction4(std::string key) const
			{
				return 0;
			}
			
			bool exists(std::string key) const
			{
				auto indx = this -> hash(key);
				auto current = &this -> table_[indx];
				for(auto i = current -> begin(); i != current -> end(); i++){
					if((*i).first == key){
						return true;
					}
				}
				return false;
			}
			
		
			void add(std::string key, VTYPE value)
			{
				//Creating new pair to add
				//Generating an index for the key
				if(exists(key)){
					throw std::runtime_error("Cannot add value for because key already exist: " + key);
				}
				unsigned long long int indx = this -> hash(key);
				
				//Collision Check
				auto current = &this -> table_[indx];
				//Keeping track of the current pair 
				//If the current spot is not empty
				if(current != nullptr){
					n_collisions_++;
				}
				if(current -> empty()){
					n_collisions_--;
				}
					//Adding the new item and increasing size
					current->push_front(std::pair<std::string, VTYPE>(key, value));
					size_++;
				//THrow error if key exists
				
	
			}
			
			
			VTYPE& get(std::string key) const
			{
				unsigned long long int indx = this -> hash(key);
				
				for(auto i = table_[indx].begin(); i != table_[indx].end(); i++ ){
						auto temp = (*i);
						if(temp.first == key){
							return (*i).second; 
						}
					}
				if(!exists(key)){
							throw std::runtime_error("Cannot get value for key because it doesn't exist: " + key);
						}

				
			}
			
			
			void remove(std::string key)
			{
				// Getting the key
				auto index = this -> hash(key);
				//Having a current variable
				auto current = &this -> table_[index];
				//This holds the current pair to be deleted
				auto instance = table_[index].begin();
							if(current -> empty()){
							 	n_collisions_--;
							}
					//Iterates over all key value pairs
						for(auto j = current->before_begin(); j != current -> end(); j++){
							
							//Peek takes a look at the value first
							auto peek = j;
							peek++;
							if(peek == current -> end()){
								break;
							}
							auto peekValue = (*peek);
							if(peekValue.first == key){
								instance = j;
							}
					}
					if(instance != current->end()){
						current->erase_after(instance);
						 size_--;
					}
					else{
						throw std::runtime_error("Cannot remove value for key because it doesn't exist: " + key);
					}
					
			
				//Key doesn't exists
			}
			void clear()
			{
				for(size_t i = 0; i < capacity_; i++){
					table_[i].clear();
				}
				capacity_ = 0;
				size_ = 0;
				n_collisions_ = 0;
				table_ = nullptr;
			}
			MyHashTable<VTYPE>& operator=(const MyHashTable<VTYPE>& other)
			{
				return *this;
			}
			
		
		private:
			
			size_t capacity_ = 0;
			
			size_t size_ = 0;
			
			size_t n_collisions_ = 0;

			//The table pointer is a forward list, that will be contained in an array
			std::forward_list<std::pair<std::string, VTYPE>> * table_ = nullptr;
	};
}













#endif
