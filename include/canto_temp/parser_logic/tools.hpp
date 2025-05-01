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
        if(token.getCell() == T::Cell::space){
            while (token.getCell() == T::Cell::space){
                token.next();
            }
        }
    }

    template<class T, class Cell>
    std::string scanContentByCloseTag(T& token, std::string tag_name, Cell){
        int open_teg = 1;
        std::string ret_value{};
        while (token.getIterCount()){
            Cell cell = token.getCell();
            if(cell == Cell::eof){
                break;
            }else if(cell == Cell::comment_open){
                skipBy (
                    token, Cell::comment_close
                );
                token.next();
                continue;
            }else if(cell == Cell::var_open){
                token.next();
                ret_value.append(
                    "{{ " 
                    + getStringByCell(
                        token, Cell::var_close)
                    + " }}"
                );
                token.next();
                continue;
            }else if(cell == Cell::instruction_open){
                token.next();
                skipSpace(token);
                std::string name_instruction = getWord(
                    token, Cell::id
                );
                if(name_instruction == tag_name){
                    open_teg++;
                }else if(name_instruction == "end" + tag_name){
                    open_teg--;
                }
                if(open_teg == 0){
                    token.next();
                    break;
                }
                ret_value.append(
                    "{% " 
                    + name_instruction 
                    + getStringByCell(
                        token, Cell::instruction_close)
                    + " %}"
                );
                token.next();
                continue;
            }
            ret_value.append(1, token.getCurrent());
            token.next();
        }
        return ret_value;
    }
}// parser_logic
}// canto_temp

#endif // TOOLS_HPP