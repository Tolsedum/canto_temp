#include "canto_temp/parser_logic/Variables.hpp"

canto_temp::parser_logic::Variables::Variables(
    std::map<std::string, nlohmann::json> *obj_list,
    ContentReader &container,
    int &iter_count
)
    : obj_list_(obj_list)
    , container_(container)
{
    iter_count_ = &iter_count;
    pre_cell_ = Token::Cell::unknown;
    count_read_tag_ = 0;

    std::map<std::string, std::function
        <void(nlohmann::json&, nlohmann::json)>> fn_list{
            {"upper", parser_logic::upper},
            {"lower", parser_logic::lower},
            {"abs", parser_logic::abs_},
            {"batch", parser_logic::batch},
            {"capitalize", parser_logic::capitalize},
            {"column", parser_logic::column},
            {"default", parser_logic::default_},
            {"escape", parser_logic::escape},
            {"first", parser_logic::first},
            {"format", parser_logic::format},
            {"join", parser_logic::join},
            {"json_encode", parser_logic::json_encode},
            {"keys", parser_logic::keys},
            {"last", parser_logic::last},
            {"length", parser_logic::length},
            {"merge", parser_logic::merge},
            {"nl2br", parser_logic::nl2br},
            {"replace", parser_logic::replace},
            {"reverse", parser_logic::reverse},
            {"round", parser_logic::round},
        };

    for (auto &&fn_pair : fn_list){
        addFilterFunctions(fn_pair.first, fn_pair.second);
    }

    addFilterFunctions("country_name", [](
        nlohmann::json& dict, nlohmann::json params
    ){
        if(params.size() == 0){
            params[0] = "ru";
        }
        parser_logic::country_name(dict, params);
    });

    addFilterFunctions("currency_name", [](
        nlohmann::json& dict, nlohmann::json params
    ){
        if(params.size() == 0){
            params[0] = "ru";
        }
        parser_logic::currency_name(dict, params);
    });
};

void canto_temp::parser_logic::Variables::addFilterFunctions(
    std::string func_name, 
    std::function<void(nlohmann::json&, nlohmann::json)> func
){
    if(func_.find(func_name) == func_.end()){
        func_[func_name] = func;
    }else{
        // ToDo throw
        std::cout << "function exists" << std::endl;
    }
}


void canto_temp::parser_logic::Variables::scanBitOr(
    Token &token, nlohmann::json& object_value
){
    if(token.getCell() == Token::Cell::bit_ore){
        token.next();
        parser_logic::skipSpace(token);
    }
    
    std::string name_function = parser_logic::getWord(
        token, Token::Cell::id
    );
    
    if(!name_function.empty()){
        nlohmann::json params;
        if(token.getCell() == Token::Cell::left_paren){
            token.next();
            params = scanParams(token);
            parser_logic::skipSpace(token);
            if(token.getCell() == Token::Cell::instruction_close){
                // token.next();
            }
        }
        
        auto f = func_.find(name_function);
        if(f != func_.end()){
            f->second(object_value, params);
        }else{
            // ToDo throw
            
            // value.append(std::string(
            //     "\nWarning to Line: "
            //         + std::to_string(__LINE__) 
            //         + " Filter '"
            //         + name_function
            //         + "' is not exists\n"
            // ));
        }
    }
}


void canto_temp::parser_logic::Variables::scanOperators(
    Token &token, nlohmann::json& left_value
){
    Token::Cell cell = token.getCell();
    token.next();
    bool is_not = false;
    if (cell == Token::Cell::is_equal){
        parser_logic::skipSpace(token);
        std::string word = 
            parser_logic::getWord(token, Token::Cell::id);
        
        if(word == "not"){
            is_not = true;
            parser_logic::skipSpace(token);
            word = parser_logic::getWord(
                token, Token::Cell::id);
        }

        if(word == "defined"){
            left_value = !left_value.is_null();
        }else if(word == "empty"){
            left_value = parser_logic::isEmptyDicVar(left_value);
        }else{

        }
        
    }else if(cell == Token::Cell::not_equal
        || cell == Token::Cell::equal
    ){
        char comp = '=';
        if(cell == Token::Cell::not_equal){
            comp = '!';
            is_not = true;
        }
        parser_logic::skipSpace(token);
        nlohmann::json right_value = 
            scanVariable(token, (*obj_list_));

        if(pre_cell_ != Token::Cell::string 
            && right_value.is_string()
        ){
            if(right_value == "true"){
                left_value = !parser_logic::isEmptyDicVar(left_value);
            }else if(right_value == "false"){
                left_value = parser_logic::isEmptyDicVar(left_value);
            }else {
                left_value = parser_logic::compare(
                    left_value, right_value, comp
                );
            }
        }else{
            left_value = parser_logic::compare(
                left_value, right_value, comp
            );
        }
    }else if(cell == Token::Cell::in_equal){
        parser_logic::skipSpace(token);
        left_value = scanVariable(token, left_value);
    }

    if(is_not && left_value.is_boolean()){
        left_value = !left_value;
    }
}

