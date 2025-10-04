#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include "product.h"
#include "user.h"
#include <vector>
#include <set>
#include <map>
#include <string>
#include <ostream>

class MyDataStore : public DataStore {
public:
    MyDataStore();
    virtual ~MyDataStore();

    // Interface methods
    virtual void addProduct(Product* p);
    virtual void addUser(User* u);
    virtual std::vector<Product*> search(std::vector<std::string>& terms, int type);
    virtual void dump(std::ostream& ofile);

    // Additional cart-related functions
    // Searches for a user by username; returns nullptr if not found.
    User* findUser(const std::string& username) const;

    // Add a product (from the last search hits) to the given user's cart.
    // hit is 1-indexed.
    bool addToCart(const std::string& username, int hit, const std::vector<Product*>& searchHits);

    // Returns the cart for the given user.
    std::vector<Product*> viewCart(const std::string& username) const;

    // Attempts to buy all items in the given user's cart.
    // (Assumes that a user has a balance and that each Product’s qty is decremented.)
    // Returns true if purchase was successful, false otherwise.
    bool buyCart(const std::string& username);

private:
    std::vector<Product*> products_;
    std::vector<User*> users_;
    // Map from keyword to set of products (for search)
    std::map<std::string, std::set<Product*>> keywordMap_;

    // Map from username to a vector of Products representing that user’s cart.
    std::map<std::string, std::vector<Product*>> carts_;
};

#endif
