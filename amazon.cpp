#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"
#include "mydatastore.h"

using namespace std;

struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};

void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    MyDataStore ds;

    // Instantiate the section parsers.
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ..." << endl;
    cout << "  OR term term ..." << endl;
    cout << "  ADD username search_hit_number" << endl;
    cout << "  VIEWCART username" << endl;
    cout << "  BUYCART username" << endl;
    cout << "  QUIT new_db_filename" << endl;
    cout << "====================================" << endl;

    vector<Product*> lastHits;  // store the most recent search results
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if(ss >> cmd) {
            if(cmd == "AND") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                lastHits = ds.search(terms, 0);
                displayProducts(lastHits);
            }
            else if(cmd == "OR") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                lastHits = ds.search(terms, 1);
                displayProducts(lastHits);
            }
            else if(cmd == "ADD") {
                string username;
                int hit;
                if(ss >> username >> hit) {
                    if(ds.addToCart(username, hit, lastHits))
                        cout << "Added product " << hit << " to " << username << "'s cart." << endl;
                    else
                        cout << "Failed to add to cart." << endl;
                }
                else {
                    cout << "Invalid ADD command." << endl;
                }
            }
            else if(cmd == "VIEWCART") {
                string username;
                if(ss >> username) {
                    vector<Product*> cart = ds.viewCart(username);
                    if(cart.empty())
                        cout << "Cart is empty." << endl;
                    else {
                        for (size_t i = 0; i < cart.size(); i++) {
                            cout << "Cart Item " << i+1 << ":" << endl;
                            cout << cart[i]->displayString() << endl;
                        }
                    }
                }
                else {
                    cout << "Invalid VIEWCART command." << endl;
                }
            }
            else if(cmd == "BUYCART") {
                string username;
                if(ss >> username) {
                    if(ds.buyCart(username))
                        cout << username << "'s cart purchased successfully." << endl;
                    else
                        cout << "Purchase failed for " << username << "." << endl;
                }
                else {
                    cout << "Invalid BUYCART command." << endl;
                }
            }
            else if(cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
            else {
                cout << "Unknown command" << endl;
            }
        }
    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if(hits.empty()) {
        cout << "No results found!" << endl;
        return;
    }
    sort(hits.begin(), hits.end(), ProdNameSorter());
    for(auto it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}
