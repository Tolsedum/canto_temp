#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include "canto_temp/parser_logic/tools.hpp"
#include "canto_temp/Dictionary.hpp"

namespace canto_temp{
namespace parser_logic{

    class Variables{
        int count_read_tag;

        std::map<std::string, std::function<void(nlohmann::json&)>> 
            func_;
        std::map<std::string, nlohmann::json>* obj_list_;
        std::shared_ptr<Container> container_;

        // std::string eraseByNidle(std::string& str, char nidle);
        // std::string getVar(std::string tag);
        // nlohmann::json readVar();
    
        // std::string get
        
    public:
        Variables(
            std::map<std::string, nlohmann::json> *obj_list,
            std::shared_ptr<Container> container
        );
        ~Variables(){};

        void addFilterFunctions(
            std::string func_name, std::function<void(nlohmann::json&)>
        );
        // std::string render(std::size_t end_tag_pos);
        nlohmann::json getDicVar(
            std::string tag
        );
        std::string getVar(
            std::string str_var
        );

        bool getBoolDicVar(nlohmann::json dic);
        bool isSetDicVar(nlohmann::json &dic);
        bool isEmptyDicVar(nlohmann::json &dic);
        bool compare(
            nlohmann::json dic, nlohmann::json dic1, char comp
        );

        nlohmann::json getDicVar();
        std::string getVar();

        void setVar(std::string var_name, nlohmann::json new_var);

    };

} // parser_logic
} // canto_temp

#endif // VARIABLES_HPP