std::string canto_temp::parser_logic::Variables::scanString(
    Token &token
){
    std::string ret_value{};
    Token::Cell cell = token.getCell();
    if(cell == Token::Cell::string){
        token.next();
    }
    while (token.getIterCount()){
        cell = token.getCell();
        if(cell == Token::Cell::string){
            token.next();
            break;
        }else if(token.isOperator()){
            if(cell == Token::Cell::is_equal
                || cell == Token::Cell::in_equal
            ){
                ret_value.append(1, token.getCurrent());
                ret_value.append(1, token.getNext());
            }else{
                ret_value.append(1, token.getCurrent());
            }
        }else{
            ret_value.append(1, token.getCurrent());
        }
        token.next();
    }
    return ret_value;
}

nlohmann::json canto_temp::parser_logic::Variables::scanParams(
    Token &token
){
    nlohmann::json ret_value{};
    int iter = 0;
    while (token.getIterCount()){
        Token::Cell cell = token.getCell();
        if(cell == Token::Cell::eof){
            break; 
        }else if(cell == Token::Cell::right_paren){
            token.next();
            break; 
        }else if(cell == Token::Cell::string){
            ret_value[iter] = scanString(token);
            iter++;
        }else if(cell == Token::Cell::id
            || cell == Token::Cell::number
        ){
            ret_value[iter] = scanVariable(token, (*obj_list_));
            iter++;
            continue;
        }else if(cell == Token::Cell::left_paren){
            token.next();
            ret_value[iter] = scanParams(token);
        }else if(cell == Token::Cell::comma){
            token.next();
            parser_logic::skipSpace(token);
            continue;
        }else if(
            cell == Token::Cell::instruction_close 
            || cell == Token::Cell::instruction_open 
            || cell == Token::Cell::var_close
        ){
            // ToDo errors
            break;
        }
        token.next();
    }
    return ret_value;
}

nlohmann::json canto_temp::parser_logic::Variables::scanNumeric(
    Token &token
){
    std::string ret_value{};
    bool is_float = false;
    while (token.getIterCount()){
        token.init();
        Token::Cell cell = token.getCell();
        if(cell == Token::Cell::number
            || cell == Token::Cell::dot
        ){
            if(cell == Token::Cell::dot){
                is_float = true;
            }
            ret_value.append(1, token.getCurrent());
        }else{
            // ToDo Error
            break;
        }
        token.next();
    }
    
    if(is_float){
        return std::atof(ret_value.c_str());
    }
    if(!empty(ret_value)){
        return std::atoi(ret_value.c_str());
    }
    return 0;
}

nlohmann::json canto_temp::parser_logic::Variables::scanLeftBracket(
    Token &token, nlohmann::json dict
){
    nlohmann::json ret_value;
    token.next();
    if(token.getCell() == Token::Cell::number){
        nlohmann::json num = scanNumeric(token);
        if(num.is_number_unsigned()){
            ret_value = dict[(unsigned)num];
        }else if(num.is_number_integer()){
            ret_value = dict[(int)num];
        }else if(num.is_number_float()){
            ret_value = dict[(float)num];
        }
    }else if(token.getCell() == Token::Cell::string){
        ret_value = dict[scanString(token)];
    }else if(token.getCell() == Token::Cell::id){
        ret_value = 
            dict[scanVariable(token, (*obj_list_))];
    }else{
        // ToDo error
    }
    return ret_value;
}

