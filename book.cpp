#include "book.h"
#include "util.h"
#include <sstream>
#include <iomanip>

Book::Book(const std::string & name, double price, int qty,
           const std::string & isbn, const std::string & author)
    : Product("book", name, price, qty), isbn_(isbn), author_(author)
{
}

Book::~Book() { }

std::set<std::string> Book::keywords() const {
    // Build keywords directly from the product name, isbn, and author.
    std::set<std::string> keys = parseStringToWords(getName());
    std::set<std::string> isbnKeys = parseStringToWords(isbn_);
    keys.insert(isbnKeys.begin(), isbnKeys.end());
    std::set<std::string> authorKeys = parseStringToWords(author_);
    keys.insert(authorKeys.begin(), authorKeys.end());
    return keys;
}

std::string Book::displayString() const {
    std::ostringstream oss;
    oss << getName() << "\n"
        << "Author: " << author_ << " ISBN: " << isbn_ << "\n"
        << std::fixed << std::setprecision(2) << getPrice() << " " 
        << getQty() << " left.";
    return oss.str();
}


void Book::dump(std::ostream& os) const {
    os << "book" << std::endl;
    os << getName() << std::endl;
    os << getPrice() << std::endl;
    os << getQty() << std::endl;
    os << isbn_ << std::endl;
    os << author_ << std::endl;
}
