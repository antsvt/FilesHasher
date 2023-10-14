#include "utils.h"

#include <iostream>

void Fatal(const std::string& what)
{
    std::string message = what + '\n';
    std::cerr << message;
    exit(-1);
}