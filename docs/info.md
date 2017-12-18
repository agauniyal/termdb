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
	   indicating success of operation while constructor throws exception
	*/

	// this throws exception
	TermDb parser3("aaaa");
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
	auto b = parser.get(bin::has_meta_key);

	// Similarly string capablities can return empty strings
	// in case said capablity is not found for given terminal
	auto s = parser.get(str::enter_bold_mode);

	// An optional type is returned for numeric capablities
	auto n1 = parser.get(num::columns);
	if(n1){
		cout << n1.value();
	} else {
		cerr << "Oops!";
	}

	// Can use handy .value_or() as well
	auto n2 = parser.get(num::columns);
	cout << n2.value_or(24);
}
```
