# Documentation

TermDb is a single header library for parsing Terminfo databases.
To read more about terminfo, execute `$ man terminfo` in your terminal.

Supported Capablities are extracted and listed inside `capablities.txt`.

Examples below are self documenting -

#### 1.
```cpp
#include "termdb.hpp"

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
	   indicating success of operation while constructr cannot
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
