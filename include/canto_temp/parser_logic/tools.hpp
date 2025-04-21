#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <map>
#include <string>
#include <vector>
#include <locale>
#include <cctype>
#include <algorithm>
#include "canto_temp/json.hpp"
#include "ContentReader.hpp"
#include "canto_temp/parser_logic/Token.hpp"

namespace canto_temp{
namespace parser_logic{

    // trim from start (in place)
    void ltrim(std::string &s);

    // trim from end (in place)
    void rtrim(std::string &s);

    bool isEmptyDicVar(nlohmann::json &dict);
    bool getBoolDicVar(nlohmann::json dic);
    bool compare(nlohmann::json dic, nlohmann::json dic1, char comp);
    
    
    template<class T, class Cell>
    void skipBy(
        T& token, Cell by
    ){
        while (
            token.getCell() != Cell::eof
            && token.getCell() != by
        ){
            token.next();
        }
    }

    template<class T, typename Cell>
    std::string getWord(
        T &token, Cell
    ){
        std::string ret_var{};
        while (token.getIterCount()){
            Cell cell = token.getCell();
            if(cell == Cell::id || cell == Cell::number){
                ret_var.append(1, token.getCurrent());
            }else {
                break;
            }
            token.next();
        }
        return ret_var;
    }

    template<class T, typename Cell>
    std::string getStringByCell(
        T &token, Cell stop_cell
    ){
        std::string ret_var{};
        while (token.getIterCount()){
            Cell cell = token.getCell();
            if(stop_cell == cell){
                break;
            }else{
                ret_var.append(1, token.getCurrent());
            }
            token.next();
        }
        return ret_var;
    }

    template<class T>
    void skipSpace(T& token){
        while (token.getCell() == T::Cell::space){
            token.next();
        }
    }
}// parser_logic
}// canto_temp

#endif // TOOLS_HPP