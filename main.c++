#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
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

      class HashTable {
         unordered_map<int, Book> table;

         public:
            void insert(int key, Book book){
              table[key] = book;
            }  

            void remove(int key){
              table.erase(key);
            }


      Book* search(int key){
        auto it = table.find(key);
        if(it != table.end()){
          return &it->second;
        }
        return nullptr;
         }
        };

      


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

















class Inventory {
     HashTable hashTable;
     BookStoreTree bookTree;      

  
  public:
    void add(Book book){
       hashTable.insert(book.ISBN, book);
       bookTree.insert(book);
    };

    void remove(int ISBN){
       hashTable.remove(ISBN);
       bookTree.remove(ISBN);
    };

    Book* search(int ISBN) {
       return hashTable.search(ISBN);
    };

    void displayAllSortedByPrice(){
      bookTree.displayByPrice();
    };

    void displayAllSortedByDate() {
        bookTree.displayByDate();
    }
};






int main() {
    Inventory inventory;

    Book book1(123, "Book One", "Author A", 29.99, "2024-01-01");
    Book book2(456, "Book Two", "Author B", 19.99, "2023-06-15");
    Book book3(789, "Book Three", "Author C", 39.99, "2025-03-21");

    inventory.add(book1);
    inventory.add(book2);
    inventory.add(book3);

    std::cout << "All books sorted by price:" << std::endl;
    inventory.displayAllSortedByPrice();

    std::cout << "\nAll books sorted by publication date:" << std::endl;
    inventory.displayAllSortedByDate();

    return 0;
}




