nlohmann::json canto_temp::parser_logic::Variables::scanVariable(
    Token &token, 
    nlohmann::json object_value, 
    Token::Cell before
){
    while (token.getIterCount()){
        Token::Cell cell = token.getCell();
        pre_cell_ = cell;
        if(
            (before != Token::Cell::unknown && before == cell)
            || cell == Token::Cell::eof
        ){
            break;
        }
        if(cell == Token::Cell::var_open){
            token.next();
            continue;
        }else if(cell == Token::Cell::left_bracket){
            object_value = scanLeftBracket(token, object_value);
        }else if(cell == Token::Cell::var_close){
            break;
        }else if(cell == Token::Cell::id){
            std::string word = canto_temp::parser_logic::getWord(
                token, Token::Cell::id
            );

            if(word == "true"){
                object_value = true;
            }else if(word == "false"){
                object_value = false;
            }else{
                object_value = object_value[word];
            }
            continue;
        }else if(token.isOperator()){
            scanOperators(token, object_value);
        }else if(cell == Token::Cell::string){
            if(pre_cell_ == Token::Cell::left_bracket){
                std::string key = scanString(token);
                object_value = object_value[key];
            }else{
                object_value = scanString(token);
            }
        }else if(cell == Token::Cell::number){
            object_value = scanNumeric(token);
            continue;
        }else if(cell == Token::Cell::bit_ore){
            scanBitOr(token, object_value);
            continue;
        }else if(cell == Token::Cell::comma){
            // ToDo token.getNext()
            break;
        }else if(cell == Token::Cell::instruction_close){
            // ToDo error
            break;
        }else if(cell != Token::Cell::unknown 
            && cell != Token::Cell::dot
        ){
            if(pre_cell_ == Token::Cell::var_open 
                && cell == Token::Cell::space
            ){
                token.next();
                continue;
            }else{
                break;
            }
        }
        if(
            token.getCell() != Token::Cell::right_paren
            && token.getCell() != Token::Cell::bit_ore
        ){
            token.next();
        }
    }
    
    return object_value;
}

nlohmann::json canto_temp::parser_logic::Variables::parseVariable(void){
    Token token(container_, iter_count_);
    nlohmann::json object_value = (*obj_list_);
    while (token.getIterCount()){
        Token::Cell cell = token.getCell();
        if(cell == Token::Cell::eof){
            break;
        }else if(cell == Token::Cell::var_close 
            || cell == Token::Cell::instruction_close
        ){
            break;
        }else if(cell == Token::Cell::bit_ore){
            scanBitOr(token, object_value);
            continue;
        }else if(token.isOperator()){
            scanOperators(token, object_value);
        }else if(cell != Token::Cell::space){
            if(cell == Token::Cell::ternar_then){
                if(parser_logic::getBoolDicVar(object_value)){
                    token.next();
                    parser_logic::skipSpace(token);
                    object_value = scanVariable(
                        token, (*obj_list_), Token::Cell::ternar_else
                    );
                }else{
                    canto_temp::parser_logic::skipBy(
                        token, Token::Cell::ternar_else
                    );
                    parser_logic::skipSpace(token);
                    object_value = scanVariable(
                        token, (*obj_list_), Token::Cell::ternar_else
                    );
                }
                break;
            }else if(cell == Token::Cell::null_operator){
                if(!parser_logic::getBoolDicVar(object_value)){
                    token.next();
                    parser_logic::skipSpace(token);
                    object_value = scanVariable(
                        token, (*obj_list_), Token::Cell::ternar_else
                    );
                }
                break;
            }else{
                object_value = scanVariable(token, object_value);
            }
        }else {
            token.next();
        }
    }
    return object_value;
}


std::string canto_temp::parser_logic::Variables::getVar(void){
    std::string ret_value{};
    nlohmann::json dict = parseVariable();
    std::ostringstream oss;
    oss << dict;
    ret_value = oss.str();
    if(dict.is_string() 
        && ret_value.size() > 2
        && ret_value[0] == '"'
        && ret_value[ret_value.size()-1] == '"'
    ){
        ret_value.erase(0, 1);
        ret_value.erase(ret_value.size() - 1);
    }
    return ret_value;
}

void canto_temp::parser_logic::Variables::setVar(
    std::string var_name, nlohmann::json new_var
){
    (*obj_list_)[var_name] = new_var;
}