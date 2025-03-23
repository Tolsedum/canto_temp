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

std::string canto_temp::parser_logic::getParams(
    std::vector<char> end_params,
    ContentReader &container
){
    std::string tag{};
    while (container.isNotEnd()){
        if(container.current() == '{'){
            //! IS throw Error
            throw "Not valid signature";
        }
        for (auto &&i : end_params){
            if(container.current() == '}' 
                || container.current() == i
            ){
                goto exit_to;
            }
        }
        tag.append(1, container.current());
        container.next();
    }
exit_to:
    ltrim(tag);
    rtrim(tag);
    return tag;
}

std::string canto_temp::parser_logic::getParams(
    // std::size_t end_tag_pos,
    char end_params,
    ContentReader &container
){
    std::string tag{};
    while (container.isNotEnd()){
        if(container.current() == '{'){
            //! IS throw Error
            throw "Not valid signature";
        }
        if(container.current() == '}' 
            || container.current() == end_params // '%'
        ){
            break;
        }
        tag.append(1, container.current());
        container.next();
    }
    ltrim(tag);
    rtrim(tag);
    return tag;
}

bool canto_temp::parser_logic::isNumeric(
    std::string str
){
    short count_dot = 0;
    return !str.empty()
        && std::find_if(
            str.begin(),
            str.end(),
            [&count_dot](unsigned char c){
                if(c == '.'){
                    count_dot++;
                    if(count_dot > 1){
                        return true;
                    }
                }
                return !(std::isdigit(c) || c == '.' || c == '-');
            }
        ) == str.end();
}

void canto_temp::parser_logic::skipTo(
    char c,
    ContentReader &container
){
    while (container.isNotEnd()){
        if(container.current() == c){
            container.next();
            break;
        }
        container.next();
    }
}

void canto_temp::parser_logic::skipTo(
    std::string str,
    ContentReader &container
){
    std::size_t pos = container.find(
        str, container.pos()
    );
    if(pos != std::string::npos){
        container.setPos(pos+1);
    }else{
        // ToDo throw
    }
}