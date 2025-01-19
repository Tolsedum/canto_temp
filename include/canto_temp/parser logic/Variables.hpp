#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include <map>
#include <vector>
#include <functional>
#include "canto_temp/Container.hpp"
#include "canto_temp/Dictionary.hpp"

namespace canto_temp{
namespace parser_logic{

    class Variables{
        std::map<std::string, std::function<void(std::string&)>> 
            func_;
        std::map<std::string, Dictionary>* obj_list_;
        std::weak_ptr<Container> container_;

        std::string eraseByNidle(std::string& str, char nidle);
        std::string getVar(std::string tag);
        Dictionary readVar();

        std::string getV();
    public:
        Variables(
            std::map<std::string, Dictionary> *obj_list,
            std::shared_ptr<Container>& container
        )
            : obj_list_(obj_list)
            , container_(container)
        {
            addFilterFunctions("upper", [](std::string& str){
                std::transform(
                    str.begin(), 
                    str.end(), 
                    str.begin(), 
                    ::toupper
                );
            });
        };
        ~Variables(){};

        void addFilterFunctions(
            std::string func_name, std::function<void(std::string&)>
        );
        std::string render(std::size_t end_tag_pos);
    };

} // parser_logic
} // canto_temp

#endif // VARIABLES_HPP