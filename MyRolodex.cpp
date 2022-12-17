

//
#include "MyRolodex.hpp"
#include "MyHashTable.hpp"
#include "Address.hpp"


/**
 * TODO: Finish the implementation for this class
 */


//	More includes?


//
namespace CPSC131::MyRolodex
{
	/// Your welcome
	MyRolodex::MyRolodex() {}
	
	/**
	 * Return true if a person exists in the Rolodex
	 */
	bool MyRolodex::exists(std::string name) const
	{
		
		return table_.exists(name);
	}
	
	/**
	 * Allow the user to add an address using its parts.
	 * This should probably just call on the other overload
	 */
	void MyRolodex::add(std::string name, std::string line1, std::string line2, std::string city, std::string state, int zip, std::string country)
	{
		// Storing all values into an object
		Address info;
		info.line1 = line1;
		info.line2 = line2;
		info.city = city;
		info.state = state;
		info.zip = zip;
		info.country = country;
		//Adding values with given key into table
		table_.add(name, info);
	}
	
	/**
	 * Add an entry into the Rolodex
	 * The person's name is the key, and their Address is the value.
	 * The Address comes in the form of the Address struct, defined in another source file.
	 */
	void MyRolodex::add(std::string name, Address address)
	{
		table_.add(name,address);
	}
	
	/**
	 * Return an address structure associated with a person
	 */
	Address MyRolodex::get(std::string name) const
	{
		return table_.get(name);
	}
	
	/**
	 * Remove a person from the Rolodex
	 */
	void MyRolodex::remove(std::string name)
	{
		table_.remove(name);
	}
	
	/**
	 * Clear the Rolodex of all information.
	 * (Hint: The table also has this method)
	 */
	void MyRolodex::clear()
	{
		table_.clear();
	}
	
	/**
	 * Return true if the Rolodex is empty, false otherwise
	 */
	bool MyRolodex::empty()
	{
		return table_.empty();
	}
	
	/**
	 * Return the number of entries in this Rolodex
	 */
	size_t MyRolodex::size()
	{
		return table_.size();
	}
}






















