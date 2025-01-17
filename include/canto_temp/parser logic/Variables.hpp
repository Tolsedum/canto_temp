#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include <map>
#include <vector>
#include "canto_temp/Container.hpp"
#include "canto_temp/Object.hpp"

namespace canto_temp{
namespace parser_logic{

    class Variables{
        std::map<std::string, Object>* obj_list_;
        Container* container_;

    public:
        Variables(
            std::map<std::string, Object> *obj_list,
            Container *container
        )
            : obj_list_(obj_list)
            , container_(container)
        {};
        ~Variables(){};

        std::string render(std::size_t end_tag_pos);
    };

} // parser_logic
} // canto_temp

#endif // VARIABLES_HPP