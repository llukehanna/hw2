#include "clothing.h"
#include "util.h"
#include <sstream>
#include <iomanip>

Clothing::Clothing(const std::string & name, double price, int qty,
                   const std::string & size, const std::string & brand)
    : Product("clothing", name, price, qty), size_(size), brand_(brand)
{
}

Clothing::~Clothing() { }

std::set<std::string> Clothing::keywords() const {
    // Build keywords directly from the product name and brand.
    std::set<std::string> keys = parseStringToWords(getName());
    std::set<std::string> brandKeys = parseStringToWords(brand_);
    keys.insert(brandKeys.begin(), brandKeys.end());
    // Also insert the size as a keyword.
    keys.insert(size_);
    return keys;
}

std::string Clothing::displayString() const {
    std::ostringstream oss;
    oss << getName() << "\n"
        << "Size: " << size_ << " Brand: " << brand_ << "\n"
        << std::fixed << std::setprecision(2) << getPrice() << " " 
        << getQty() << " left.";
    return oss.str();
}


void Clothing::dump(std::ostream& os) const {
    os << "clothing" << "\n"
       << getName() << "\n"
       << getPrice() << "\n"
       << getQty() << "\n"
       << size_ << "\n"
       << brand_ << "\n";
}
