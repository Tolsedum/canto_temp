#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include <map>
#include <algorithm>
#include <cctype>
#include <locale>
#include "canto_temp/Dictionary.hpp"
#include "canto_temp/Container.hpp"

namespace canto_temp{
namespace parser_logic{

    class Variables{
        std::map<std::string, std::function<void(Dictionary&)>> 
            func_;
        std::map<std::string, Dictionary>* obj_list_;

        int count_read_tag;
        // std::weak_ptr<Container> container_;

        // std::string eraseByNidle(std::string& str, char nidle);
        // std::string getVar(std::string tag);
        // Dictionary readVar();
    
        
    public:
        Variables(
            std::map<std::string, Dictionary> *obj_list
        )
            : obj_list_(obj_list)
            // , container_(container)
        {
            count_read_tag = 0;
            addFilterFunctions("upper", [](Dictionary& dict){
                std::string str = dict.toString();
                std::transform(
                    str.begin(), 
                    str.end(), 
                    str.begin(), 
                    ::toupper
                );
                dict = str;
            });
        };
        ~Variables(){};

        void addFilterFunctions(
            std::string func_name, std::function<void(Dictionary&)>
        );
        // std::string render(std::size_t end_tag_pos);
        Dictionary getDicVar(std::string tag);
        std::string getVar(std::string str_var);
        // ToDo Создать получение Dict

        void ltrim(std::string &s);
        void rtrim(std::string &s);
    };

} // parser_logic
} // canto_temp

#endif // VARIABLES_HPP