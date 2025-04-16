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
    addFilterFunctions("upper", [](nlohmann::json& dict){
        if(dict.is_string()){
            std::string str = dict;
            std::transform(
                str.begin(), 
                str.end(), 
                str.begin(), 
                ::toupper
            );
            dict = str;
        }
    });
    addFilterFunctions("lower", [](nlohmann::json& dict){
        if(dict.is_string()){
            std::string str = dict;
            std::transform(
                str.begin(), 
                str.end(), 
                str.begin(), 
                ::tolower
            );
            dict = str;
        }
    });
};


void canto_temp::parser_logic::Variables::addFilterFunctions(
    std::string func_name, std::function<void(nlohmann::json&)> func
){
    if(func_.find(func_name) == func_.end()){
        func_[func_name] = func;
    }else{
        // ToDo throw
        std::cout << "function exists" << std::endl;
    }
}

// void canto_temp::parser_logic::Variables::skipSpace(Token& token){
//     while (token.getCell() == Token::Cell::space){
//         token.next();
//     }
// }

// void canto_temp::parser_logic::Variables::skipBy(
//     Token& token, Token::Cell by
// ){
//     while (
//         token.getCell() != Token::Cell::var_close
//         && token.getCell() != Token::Cell::instruction_close
//         && token.getCell() != by
//     ){
//         token.next();
//     }
//     token.next();
// }

std::string canto_temp::parser_logic::Variables::scanString(Token &token){
    std::string ret_value{};
    Token::Cell cell = token.getCell();
    if(cell == Token::Cell::string){
        token.next();
    }
    while (token.getIterCount()){
        cell = token.getCell();
        if(cell == Token::Cell::string
            || cell == Token::Cell::var_close 
            || cell == Token::Cell::instruction_close
        ){
            break;
        }else{
            ret_value.append(1, token.getCurrent());
        }
        token.next();
    }
    return ret_value;
}

nlohmann::json canto_temp::parser_logic::Variables::scanNumeric(Token &token){
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

std::string canto_temp::parser_logic::Variables::scanIdName(Token &token){
    std::string ret_value{};
    while (token.getIterCount()){
        token.init();
        Token::Cell cell = token.getCell();
        if(cell == Token::Cell::id
            || cell == Token::Cell::number
        ){
            ret_value.append(1, token.getCurrent());
        }else if(// Error
            cell == Token::Cell::string
            || cell == Token::Cell::comma
            || cell == Token::Cell::left_brace
            || cell == Token::Cell::unknown
        ){
            // ToDo Error
            break;
        }else{
            break;
        }
        token.next();
    }
    return ret_value;
}

nlohmann::json canto_temp::parser_logic::Variables::scanIdValue(
    Token &token, nlohmann::json dict
){
    nlohmann::json ret_value = dict;
    // ToDo Придумать как правильно считать арифметику
    // ToDo Проверять на наличие данных по ключу dict[data]
    while (token.getIterCount()){
        Token::Cell cell = token.getCell();
        pre_cell_ = cell;
        if(cell == Token::Cell::dot){
            
        }else if(cell == Token::Cell::number){
            if(pre_cell_ == Token::Cell::dot
                || pre_cell_ == Token::Cell::left_bracket
            ){
                // Если нет операций
                ret_value = ret_value[scanNumeric(token)];
            }else if(cell == Token::Cell::id){
                ret_value = scanIdValue(token, ret_value);
            }else{
                // ToDo 
            }
        }else if(cell == Token::Cell::id){
            std::string var_name = scanIdName(token);
            if(!var_name.empty()){
                ret_value = ret_value[var_name];
            }
            continue;
        }else if(cell == Token::Cell::left_bracket){
            token.next();
            if(token.getCell() == Token::Cell::number){
                ret_value = ret_value[scanNumeric(token)];
            }else if(token.getCell() == Token::Cell::string){
                std::string key = scanString(token);
                ret_value = ret_value[key];
            }else{
                // ToDo
            }
        }else if(cell == Token::Cell::string){
            if(pre_cell_ == Token::Cell::left_paren){
                // ToDo получить строку
                // value =  getString(token);
            }else{
                // if(!var_name.empty()){
                //     ret_value = ret_value[var_name];
                //     var_name.clear();
                // }
                ret_value = ret_value[scanString(token)];
            }
            

        }else if(cell == Token::Cell::left_paren){
            // ToDo Получить результат функции через рекурсию
            
        }else if(cell == Token::Cell::right_paren){
            // ToDo Вернуть получившийся результат из рекурсии
            
            // if(!var_name.empty()){
            //     ret_value = ret_value[var_name];
            //     var_name.clear();
            // }
            break;
        }else if(// Error
            cell == Token::Cell::comma
            || cell == Token::Cell::left_brace
            || cell == Token::Cell::unknown
        ){
            // ToDo Error
            break;
        }else{
            break;
        }
        token.next();
    }
    return ret_value;
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
        auto f = func_.find(name_function);
        if(f != func_.end()){
            f->second(object_value);
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
            left_value = isEmptyDicVar(left_value);
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
                left_value = !isEmptyDicVar(left_value);
            }else if(right_value == "false"){
                left_value = isEmptyDicVar(left_value);
            }else {
                left_value = compare(left_value, right_value, comp);
            }
        }else{
            left_value = compare(left_value, right_value, comp);
        }
    }

    if(is_not && left_value.is_boolean()){
        left_value = !left_value;
    }
}

nlohmann::json canto_temp::parser_logic::Variables::scanVariable(
    Token &token, 
    nlohmann::json object_value, 
    Token::Cell before
){
    while (token.getIterCount()){
        Token::Cell cell = token.getCell();
        pre_cell_ = cell;
        if(before != Token::Cell::unknown && before == cell){
            break;
        }
        if(cell == Token::Cell::var_open){
            token.next();
            continue;
        }else if(cell == Token::Cell::left_bracket){
            token.next();
            if(token.getCell() == Token::Cell::number){
                nlohmann::json num = scanNumeric(token);
                if(num.is_number_unsigned()){
                    object_value = object_value[(unsigned)num];
                }else if(num.is_number_integer()){
                    object_value = object_value[(int)num];
                }else if(num.is_number_float()){
                    object_value = object_value[(float)num];
                }
            }else if(token.getCell() == Token::Cell::string){
                object_value = object_value[scanString(token)];
            }else if(token.getCell() == Token::Cell::id){
                object_value = 
                    object_value[scanVariable(token, (*obj_list_))];
            }
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
        }else if(cell == Token::Cell::bit_ore){
            scanBitOr(token, object_value);
            continue;
        }else if(cell == Token::Cell::comma){
            // ToDo token.getNext()
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
        token.next();
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
                if(getBoolDicVar(object_value)){
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
                if(!getBoolDicVar(object_value)){
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

bool canto_temp::parser_logic::Variables::getBoolDicVar(
    nlohmann::json dic
){
    return !isEmptyDicVar(dic);
}

bool canto_temp::parser_logic::Variables::isEmptyDicVar(
    nlohmann::json &dict
){
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

bool canto_temp::parser_logic::Variables::compare(
    nlohmann::json dic, 
    nlohmann::json dic1,
    char comp
){
    if(comp == '='){
        return dic == dic1;
    }else if(comp == '!'){
        return dic != dic1;
    }
    return false;
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