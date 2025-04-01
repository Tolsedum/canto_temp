#ifndef VARIABLES_HPP
#define VARIABLES_HPP

#include "canto_temp/parser_logic/tools.hpp"
#include "canto_temp/parser_logic/Token.hpp"

namespace canto_temp{
namespace parser_logic{

    class Variables{
        int count_read_tag;

        std::map<std::string, std::function<void(nlohmann::json&)>> 
            func_;
        std::map<std::string, nlohmann::json>* obj_list_;
        ContentReader container_;

        // std::string eraseByNidle(std::string& str, char nidle);
        // std::string getVar(std::string tag);
        // nlohmann::json readVar();
    
        // std::string get


        void skipSpace(Token& token);
        void skipBy(Token& token, Token::Cell by);
        int scanNumeric(Token &token);
        std::string scanString(Token &token);
        std::string scanIdName(Token &token);
        nlohmann::json scanIdValue(Token &token, nlohmann::json dict);
        
        nlohmann::json scanVariable(
            Token &token, nlohmann::json dict, 
            Token::Cell after = Token::Cell::unknown
        );

    public:
        Variables(
            std::map<std::string, nlohmann::json> *obj_list,
            ContentReader &container
        );
        ~Variables(){};

        void addFilterFunctions(
            std::string func_name, std::function<void(nlohmann::json&)>
        );
       
        std::string getVar(
            std::string str_var
        );
        std::string getVar();

        nlohmann::json parseOperation(void);
        nlohmann::json parseVariable(void);

        nlohmann::json getDicVar(std::string var = "");

        bool getBoolDicVar(nlohmann::json dic);
        bool isSetDicVar(nlohmann::json &dic);
        bool isEmptyDicVar(nlohmann::json &dic);
        bool compare(
            nlohmann::json dic, nlohmann::json dic1, char comp
        );


        void setVar(std::string var_name, nlohmann::json new_var);

    };

} // parser_logic
} // canto_temp

#endif // VARIABLES_HPP