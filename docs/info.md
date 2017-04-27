# Documentation

TermDb is a single header library for parsing Terminfo databases.
To read more about terminfo, execute `$ man terminfo` in your terminal.

Supported Capablities are extracted and listed inside `capablities.txt`.

Examples below are self documenting -

#### 1.
```cpp
#include "termdb.hpp"

// Everything is provided under namespace tdb
using namespace tdb;

int main()
{

	/* Create a parser object which, obviously,
	   does nothing right now */
	TermDb parser;

	// now you can ask it to parse some terminfo databases
	bool result = parser.parse("gnome");

	// or specify location of database files
	// default is '/usr/share/terminfo/'
	parser.parse("adm3a","/usr/local/share/");

	// right => /usr/share/terminfo/
	// wrong !==! /usr/share/terminfo

	// you can call parse() method as many times on
	// same object, it will overwrite previous db
	parser.parse("gnome");
	parser.parse("xterm");
}
```

#### 2.
```cpp
{

	// There is one other way to construct parser objects
	// which follows same signature as parse() method
	TermDb parser1("gnome");
	TermDb parser2("xterm", "/usr/local/terminfo/");

	/* The difference b/w constructor and parse() method
	   is that parse() method returns a boolean result
	   indicating success of operation while constructor cannot
	*/

	// so how to check for errors?
	TermDb parser3("aaaa");
	auto res = parser3.getStatus();

	if(res < 0){
		std::cerr << "UhOH!!!";
	} else {
		// do your magic ;)
	}
}
```

#### 3.
```cpp
{
	// What about capablities are not present?

	TermDb parser;
	bool result = parser.parse("adm3a");

	// Well boolean capablities can return false so
	// b will be false if capablity is missing
	auto b = parser.getCapability(bin::has_meta_key);

	// Similarly string capablities can return empty strings
	// in case said capablity is not found for given terminal
	auto s = parser.getCapability(str::enter_bold_mode);

	// The case becomes tricky when dealing with numerical
	// capablities. To ease out development, a special var
	// tdb::NP is provided which denotes NOT PRESENT
	auto n = parser.getCapability(num::columns);
	if( n == tdb::NP){
		// it ain't there
	}
}
```


#### 4.
```cpp
{
	// What about thread safety?

	// Well either you can create new parser objects in each thread
	// or in case you want to use same parser object, guard the
	// constructor and/or parse() method with a lock_guard() or mutex.
}
```
