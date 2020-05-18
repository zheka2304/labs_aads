#include <iostream>

#include "rb_map.h"


int main() {
    rb_map<std::string, std::string> map;

    while (true) {
        std::cout << "\n\nmap: ";
        map.print();
        std::cout << "\n\ntype 1 to insert value\ntype 2 to remove value\ntype 3 to get value\ntype 4 to debug red-black tree\ntype any other number to exit\n";

        int choice = 1;
        std::cin >> choice;
        switch (choice) {
            case 1: {
                std::string key, value;
                std::cout << "input key: ";
                std::cin >> key;
                std::cout << "input value: ";
                std::cin >> value;
                map[key] = value;
                break;
            }
            case 2: {
                std::string key, value;
                std::cout << "input key: ";
                std::cin >> key;
                if (map.remove(key)) {
                    std::cout << "value was removed\n";
                } else {
                    std::cout << "no such key\n";
                }
                break;
            }
            case 3: {
                std::string key, value;
                std::cout << "input key: ";
                std::cin >> key;
                if (map.has(key)) {
                    std::cout << "value at " << key << " = " << map[key] << "\n";
                } else {
                    std::cout << "no such key\n";
                }
                break;
            }
            case 4: {
                map.show_tree();
                break;
            }
            default:
                return 0;
        }
    }
}