#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include "canto_temp/parser_logic/tools.hpp"
// #include "canto_temp/parser_logic/Token.hpp"

namespace canto_temp{
namespace parser_logic{

    class Variables{
        int count_read_tag_, *iter_count_;
        Token::Cell pre_cell_;

        std::map<std::string, std::function<void(nlohmann::json&)>> 
            func_;
        std::map<std::string, nlohmann::json>* obj_list_;
        ContentReader container_;

    protected:
        void scanOperators(Token &token, nlohmann::json& object_value);
        void scanBitOr(Token &token, nlohmann::json& object_value);
        
        
        

        std::string scanString(Token &token);
        nlohmann::json scanNumeric(Token &token);
        nlohmann::json scanVariable(
            Token &token, nlohmann::json dict, 
            Token::Cell before = Token::Cell::unknown
        );
        nlohmann::json scanLeftBracket(
            Token &token, nlohmann::json dict
        );

    public:
        Variables(
            std::map<std::string, nlohmann::json> *obj_list,
            ContentReader &container,
            int &iter_count
        );
        ~Variables(){};

        void addFilterFunctions(
            std::string func_name, std::function<void(nlohmann::json&)>
        );

        void setVar(std::string var_name, nlohmann::json new_var);
        
        

        std::string getVar(void);
        nlohmann::json parseVariable(void);
    };

} // parser_logic
} // canto_temp

#endif // VARIABLES_HPP