// src/libutils/utils.cpp

#include "libutils/utils.h"
#include <iostream>

namespace WorkshopApp {

void print_message(const std::string& message) {
    std::cout << "UTIL_LOG: " << message << std::endl;
}

} // namespace WorkshopApp