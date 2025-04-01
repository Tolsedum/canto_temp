#include "canto_temp/parser_logic/Variables.hpp"

canto_temp::parser_logic::Variables::Variables(
    std::map<std::string, nlohmann::json> *obj_list,
    ContentReader &container
)
    : obj_list_(obj_list)
    , container_(container)
{
    count_read_tag = 0;
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

void canto_temp::parser_logic::Variables::skipSpace(Token& token){
    while (token.getCell() == Token::Cell::space){
        token.next();
    }
}

void canto_temp::parser_logic::Variables::skipBy(
    Token& token, Token::Cell by
){
    while (
        token.getCell() != Token::Cell::var_close
        && token.getCell() != Token::Cell::instruction_close
        && token.getCell() != by
    ){
        token.next();
    }
    token.next();
}

std::string canto_temp::parser_logic::Variables::scanString(Token &token){
    std::string ret_value{};
    int iter_count = container_.size() - container_.pos();
    while (iter_count){
        Token::Cell cell = token.getCell();
        if(cell == Token::Cell::unknown){
            // ToDo Error
            break;
        }else if(cell == Token::Cell::id){
            ret_value.append(1, token.getCurrent());
        }else if(cell != Token::Cell::string){
            break;
        }
        token.next();
        token.initToken();
        iter_count--;
        if(iter_count == 0){
            break;
        }
    }
    return ret_value;
}

int canto_temp::parser_logic::Variables::scanNumeric(Token &token){
    std::string ret_value{};
    int iter_count = container_.size() - container_.pos();
    bool is_float = false;
    while (iter_count){
        token.initToken();
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
        iter_count--;
        if(iter_count == 0){
            break;
        }
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
    int iter_count = container_.size() - container_.pos();
    while (iter_count){
        token.initToken();
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
        iter_count--;
        if(iter_count == 0){
            break;
        }
    }
    return ret_value;
}

nlohmann::json canto_temp::parser_logic::Variables::scanIdValue(
    Token &token, nlohmann::json dict
){
    nlohmann::json ret_value = dict;
    // std::string var_name{};
    int iter_count = container_.size() - container_.pos();
    bool left_paren = false;
    Token::Cell pre_cell = Token::Cell::unknown;
    // ToDo Придумать как правильно считать арифметику
    while (iter_count){
        
        Token::Cell cell = token.getCell();
        if(cell == Token::Cell::dot){
            pre_cell = cell;
        }else if(cell == Token::Cell::number){
            if(pre_cell == Token::Cell::dot
                || pre_cell == Token::Cell::left_bracket
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
            if(pre_cell == Token::Cell::left_paren){
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
            pre_cell = cell;
            left_paren = true;
        }else if(cell == Token::Cell::right_paren){
            // ToDo Вернуть получившийся результат из рекурсии
            pre_cell = cell;
            // if(!var_name.empty()){
            //     ret_value = ret_value[var_name];
            //     var_name.clear();
            // }
            left_paren = false;
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
        // token.initToken();
        iter_count--;
        if(iter_count == 0){
            break;
        }
    }
    std::cout << ret_value.dump() << std::endl;
    return ret_value;
}

nlohmann::json canto_temp::parser_logic::Variables::scanVariable(
    Token &token, 
    nlohmann::json dict, 
    Token::Cell after
){
    int iter_count = container_.size() - container_.pos();
    Token::Cell pre_cell = Token::Cell::unknown;
    nlohmann::json object_value = dict;
    
    while (iter_count){
        Token::Cell cell = token.getCell();
        if(after != Token::Cell::unknown && after == cell){
            break;
        }
        if(cell == Token::Cell::var_open){
            token.next();
            iter_count--;
            pre_cell = cell;
            continue;
        }else if(cell == Token::Cell::left_bracket){
            // ToDo 
        }else if(cell == Token::Cell::var_close){
            break;
        }
        // else if(cell == Token::Cell::ternar_then){
        //     if(getBoolDicVar(object_value)){
        //         token.next();
        //         skipSpace(token);
        //         object_value = scanVariable(
        //             token, dict, Token::Cell::ternar_else
        //         );
        //         std::cout << object_value.dump() << std::endl;
        //     }
        // }
        else if(cell == Token::Cell::id){
            object_value = scanIdValue(token, object_value);
            iter_count--;
            pre_cell = cell;
            continue;
        }else if(cell == Token::Cell::string){
            if(pre_cell == Token::Cell::left_bracket){
                std::string key = scanString(token);
                object_value = object_value[key];
            }else{
                object_value = scanString(token);
            }
        }else if(cell == Token::Cell::number){
            object_value = scanNumeric(token);
        }else if(cell == Token::Cell::bit_ore){
            token.next();
            std::string name_function = scanString(token);
            
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
            iter_count--;
            pre_cell = cell;
            continue;
        }else if(cell == Token::Cell::comma){
            // ToDo token.getNext()
        }else if(cell == Token::Cell::instruction_close){
            // ToDo error
            break;
        }else if(cell != Token::Cell::unknown 
            && cell != Token::Cell::dot
        ){
            if(pre_cell == Token::Cell::var_open 
                && cell == Token::Cell::space
            ){
                iter_count--;
                token.next();
                pre_cell = cell;
                continue;
            }else{
                break;
            }
        }
        pre_cell = cell;
        iter_count--;
        if(iter_count == 0){
            break;
        }
        token.next();
    }
    
    return object_value;
}

nlohmann::json canto_temp::parser_logic::Variables::parseVariable(void){
    Token token(container_);
    int iter_count = container_.size() - container_.pos();
    nlohmann::json object_value = (*obj_list_);
    while (iter_count){
        Token::Cell cell = token.getCell();
        if(cell == Token::Cell::var_close){
            break;
        }else if(cell != Token::Cell::space){
            if(cell == Token::Cell::ternar_then){
                if(getBoolDicVar(object_value)){
                    token.next();
                    skipSpace(token);
                    object_value = scanVariable(
                        token, (*obj_list_), Token::Cell::ternar_else
                    );
                }else{
                    skipBy(token, Token::Cell::ternar_else);
                    skipSpace(token);
                    object_value = scanVariable(
                        token, (*obj_list_), Token::Cell::ternar_else
                    );
                }
                break;
            }else if(cell == Token::Cell::null_operator){
                if(!getBoolDicVar(object_value)){
                    token.next();
                    skipSpace(token);
                    object_value = scanVariable(
                        token, (*obj_list_), Token::Cell::ternar_else
                    );
                    std::cout << object_value.dump() << std::endl;
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

nlohmann::json canto_temp::parser_logic::Variables::parseOperation(void){
    std::string ret_value{};
    int iter_count = container_.size() - container_.pos();
    
    Token::Cell last_value = Token::Cell::unknown;
    nlohmann::json object_value = (*obj_list_);

    Token token(container_);
    while (iter_count){
        Token::Cell cell = token.getCell();
        if(cell == Token::Cell::instruction_open){
            token.next();
            continue;
        }else if(cell == Token::Cell::instruction_close){
            break;
        }else if(cell == Token::Cell::id){
            std::string name_id = scanString(token);
            if(name_id == "is"){
                std::string is_function = 
                    name_id + parser_logic::getParams('%', container_);
                parser_logic::skipTo('}', container_);

                if(is_function == "is defined"){
                    object_value = !object_value.is_null();
                }else if(is_function == "is not empty"){
                    object_value = !isEmptyDicVar(object_value);
                }else if(is_function == "is empty"){
                    object_value = isEmptyDicVar(object_value);
                }
                break;
            }else{
                if(token.getCell() != Token::Cell::left_brace){
                    object_value = object_value[name_id];
                    object_value = scanVariable(token, object_value);
                }else{
                    // name_id
                }
            }
            continue;
        }else if(cell == Token::Cell::left_paren){
            // ToDo получить значение функции
        }else if(cell == Token::Cell::string){
            last_value = Token::Cell::string;
            scanString(token);
        }else if(cell != Token::Cell::number){
            
        }else if(cell != Token::Cell::comma){
            // ToDo token.getNext()
        }else if(cell == Token::Cell::instruction_close
            || cell ==  Token::Cell::bit_ore
        ){
            break;
        }else{
            // ToDo
        }
        
        iter_count--;
        if(iter_count == 0){
            break;
        }
        token.next();
    }
    

    return object_value;
}

nlohmann::json canto_temp::parser_logic::Variables::getDicVar(
    std::string var
){
    bool get_from_str = !var.empty();
    if(get_from_str){
        if(var == "true"){
            return true;
        }else if(var == "false"){
            return false;
        }else {
            var.append("%}");
        }
    }
    int iter = 0;
    auto fn = [](
        ContentReader &container_, std::string var = "", int &iter
    ){
        char ret_char = '\n';
        if(var.empty()){
            ret_char = container_.next();
        }else{
            if((int)var.size() >= iter){
                ret_char = var[iter];
            }
            iter++;
        }
        return ret_char;
    };
    std::string name_var{};
    nlohmann::json obj = (*obj_list_);
    char pre = '\n';
    while (container_.isNotEnd()){
        count_read_tag++;
        char c = fn(container_, var, iter);
        
        if(c == '|'){
            break;
        }else if(pre == '%' && c == '}'){
            break;
        }else if(c == '%'){
            char next = fn(container_, var, iter);
            if(next == '}'){
                if(!get_from_str){
                    parser_logic::skipTo('}', container_);
                }
                break;
            }
            // ToDo
        }else if(c == ' '){
            char next = fn(container_, var, iter);
            if(next == '%'){
                char next_next = fn(container_, var, iter);
                if(next_next == '}'){
                    parser_logic::skipTo('}', container_);
                    break;
                }
                // ToDo
            }else if((next == '!' || next == '=') 
                && !name_var.empty()
            ){

                char next_next = fn(container_, var, iter);
                if(next_next == '='){
                    fn(container_, var, iter);
                    std::string var_name = 
                        parser_logic::getParams('%', container_);
                    parser_logic::skipTo('}', container_);
                    if(var_name == "true"){
                        obj = !isEmptyDicVar(obj[name_var]);
                    }else if(var_name == "false"){
                        obj = isEmptyDicVar(obj[name_var]);
                    }else {
                        obj = compare(
                            obj[name_var], getDicVar(var_name), next
                        );
                    }
                    name_var.clear();
                    break;
                }else{
                    // ToDo
                }
            }else if(name_var == "not"){
                std::string var_name = 
                    parser_logic::getParams('%', container_);
                parser_logic::skipTo('}', container_);
                obj = getDicVar(var_name);
                obj = !isEmptyDicVar(obj);
                name_var.clear();
                break;
            }else{
                std::string word = 
                    parser_logic::getParams(' ', container_);
                if(word == "is"){
                    std::string is_function = 
                        next + parser_logic::getParams('%', container_);
                    parser_logic::skipTo('}', container_);

                    if(is_function == "is defined"){
                        obj = !obj[name_var].is_null();
                    }else if(is_function == "is not empty"){
                        obj = !isEmptyDicVar(obj[name_var]);
                    }
                    name_var.clear();
                    break;
                }else{
                    if(!name_var.empty()){
                        obj = obj[name_var];
                    }
                    obj = getDicVar(word);
                    name_var.clear();
                }
            }
        }else if(
            (c == '.' || c == '[' || c == ']')
        ){
            if(c == ']' && (pre != '\'' && pre != '"')){
                // ToDo try catch Может быть переменная
                int iter = std::atoi(name_var.c_str());
                obj = obj[iter];
            }else if(!name_var.empty()){
                obj = obj[name_var];
            }
            name_var.clear();
        }else if(c == '>' || c == '<' || c == '='){
            if(isNumeric(name_var)){
                float var = std::atof(name_var.c_str());
                if(c == '>'){
                    obj = obj > var;
                }else if(c == '<'){
                    obj = obj < var;
                }else if(c == '='){
                    obj = obj == var;
                }
            }else{
                obj = obj[name_var];
            }
        }else if(c != ' ' && c != '"' && c != '\''){
            name_var.append(1, c);
        }
        pre = c;
    }
    
    if(!name_var.empty()){
        obj = obj[name_var];
    }
    if(count_read_tag > 0){
        count_read_tag++;
    }
    return obj;
    
}
/*
    nlohmann::json canto_temp::parser_logic::Variables::getDicVar(
        std::string tag
    ){
        char pre = '\n';
        std::string name_var{};
        nlohmann::json obj = (*obj_list_);

        rtrim(tag);
        ltrim(tag);
        for (std::size_t i = 0; i < tag.size(); i++){
            count_read_tag = i;
            if(tag[i] == '|'){
                break;
            }else if(
                (tag[i] == '.' || tag[i] == '[' || tag[i] == ']')
            ){
                if(tag[i] == ']' && (pre != '\'' && pre != '"')){
                    // ToDo try catch Может быть переменная
                    int iter = std::atoi(name_var.c_str());
                    obj = obj[iter];
                }else if(!name_var.empty()){
                    obj = obj[name_var];
                }
                name_var.clear();
            }else if(tag[i] == '>' || tag[i] == '<' || tag[i] == '='){
                if(isNumeric(name_var)){
                    // ToDo Доделать
                }else{
                    obj = obj[name_var];
                }
                
            }else if(tag[i] != ' ' && tag[i] != '"' && tag[i] != '\''){
                name_var.append(1, tag[i]);
            }
            pre = tag[i];
        }

        if(!name_var.empty()){
            obj = obj[name_var];
        }
        
        
        
        if(count_read_tag > 0){
            count_read_tag++;
        }
        return obj;
    }

    nlohmann::json canto_temp::parser_logic::Variables::getDicVar(){
        std::string name_var{};
        nlohmann::json obj = (*obj_list_);
        char pre = '\n';
        while (container_.isNotEnd()){
            count_read_tag++;
            char c = container_.next();
            
            if(c == '|'){
                break;
            }else if(pre == '%' && c == '}'){
                break;
            }else if(c == '%'){
                char next = container_.next();
                if(next == '}'){
                    parser_logic::skipTo('}', container_);
                    break;
                }
                // ToDo
            }else if(c == ' '){
                char next = container_.next();
                if(next == '%'){
                    char next_next = container_.next();
                    if(next_next == '}'){
                        parser_logic::skipTo('}', container_);
                        break;
                    }
                    // ToDo
                }else if((next == '!' || next == '=') 
                    && !name_var.empty()
                ){
                    char next_next = container_.next();
                    if(next_next == '='){
                        container_.next();
                        std::string var_name = 
                            parser_logic::getParams('%', container_);
                        parser_logic::skipTo('}', container_);
                        if(var_name == "true"){
                            obj = !isEmptyDicVar(obj[name_var]);
                        }else if(var_name == "false"){
                            obj = isEmptyDicVar(obj[name_var]);
                        }else {
                            obj = compare(
                                obj[name_var], getDicVar(var_name), next
                            );
                        }
                        name_var.clear();
                        break;
                    }else{
                        // ToDo
                    }
                }else if(name_var == "not"){
                    std::string var_name = 
                        parser_logic::getParams('%', container_);
                    parser_logic::skipTo('}', container_);
                    obj = getDicVar(var_name);
                    obj = !isEmptyDicVar(obj);
                    name_var.clear();
                    break;
                }else{
                    std::string word = 
                        parser_logic::getParams(' ', container_);
                    if(word == "is"){
                        std::string is_function = 
                            next + parser_logic::getParams('%', container_);
                        parser_logic::skipTo('}', container_);

                        if(is_function == "is defined"){
                            obj = !obj[name_var].is_null();
                        }else if(is_function == "is not empty"){
                            obj = !isEmptyDicVar(obj[name_var]);
                        }
                        name_var.clear();
                        break;
                    }else{
                        if(!name_var.empty()){
                            obj = obj[name_var];
                        }
                        obj = getDicVar(word);
                        name_var.clear();
                    }
                }
            }else if(
                (c == '.' || c == '[' || c == ']')
            ){
                if(c == ']' && (pre != '\'' && pre != '"')){
                    // ToDo try catch Может быть переменная
                    int iter = std::atoi(name_var.c_str());
                    obj = obj[iter];
                }else if(!name_var.empty()){
                    obj = obj[name_var];
                }
                name_var.clear();
            }else if(c == '>' || c == '<' || c == '='){
                if(isNumeric(name_var)){
                    float var = std::atof(name_var.c_str());
                    if(c == '>'){
                        obj = obj > var;
                    }else if(c == '<'){
                        obj = obj < var;
                    }else if(c == '='){
                        obj = obj == var;
                    }
                }else{
                    obj = obj[name_var];
                }
            }else if(c != ' ' && c != '"' && c != '\''){
                name_var.append(1, c);
            }
            pre = c;
        }
        
        if(!name_var.empty()){
            obj = obj[name_var];
        }
        if(count_read_tag > 0){
            count_read_tag++;
        }
        return obj;
    }

*/


std::string canto_temp::parser_logic::Variables::getVar(
    std::string tag
){
    
    nlohmann::json dict = getDicVar(tag);
    if(count_read_tag > 0){
        std::string name_function{};
        for (std::size_t i = count_read_tag; i < tag.size(); i++){
            if(tag[i] != '|' && tag[i] != ' '){
                name_function.append(1, tag[i]);
            }
        }
        if(!name_function.empty()){
            auto f = func_.find(name_function);
            if(f != func_.end()){
                f->second(dict);
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
    // return std::string(dict);
    std::ostringstream oss;
    oss << dict;
    std::string ret_value(oss.str()); 
    if(dict.is_string()){
        ret_value.erase(0, 1);
        ret_value.erase(ret_value.size()-1);
    }
    return ret_value;
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



std::string canto_temp::parser_logic::Variables::getVar(){
    nlohmann::json dict = getDicVar();
    // ToDo
    // if(count_read_tag > 0){
    //     std::string name_function{};
    //     for (std::size_t i = count_read_tag; i < tag.size(); i++){
    //         if(tag[i] != '|' && tag[i] != ' '){
    //             name_function.append(1, tag[i]);
    //         }
    //     }
    //     if(!name_function.empty()){
    //         auto f = func_.find(name_function);
    //         if(f != func_.end()){
    //             f->second(dict);
    //         }else{
    //             // ToDo throw
                
    //             // value.append(std::string(
    //             //     "\nWarning to Line: "
    //             //         + std::to_string(__LINE__) 
    //             //         + " Filter '"
    //             //         + name_function
    //             //         + "' is not exists\n"
    //             // ));
    //         }
    //     }
    // }
    // std::string ret_value(dict);
    // if(ret_value.size() > 0
    //     && ret_value[0] == '"'
    //     && ret_value[ret_value.size()-1] == '"'
    // ){
    //     ret_value.erase(0, 1);
    //     ret_value.erase(ret_value.size()-1);
    // }
    // return ret_value;
    std::ostringstream oss;
    oss << dict;
    return oss.str();
}

void canto_temp::parser_logic::Variables::setVar(
    std::string var_name, nlohmann::json new_var
){
    (*obj_list_)[var_name] = new_var;
}