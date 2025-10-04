#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <set>
#include <string>
#include "util.h"

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}

/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/

std::set<std::string> parseStringToWords(const std::string& str) {
    std::set<std::string> words;
    std::string curr;
    for (size_t i = 0; i < str.size(); i++) {
        char c = str[i];
        // Allow alphanumeric characters and plus signs to be part of a word.
        if (std::isalnum(c) || c == '+') {
            curr.push_back(std::tolower(c));
        } else {
            if (curr.length() >= 2)
                words.insert(curr);
            curr.clear();
        }
    }
    if (curr.length() >= 2)
        words.insert(curr);
    return words;
}



/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}
