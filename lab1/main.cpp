#include <iostream>

#include "rb_map.h"


int main() {

    rb_map<std::string, std::string> mMap;
    mMap["a"] = "b";
    mMap["b"] = "c";
    mMap["b"] = "a";
    mMap.remove("b");
    mMap.print();

    return 0;
}