
#include "utils.hpp"

void str_tolower(std::string &str)
{
    for (unsigned long  i = 0; i < str.length(); i++)
        str[i] = tolower(str[i]);
}