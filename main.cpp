#include <iostream>
#include <cstdio>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <functional>
#include <optional>

constexpr auto MAX_ITEMS = 30;

enum class Type
{
  Fruit,
  Vegetables,
  Diary,
  Clothes,
  Other
};

constexpr std::string_view TYPE_DISPLAY_NAMES[5] = {
  [static_cast<int>(Type::Fruit)] = "Fruit",
  [static_cast<int>(Type::Vegetables)] = "Vegetables",
  [static_cast<int>(Type::Diary)] = "Diary",
  [static_cast<int>(Type::Clothes)] = "Clothes",
  [static_cast<int>(Type::Other)] = "Other",
};

constexpr auto IsTypeValid(int type) { 
  return type >= 0 && type < 5; 
}

constexpr auto GetTypeName(int type){
  if (!IsTypeValid(type)) { return std::string_view {""}; }

  return TYPE_DISPLAY_NAMES[type];
}

struct Item
{
  std::string name;
  std::string barcode;
  std::string bestBeforeDate;
  int quantity;
  float price;
  int typeId;

  Item() = default;

  Item(const std::string& barcode, const std::string& name, const std::string& bestBeforeDate, const float price, const int quantity, const int typeId) :
          name {name}, barcode {barcode}, bestBeforeDate {bestBeforeDate}, price {price}, quantity {quantity}, typeId {typeId}
  {}

  auto print() const 
  { 
    std::cout << GetTypeName(typeId) << std::setw(20) << barcode << std::setw(20) << name << std::setw(20) << bestBeforeDate << std::setw(20) << price << std::setw(20) << quantity << std::endl;
  }
};

struct Inventory
{
  using Items = std::vector<Item>;
  using ItemP = Items::iterator;  
  using FindPredicate = std::function<bool(const Item&)>;

  Items items;

  Inventory() { items.reserve(MAX_ITEMS); }

  /// @brief Adds the item to the inventory.
  auto add(const Item& item) { items.emplace_back(item); }

  /// @brief Removed the item from inventory.
  auto remove(ItemP pItem) { items.erase(pItem); }

  /// @brief lists all items in the inventory.
  auto list()
  {
    std::cout << "Type" << std::setw(20) << "Barcode" << std::setw(20) << "Name" << std::setw(20) << "Best Before" << std::setw(20) << "Price" << std::setw(20) << "Quantity" << std::endl;
    
    std::for_each(items.begin(), items.end(), [](const auto& item) { 
      item.print(); 
    });
    
    std::cout << std::endl;
  }

  /// @brief Finds an item in the inventory.
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

auto PrintItem(Inventory::ItemP& pItem)
{
  std::cout << "Type" << std::setw(20) << "Barcode" << std::setw(20) << "Name" << std::setw(20) << "Best Before" << std::setw(20) << "Price" << std::setw(20) << "Quantity" << std::endl;
  
  pItem->print();
}

auto PrintTypes()
{
  std::cout << "Types: " << std::endl;
  
  std::for_each_n(std::begin(TYPE_DISPLAY_NAMES), std::size(TYPE_DISPLAY_NAMES), [i = 0](const auto& name) mutable {
    std::cout << i << ". " << name.data() << std::endl;
    i++;
  });
  
  std::cout << std::endl;
}

void AddItem(Inventory& inventory) 
{
  Item item;

  do {
    PrintTypes();
    
    std::cout << "Select product category to add: ";
    std::cin >> item.typeId;

    if (IsTypeValid(item.typeId)) {
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
    
      std::cout << "Quantity: ";
      std::cin >> item.quantity;
    
      system("clear");
      std::cout <<  "Item Added: " << item.name << "(" << item.barcode << ")" << std::endl;
      std::cout << std::endl;
    
      inventory.add(item);

      break;
    } else {
      system("clear");
      
      std::cout << "Invalid option selected. Please try again." << std::endl; 
    }
  } while(true);
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
    {// if item was found
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
    { // if item was found
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
    std::string bestBeforeDate;
    float price;
    int quantity;
  
    std::cout << "Please enter new item details.." << std::endl;
    std::cout << "Name (" << pItem->name << "): ";
    getline(std::cin, name);

    std::cout << "Best Before (" << pItem->bestBeforeDate << "): ";
    getline(std::cin, bestBeforeDate);

    std::cout << "Price (" << pItem->price << "): ";
    std::cin >> price;

    std::cout << "Quantity (" << pItem->quantity << "): ";
    std::cin >> quantity;

    pItem->name = name;
    pItem->bestBeforeDate = bestBeforeDate;
    pItem->price = price;
    pItem->quantity = quantity;

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
    coconut_item.typeId = 0;
    coconut_item.name = "Coconut";
    coconut_item.barcode = "Coco";
    coconut_item.bestBeforeDate = "14-10-2023";
    coconut_item.price = 1.25;
    coconut_item.quantity = 5;
    inventory.add(coconut_item);

    Item shampoo_item;
    shampoo_item.typeId = 4;
    shampoo_item.name = "Shampoo";
    shampoo_item.barcode = "sham";
    shampoo_item.bestBeforeDate = "66-99-2223";
    shampoo_item.price = 9.25;
    shampoo_item.quantity = 10;
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