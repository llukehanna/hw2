#include "mydatastore.h"
#include "util.h"  // for convToLower, setIntersection, setUnion
#include <iterator>
#include <iostream>  // For debugging if needed

MyDataStore::MyDataStore() { }

MyDataStore::~MyDataStore() {
    for (size_t i = 0; i < products_.size(); i++) {
        delete products_[i];
    }
    for (size_t i = 0; i < users_.size(); i++) {
        delete users_[i];
    }
}



void MyDataStore::addUser(User* u) {
    users_.push_back(u);
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type) {
    std::set<Product*> resultSet;
    bool firstTerm = true;
    
    for (size_t i = 0; i < terms.size(); i++) {
    std::string term = convToLower(terms[i]);
    term = trim(term);
    if (keywordMap_.find(term) != keywordMap_.end()) {
        if (firstTerm) {
            resultSet = keywordMap_[term];
            firstTerm = false;
        } else {
            if (type == 0) { // AND search
                resultSet = setIntersection(resultSet, keywordMap_[term]);
            } else { // OR search
                resultSet = setUnion(resultSet, keywordMap_[term]);
            }
        }
    } else {
        if (type == 0) { // AND search: missing term means no match
            resultSet.clear();
            break;
        }
    }
}
return std::vector<Product*>(resultSet.begin(), resultSet.end());

} 

void MyDataStore::addProduct(Product* p) {
    products_.push_back(p);
    std::set<std::string> keys = p->keywords();
    for (std::set<std::string>::iterator it = keys.begin(); it != keys.end(); ++it) {
        std::string key = convToLower(*it);
        key = trim(key);
        keywordMap_[key].insert(p);
    }
}


void MyDataStore::dump(std::ostream &ofile) {
    // Output the products section exactly as expected.
    ofile << "<products>" << std::endl;
    for (size_t i = 0; i < products_.size(); i++) {
         // Each product's dump() should output plain text fields (one per line)
         products_[i]->dump(ofile);
    }
    ofile << "</products>" << std::endl;
    
    // Output the users section exactly as expected.
    ofile << "<users>" << std::endl;
    for (size_t i = 0; i < users_.size(); i++) {
         users_[i]->dump(ofile);
    }
    ofile << "</users>" << std::endl;
}


// Returns a pointer to the user with the given username, or nullptr if not found.
User* MyDataStore::findUser(const std::string& username) const {
    for (size_t i = 0; i < users_.size(); i++) {
        if (users_[i]->getName() == username)
            return users_[i];
    }
    return nullptr;
}

// Adds a product from the searchHits (hit is 1-indexed) to the user's cart.
bool MyDataStore::addToCart(const std::string& username, int hit, const std::vector<Product*>& searchHits) {
    User* u = findUser(username);
    if (!u) return false;
    if (hit < 1 || hit > (int)searchHits.size()) return false;
    carts_[username].push_back(searchHits[hit - 1]);
    return true;
}

std::vector<Product*> MyDataStore::viewCart(const std::string& username) const {
    if (!findUser(username))   // If user not found, return empty vector.
        return std::vector<Product*>();
    if (carts_.find(username) != carts_.end())
        return carts_.at(username);
    return std::vector<Product*>();
}


// Attempts to buy all items in the user's cart.
// For simplicity, assume that each product purchase reduces its quantity by 1,
// and that the user has sufficient funds (or we simulate insufficient funds based on user balance).
// We'll assume the User class has getBalance() and subtractBalance(double) methods.
// (If not, you can adjust accordingly.)
bool MyDataStore::buyCart(const std::string& username) {
    User* u = findUser(username);
    if (!u) return false;
    std::vector<Product*>& cart = carts_[username];
    
    // Count frequency for each product in the cart.
    std::map<Product*, int> freq;
    double totalCost = 0;
    for (Product* p : cart) {
        freq[p]++;
        totalCost += p->getPrice();
    }
    
    // Check if the user has enough balance.
    if (u->getBalance() < totalCost)
        return false;
    
    // Check that every product has enough stock.
    for (auto& pair : freq) {
        Product* p = pair.first;
        int count = pair.second;
        if (p->getQty() < count)
            return false;
    }
    
    // Deduct the balance.
    u->subtractBalance(totalCost);
    
    // Decrement product quantities.
    for (auto& pair : freq) {
        Product* p = pair.first;
        int count = pair.second;
        p->subtractQty(count);
    }
    
    // Clear the cart.
    cart.clear();
    return true;
}


