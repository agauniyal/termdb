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

	/* Create a parser object, with exceptions enabled, which parses some terminfo databases */
	TermDb<Exceptions::ON> parser("gnome");

	// or specify location of database files
	// default is '/usr/share/terminfo/'
	TermDb<Exceptions::ON> parser2("adm3a","/usr/local/share/");

	// you get the Exceptions::ON behavior by default
	// TermDb parser3("gnome");  // [C++17]

	// don't need exceptions ? we got you ;)
	TermDb<Exceptions::OFF> parser4("gnome");
	TermDb<Exceptions::OFF> parser5("adm3a","/usr/local/share/");

	// right => /usr/share/terminfo/
	// wrong !==! /usr/share/terminfo
}
```

#### 2.
```cpp
#include <iostream>
#include "termdb.hpp"

using namespace std;
using namespace tdb;

int main()
{
	// What about capablities are not present?

	try{
		TermDb<Exceptions::ON> parser("adm3a");

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
	} catch(...) {
		cout << "Oops !! Error Occured..\n";
	}

	return 0;
}
```

#### 3.
```cpp


#include "termdb.hpp"

using namespace std;
using namespace tdb;

int main()
{
	// What about capablities are not present?

	TermDb<Exceptions::ON> terminal("gnome-terminal");

	// Well boolean capablities can return false so
	// b will be false if capablity is missing
	auto b = terminal.get(bin::has_meta_key);

	// Similarly string capablities can return empty strings
	// in case said capablity is not found for given terminal
	auto s = terminal.get(str::enter_bold_mode);

	// An optional type is returned for numeric capablities
	auto n1 = terminal.get(num::columns);
	if(n1){
		cout << n1.value();
	} else {
		cerr << "Oops!";
	}

	// Can use handy .value_or() as well
	auto n2 = terminal.get(num::columns);
	cout << n2.value_or(24);
}
```

#### 4.
```cpp
#include <iostream>
#include "termdb.hpp"

using namespace std;
using namespace tdb;

int main()
{
	try{
		TermDb<Exceptions::ON> terminal("gnome-terminal");
		// TermDb terminal("gnome-terminal"); // Exceptions ON by default [C++17]

		auto name = terminal.getName();
		cout << "Terminal name: " << name << '\n';

		auto canRedefineColors = terminal.get(bin::can_change);
		cout << "Can you redefine colors on " << name << " ? " << canRedefineColors << '\n';

		auto maxColors = terminal.get(num::max_colors).value_or(0);
		cout << "Max colors for " << name << " : " << maxColors << '\n';

		auto clearScreenCode = terminal.get(str::clear_screen);
		cout << "Clear screen on " << name << " using code: " << clearScreenCode << '\n';

	} catch (...) {
		cout << "Oops!!\n";
	}


	return 0;
}
```

#### 5.
```cpp
#include <iostream>
#include "termdb.hpp"

using namespace std;
using namespace tdb;

int main(){

	TermDb<Exceptions::OFF> terminal("gnome-terminal");

	if (terminal){
		auto name = terminal.getName();
		cout << "Terminal name: " << name << '\n';

		auto canRedefineColors = terminal.get(bin::can_change);
		cout << "Can you redefine colors on " << name << " ? " << canRedefineColors << '\n';

		auto maxColors = terminal.get(num::max_colors).value_or(0);
		cout << "Max colors for " << name << " : " << maxColors << '\n';

		auto clearScreenCode = terminal.get(str::clear_screen);
		cout << "Clear screen on " << name << " using code: " << clearScreenCode << '\n';
	} else {
		cout << "Oops!!\n";
	}

	return 0;
}
```

#### 6.
```cpp
#include <iostream>
#include "termdb.hpp"

using namespace std;
using namespace tdb;

int main(){

	try{
		// auto-detect using $TERM on linux and use cmd on Windows
		TermDb<Exceptions::ON> terminal; 

		auto name = terminal.getName();
		cout << "Terminal name: " << name << '\n';

		auto canRedefineColors = terminal.get(bin::can_change);
		cout << "Can you redefine colors on " << name << " ? " << canRedefineColors << '\n';

		auto maxColors = terminal.get(num::max_colors).value_or(0);
		cout << "Max colors for " << name << " : " << maxColors << '\n';

		auto clearScreenCode = terminal.get(str::clear_screen);
		cout << "Clear screen on " << name << " using code: " << clearScreenCode << '\n';
	} catch {
		cout << "Oops!!\n";
	}

	return 0;
}
```

#### 7.
```cpp
#include <iostream>
#include "termdb.hpp"

using namespace std;
using namespace tdb;

int main(){

	// auto-detect using $TERM on linux and use cmd on Windows
	TermDb<Exceptions::OFF> terminal; 

	if (terminal) {
		auto name = terminal.getName();
		cout << "Terminal name: " << name << '\n';

		auto canRedefineColors = terminal.get(bin::can_change);
		cout << "Can you redefine colors on " << name << " ? " << canRedefineColors << '\n';

		auto maxColors = terminal.get(num::max_colors).value_or(0);
		cout << "Max colors for " << name << " : " << maxColors << '\n';

		auto clearScreenCode = terminal.get(str::clear_screen);
		cout << "Clear screen on " << name << " using code: " << clearScreenCode << '\n';
	} else {
		cout << "Oops!!\n";
	}

	return 0;
}
```