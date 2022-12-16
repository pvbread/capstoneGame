// pads strings with white space so that they get rendered properly in menus
// takes the longest string as reference
// may need to have a minimum length otherwise
#include "pch.h"

std::vector<std::string> padMenuStrings(const std::vector<std::string>& strs)
{
    int maxLength = INT_MIN;
    for (auto s: strs)
    {
        maxLength = std::max<int>(maxLength, s.length());
    }
    std::vector<std::string> padded;

    for (auto s: strs)
    {
        while(s.length() != maxLength)
        {
            s += " ";
        }
        padded.push_back(s);
    }

    return padded;
}