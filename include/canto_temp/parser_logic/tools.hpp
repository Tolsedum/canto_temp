#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <map>
#include <string>
#include <vector>
#include <locale>
#include <cctype>
#include <algorithm>
#include "canto_temp/json.hpp"
#include "canto_temp/Container.hpp"

namespace canto_temp{
namespace parser_logic{

    // trim from start (in place)
    void ltrim(std::string &s);

    // trim from end (in place)
    void rtrim(std::string &s);

    std::string getParams(
        std::vector<char> end_params,
        std::shared_ptr<Container> container
    );

    std::string getParams(
        // std::size_t end_tag_pos,
        char end_params,
        std::shared_ptr<Container> container
    );

    bool isNumeric(
        std::string str
    );
    
    void skipTo(
        char c,
        std::shared_ptr<Container> container
    );
}// parser_logic
}// canto_temp

#endif // TOOLS_HPP