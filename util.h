#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <set>


/** Complete the setIntersection and setUnion functions below
 *  in this header file (since they are templates).
 *  Both functions should run in time O(n*log(n)) and not O(n^2)
 */
template<typename T>
std::set<T> setIntersection(const std::set<T>& s1, const std::set<T>& s2) {
    std::set<T> result;
    typename std::set<T>::const_iterator it1 = s1.begin(), it2 = s2.begin();
    while (it1 != s1.end() && it2 != s2.end()){
        if (*it1 < *it2)
            ++it1;
        else if (*it2 < *it1)
            ++it2;
        else {
            result.insert(*it1);
            ++it1;
            ++it2;
        }
    }
    return result;
}


template<typename T>
std::set<T> setUnion(const std::set<T>& s1, const std::set<T>& s2) {
    std::set<T> result = s1; // copy s1
    for (typename std::set<T>::const_iterator it = s2.begin(); it != s2.end(); ++it)
        result.insert(*it);
    return result;
}


/***********************************************/
/* Prototypes of functions defined in util.cpp */
/***********************************************/

std::string convToLower(std::string src);

std::set<std::string> parseStringToWords(const std::string& str);

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// Removes any leading whitespace
std::string &ltrim(std::string &s) ;

// Removes any trailing whitespace
std::string &rtrim(std::string &s) ;

// Removes leading and trailing whitespace
std::string &trim(std::string &s) ;
#endif
