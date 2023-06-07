#include "elements.h"
#include <map>
std::map<Elements, std::string> elements_string = {
    {Elements::ERROR, "error"},
    {Elements::ERROR_MESSAGE, "error_message"},
    {Elements::WARNING, "warning"},
    {Elements::WARNING_MESSAGE, "warning_message"},
    {Elements::INFO, "info"},
    {Elements::DEBUG, "debug"},
    {Elements::DEBUG_MESSAGE, "debug_message"},
};