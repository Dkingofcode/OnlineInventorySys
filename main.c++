#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <mutex>
#include <shared_mutex>
#include "crow.h"
using namespace std;


// Book class

class Book { 

  public:  
       int ISBN;
       string title;
       string author;
       double price; 
       string publication_date;

       Book(int isbn, string t, string a, double p, string pd) :
       ISBN(isbn), title(t), author(a), price(p), publication_date(pd) {}
};


   // Hashtable class

      // class HashTable {
      //    unordered_map<int, Book> table;

      //    public:
      //       void insert(int key, Book book){
      //         table[key] = book;
      //       }  

      //       void remove(int key){
      //         table.erase(key);
      //       }


      // Book* search(int key){
      //   auto it = table.find(key);
      //   if(it != table.end()){
      //     return &it->second;
      //   }
      //   return nullptr;
      //    }
      //   };

      


// Balanced Binary Tree 
      class  BookStoreTree {
        std::map<int, Book> tree; // ISBN as the key

        public:
             void insert(Book book){
               tree[book.ISBN] = book;
             }

             void remove(int key){
              tree.erase(key);
             }

             Book* search(int key){
                 auto it = tree.find(key);
                 if (it != tree.end()) {
                    return &it->second;
                 }
                 return nullptr;
             }

         
     void displayByPrice() {
        for(const auto& pair : tree){
          std::cout << "ISBN: " << pair.second.ISBN << ", Title: " << pair.second.title << ", Price: " << pair.second.price << std::endl;
        }
     }

     void displayByDate() {
      // Sorting by Date will require a different structure or custom comparator
      // Here we are just displaying by order of insertion (which is by ISBN in this case)
      for(const auto&pair : tree){
        std::cout << "ISBN: " << pair.second.ISBN << ", Title" << pair.second.title << ", Publication Date: " << pair.second.publication_date << std::endl;
        }
       };
      };

















class BookStoreInventory {
    private:
          unordered_map<int, Book> inventory;
          map<double, vector<Book>> priceIndex;
          map<string, vector<Book>> dateIndex;
          shared_mutex mutex_;

          void indexBook(const Book& book) {
             priceIndex[book.price].push_back(book);
             dateIndex[book.publication_date].push_back(book);
          }

  
  public:
    void addBook(const Book& book){
       unique_lock lock(mutex_);
       inventory[book.ISBN] = book;
       indexBook(book);
    };

    void removeBook(int isbn){
       unique_lock lock(mutex_);
       if(inventory.find(isbn) != inventory.end()){
         Book book = inventory[isbn];
         inventory.erase(isbn);

         // Remove from indices
         auto& priceVec = priceIndex[book.price];
         priceVec.erase(remove(priceVec.begin(), priceVec.end(), book), priceVec.end());

         auto& dateVec = dateVec[book.publication_date];
         dateVec.erase(remove(dateVec.begin(), dateVec.end(), book), dateVec.end());
       }
    };

    Book* searchByISBN(int isbn) {
      shared_lock lock(mutex_);
      if(inventory.find(isbn) != inventory.end()) {
         return inventory[isbn];
      } 
      throw runtime_error("Book not found");
    };

    vector<Book> searchByPrice(double price) {
      shared_lock lock(mutex_);
      if (priceIndex.find(price) != priceIndex.end()) {
         return priceIndex[price];
      }
      return {};
    }

    vector<Book> searchByDate(const string& date){
      shared_lock lock(mutex_);
      if(dateIndex.find(date) != dateIndex.end()) {
         return dateIndex[date];
      }
      return {};
    }

    void displayAllBooks(){
      shared_lock lock(mutex_);
         for(const auto& book : inventory){
            cout << "ISBN: " << book.ISBN << ", Title: " << book.title << ", Price: " << price << endl;
         }
    }


    void displayAllBooksByPrice(){
      shared_lock lock(mutex_);
      for (const auto& [price, books] : priceIndex){
         for(const auto& book : books){
            cout << "ISBN: " << book.ISBN << ", Title: " << book.title << ", Price: " << price << endl;
         }
      }
    };

    void displayAllBooksByDate() {
        shared_lock lock(mutex_);
        for (const auto& [date, books] : dateIndex) {
         for(const auto& book : books) {
            cout << "ISBN: " << book.ISBN << 
         }
        }
    }
};






int main() {
    BookStoreInventory inventory;

    crow::SimpleApp app;

    CROW_ROUTE(app, "/add").methods("POST"_method)([&inventory](const crow::request& req){
      auto x = crow::json::load(req.body);
      if(!x){
         return crow::response(400);
      }

      Book book(x["ISBN"].i(), x["title"].s(), x["author"].s(), x["price"].d(), x["publication_date"].s());
      inventory.addBook(book);
      return crow::response(200);
    });

    CROW_ROUTE(app, "/search/<int>").methods("GET"_method)([&inventory](int isbn){
        try{
          Book book = inventory.searchByISBN(isbn);
          crow::json::xvalue x;
          x["ISBN"] = book.ISBN;
          x["title"] = book.title;
          x["author"] = book.author;
          x["price"] = book.price;
          x["publication_date"] = book.publication_date;
          return crow::response(x);
        } catch (const runtime_error& e){
           return crow::response(404);
        }
    });

    // Add more routes for removing books and displaying all books
     
     // Delete Book
     CROW_ROUTE(app, "/delete/<int>").methods("POST"_method)([&inventory](int isbn){
        try{
           inventory.removeBook(isbn);
           return crow::response(200);
        }catch(const runtime_error& e){
           return crow::response(404);
        }
     });

     // Display All Books
     CROW_ROUTE(app, "/Books").methods("GET"_method)([&inventory](int isbn){
        try{
          inventory.displayAllBooks();
          return crow::response(200);
        }catch(const runtime_error& e){
           return crow::response(404);
        }
     });
        
     app.port(18080).multithreaded().run();   


   //  Book book1(123, "Book One", "Author A", 29.99, "2024-01-01");
   //  Book book2(456, "Book Two", "Author B", 19.99, "2023-06-15");
   //  Book book3(789, "Book Three", "Author C", 39.99, "2025-03-21");

   //  inventory.add(book1);
   //  inventory.add(book2);
   //  inventory.add(book3);

   //  std::cout << "All books sorted by price:" << std::endl;
   //  inventory.displayAllSortedByPrice();

   //  std::cout << "\nAll books sorted by publication date:" << std::endl;
   //  inventory.displayAllSortedByDate();

   //  return 0;
}




















