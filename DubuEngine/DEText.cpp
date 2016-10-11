#include <sstream>
#include "DEText.h"

std::string trim(std::string s) {
	return s.erase(s.find_last_not_of(" \n\r\t") + 1);
}

SubStrings SplitString(std::string s, char c) {
	SubStrings result;

	// Count the items
	std::istringstream words_counter(s);
	int count = 0;
	std::string temp;
	while (words_counter >> temp) {
		count++;
	}
	result.items = count;

	// Write the items into substrings
	std::string* sub_strings = new std::string[count + 1];
	std::istringstream words(s);
	int word_id = 0;
	while (words >> temp) {
		sub_strings[word_id] = temp;
		word_id++;
	}
	result.s = sub_strings;

	// Return the resulting substring
	return result;
}