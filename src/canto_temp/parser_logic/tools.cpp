#include "canto_temp/parser_logic/tools.hpp"


// trim from start (in place)
void canto_temp::parser_logic::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void canto_temp::parser_logic::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

bool canto_temp::parser_logic::isEmptyDicVar(nlohmann::json &dict){
    return dict.is_null()
        ? true
        : (dict.is_boolean()
            ? !dict.get<bool>()
            : (dict.is_string() 
                ? std::string(dict).empty()
                : (dict.is_number() 
                    ? dict.get<float>() != 0
                    : dict.empty()
                )));
}

bool canto_temp::parser_logic::getBoolDicVar(nlohmann::json dic){
    return !isEmptyDicVar(dic);
}

bool canto_temp::parser_logic::compare(
    nlohmann::json dic, nlohmann::json dic1, char comp
){
    if(comp == '='){
        return dic == dic1;
    }else if(comp == '!'){
        return dic != dic1;
    }
    return false;
}

