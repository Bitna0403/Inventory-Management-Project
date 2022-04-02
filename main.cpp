#include <iostream>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <functional>

constexpr auto MAX_ITEMS = 30;


enum class Types
{
  Fruit,
  Vegetables,
  Diary,
  Clothes,
  Other
};

struct Item
{
  std::string name;
  std::string barcode;
  std::string bestBeforeDate;
  float price;

  Item() = default;

  Item(const std::string& name, const std::string& barcode, const std::string& bestBeforeDate, const int price) :
          name {name}, barcode {barcode}, bestBeforeDate {bestBeforeDate}, price {price}
  {}

  auto print() const 
  { 
    std::cout <<  barcode << std::setw(20) << name << std::setw(20) << bestBeforeDate << std::setw(20) << price << std::endl;
  }
};

struct Inventory
{
  using Items = std::vector<Item>;
  using ItemP = Items::iterator;  
  using FindPredicate = std::function<bool(const Item&)>;

  Items items;

  Inventory() { items.reserve(MAX_ITEMS); }

  auto add(const Item& item) { items.emplace_back(item); }
  auto remove(ItemP pItem) { items.erase(pItem); }

  auto list()
  {
    std::cout  << "Barcode" << std::setw(20) << "Name" << std::setw(20) << "Best Before" << std::setw(20) << "Price" << std::endl;
    
    std::for_each(items.begin(), items.end(), [](const auto& item) { 
      item.print(); 
    });
    
    std::cout << std::endl;
  }

  auto find(const FindPredicate& predicate) -> ItemP
  {
    auto pItem = std::find_if(items.begin(), items.end(), predicate);
    
    if (pItem != items.end()) 
    { 
      return pItem; 
    }

    return {};
  }
};

void PrintItem(Inventory::ItemP& pItem)
{
  std::cout  << "Barcode" << std::setw(20) << "Name" << std::setw(20) << "Best Before" << std::setw(20) << "Price" << std::endl;
  
  pItem->print();
}

void AddItem(Inventory& inventory) 
{
  Item item;
  
  std::cin.ignore(1, '\n');
  
  std::cout << "Please enter item details" << std::endl;

  std::cout << "Barcode: ";
  getline(std::cin, item.barcode);
  
  std::cout << "Name: ";
  getline(std::cin, item.name);

  std::cout << "Best Before Date (dd-mm-yyyy): ";
  getline(std::cin, item.bestBeforeDate);

  std::cout << "Price: ";
  std::cin >> item.price;

  system("clear");
  std::cout <<  "Item Added: " << item.name << "(" << item.barcode << ")" << std::endl;
  std::cout << std::endl;

  inventory.add(item);
}

Inventory::ItemP FindItem(Inventory& inventory)
{
  int findBy;
    
  std::cout << "1. Find by Barcode" << std::endl;
  std::cout << "2. Find by Name" << std::endl;

  std::cin.ignore(1, '\n');
  
  std::cout << "Find by: ";
  std::cin >> findBy;
  
  Inventory::ItemP pItem;

  if (findBy == 1) // find by Barcode
  {
    std::string barcode;

    std::cin.ignore(1, '\n');
    
    std::cout << "Enter Barcode: ";
    getline(std::cin, barcode);
    
    pItem = inventory.find([&](const Item& item) { return item.barcode == barcode; });

    system("clear");
    
    if (pItem != Inventory::ItemP {})
    {
      PrintItem(pItem);
      
      std::cout << std::endl;
      
      return pItem;
    } else {
      std::cout << "No item found with Barcode: " << barcode << std::endl;
      std::cout << std::endl;

      return {};
    }
  } else if(findBy == 2) { // find by name
    std::string name;

    std::cin.ignore(1, '\n');
    
    std::cout << "Enter Name: ";
    getline(std::cin, name);
    
    pItem = inventory.find([&](const Item& item) { return item.name == name; });

    system("clear");
    
    if (pItem != Inventory::ItemP {})
    {
      PrintItem(pItem);
      
      std::cout << std::endl;
      
      return pItem;
    } else {
      std::cout << "No item found with Name: " << name << std::endl;
      std::cout << std::endl;

      return {};
    }
  } else { // Invalid choice
    system("clear");
    
    std::cout << "Invalid choice, please try again..." << std::endl;
    std::cout << std::endl;
    
    return {};
  }
}

void EditItem(Inventory& inventory)
{
  std::cout << "First, find item to edit..." << std::endl;
  
  Inventory::ItemP pItem = FindItem(inventory);

  if (pItem != Inventory::ItemP {})
  { 
    std::string name;
  
    std::cout << "Please enter new item details.." << std::endl;
    std::cout << "Name (" << pItem->name << "): ";
    getline(std::cin, name);

    pItem->name = name;

    system("clear");
    
    PrintItem(pItem);

    std::cout << std::endl;
    std::cout << "Item Updated" << std::endl;
    std::cout << std::endl;
  }
}

void RemoveItem(Inventory& inventory)
{
  std::cout << "Find item to delete..." << std::endl;
  
  Inventory::ItemP pItem = FindItem(inventory);

  if (pItem != Inventory::ItemP {})
  { 
    inventory.remove(pItem);
    std::cout << "Item Removed" << std::endl;
    std::cout << std::endl;
  }
}

struct ManagementUI
{
  Inventory inventory;

  auto start()
  {

    Item coconut_item;
    coconut_item.name = "Coconut";
    coconut_item.barcode = "Coco";
    coconut_item.bestBeforeDate = "14-10-2023";
    coconut_item.price = 1.25;
    inventory.add(coconut_item);

    Item shampoo_item;
    shampoo_item.name = "Shampoo";
    shampoo_item.barcode = "sham";
    shampoo_item.bestBeforeDate = "66-99-2223";
    shampoo_item.price = 9.25;
    inventory.add(shampoo_item);

    
    do {
      int choiceFromMenu = 0;
    
      std::cout << "======== Bitna Store Inventory Management ==========" << std::endl;    
      std::cout << "1. Add Item" << std::endl;
      std::cout << "2. List Items" << std::endl;
      std::cout << "3. Find Item" << std::endl;
      std::cout << "4. Edit Item" << std::endl;
      std::cout << "5. Remove Item" << std::endl;
      std::cout << "6. Exit" << std::endl;
    
      std::cout << "Please choose from menu: ";
      
      std::cin >> choiceFromMenu;

      system("clear");

      if(choiceFromMenu == 1)
      {
        AddItem(inventory);
      } else if (choiceFromMenu == 2) {
        inventory.list();
      } else if (choiceFromMenu == 3) {
        FindItem(inventory);
      } else if (choiceFromMenu == 4) {
        EditItem(inventory); 
      } else if (choiceFromMenu == 5) {
        RemoveItem(inventory);
      } else if (choiceFromMenu == 6) {
        std::cout << "Exiting the Store Inventory Management... Goodbye" << std::endl;
        break;
      } else { 
        std::cout << "Please choose between 1 - 6, then press enter to continue..." << std::endl; 
      }
    } while(true);
  }
};

auto main() -> int
{
  ManagementUI managementUI {};

  managementUI.start();
}