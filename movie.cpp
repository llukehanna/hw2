#include "movie.h"
#include "util.h"
#include <sstream>
#include <iomanip>

Movie::Movie(const std::string & name, double price, int qty,
             const std::string & genre, const std::string & rating)
    : Product("movie", name, price, qty), genre_(genre), rating_(rating)
{
}

Movie::~Movie() { }

std::set<std::string> Movie::keywords() const {
    std::set<std::string> keys = parseStringToWords(getName());
    std::set<std::string> genreKeys = parseStringToWords(genre_);
    keys.insert(genreKeys.begin(), genreKeys.end());
    keys.insert(rating_);
    return keys;
}

std::string Movie::displayString() const {
    std::ostringstream oss;
    oss << getName() << "\n"
        << "Genre: " << genre_ << " Rating: " << rating_ << "\n"
        << std::fixed << std::setprecision(2) << getPrice() << " " 
        << getQty() << " left.";
    return oss.str();
}

void Movie::dump(std::ostream& os) const {
    os << "movie" << "\n"
       << getName() << "\n"
       << getPrice() << "\n"
       << getQty() << "\n"
       << genre_ << "\n"
       << rating_ << "\n";
}
