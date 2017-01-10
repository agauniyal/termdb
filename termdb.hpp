#include <algorithm>
#include <array>
#include <bitset>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

namespace termdb {

// NP represents 'Not Present' properties, represented by
// -1 value in terminfo databases. Since uint16_t is unsigned
// we need to represent it by special NP variable
uint16_t NP = std::numeric_limits<uint16_t>::max();


int loadDb(const std::string name, std::string path = "/usr/share/terminfo/")
{
	if (name.length() == 0 || path.length() == 0) {
		return -1;
	}

	path.append(name.substr(0, 1)).append("/").append(name);
	std::ifstream db(path.c_str(), std::ios::binary | std::ios::ate);
	if (!db) {
		return -2;
	}

	const int size = db.tellg();
	if (!size) {
		return -2;
	}


	db.seekg(0, std::ios::beg);
	std::vector<uint8_t> buffer(size, 0);
	db.read(reinterpret_cast<char *>(buffer.data()), size);
	db.close();

	// header contains a constant magic number
	auto magic_byte = buffer[0] | (buffer[1] << 8);
	if (magic_byte != 0432) {
		return -3;
	}

	/* size array contains size of -
	  - name [0]
	  - booleans [1]
	  - numbers [2]
	  - offsets [3]
	  - stringTable [4]
	*/
	uint16_t sizeArray[5] = { 0 };
	for (auto i = 2, j = 0; i < 12; i += 2, ++j) {
		sizeArray[j] = buffer[i] | (buffer[i + 1] << 8);
	}

	// parse name of terms
	int cursor = 12;
	std::string nameString(sizeArray[0] - 1, ' ');
	std::copy_n(buffer.begin() + cursor, sizeArray[0] - 1, nameString.begin());
	cursor += sizeArray[0];

	// parse boolean values
	std::bitset<44> booleans;
	for (auto i = cursor; i < cursor + sizeArray[1]; ++i) {
		if (buffer[i]) {
			booleans.set(i - cursor);
		}
	}
	cursor += sizeArray[1];


	/*
	    Between the boolean section and the number section, a null
	    byte will be inserted, if necessary, to ensure that the
	    number section begins on an even byte (this is a relic of
	    the PDP-11's word-addressed architecture, originally
	    designed in to avoid IOT traps induced by addressing a
	    word on an odd byte boundary). All short integers are
	    aligned on a short word boundary.
	*/
	cursor += cursor % 2;


	// parse numbers values
	uint16_t numbers[39] = { 0 };
	for (auto i = cursor, j = 0; j < sizeArray[2]; i += 2, ++j) {
		numbers[j] = buffer[i] | (buffer[i + 1] << 8);
	}
	cursor += sizeArray[2] * 2;


	// parse stringOffset values
	std::vector<uint16_t> stringOffset(sizeArray[3], 0);
	for (auto i = cursor, j = 0; j < sizeArray[3]; i += 2, ++j) {
		stringOffset[j] = buffer[i] | (buffer[i + 1] << 8);
	}
	cursor += sizeArray[3] * 2;


	for (const auto &offset : stringOffset) {
		if (offset != NP) {
			auto index = cursor + offset;
			char ch    = buffer[index];
			while (ch != '\0') {
				// do something with ch
				ch = buffer[++index];
			}
		}
	}

	return 0;
}
}

/*
 * [1] http://invisible-island.net/ncurses/man/term.5.html
 * [2] $ man terminfo
 * [3] $ man term
*/
