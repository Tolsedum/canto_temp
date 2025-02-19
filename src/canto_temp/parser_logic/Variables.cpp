#include "canto_temp/parser_logic/Variables.hpp"

canto_temp::parser_logic::Variables::Variables(
    std::map<std::string, nlohmann::json> *obj_list,
    std::shared_ptr<Container> container
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

nlohmann::json canto_temp::parser_logic::Variables::getDicVar(
    std::string tag
){
    rtrim(tag);
    ltrim(tag);

    nlohmann::json obj = (*obj_list_);
    // ToDo Перенести в цикл
    // if(tag.find('.') != std::string::npos 
    //     || tag.find('[') != std::string::npos 
    //     || tag.find('|') != std::string::npos 
    //     || tag.find('>') != std::string::npos 
    //     || tag.find('<') != std::string::npos 
    //     || tag.find('=') != std::string::npos 
    // ){
        std::string name_var{};
        char pre;
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
                
                // if()
                // std::cout<< name_var << std::endl;
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
    // }else{
    //     return (*obj_list_)[tag];
    // }
    return obj;
}

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
    std::ostringstream oss;
    oss << dict;
    
    return oss.str();
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

nlohmann::json canto_temp::parser_logic::Variables::getDicVar(){
    std::string name_var{};
    nlohmann::json obj = (*obj_list_);
    char pre = '\n';
    while (container_->isNotEnd()){
        count_read_tag++;
        char c = container_->next();
        
        if(c == '|'){
            break;
        }else if(pre == '%' && c == '}'){
            break;
        }else if(c == '%'){
            char next = container_->next();
            if(next == '}'){
                parser_logic::skipTo('}', container_);
                break;
            }
            // ToDo
        }else if(c == ' '){
            char next = container_->next();
            if(next == '%'){
                char next_next = container_->next();
                if(next_next == '}'){
                    parser_logic::skipTo('}', container_);
                    break;
                }
                // ToDo
            }else if((next == '!' || next == '=') 
                && !name_var.empty()
            ){
                char next_next = container_->next();
                if(next_next == '='){
                    container_->next();
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
            
            // if(next == 'i' && container_->next() == 's'){
            //     std::string is_function = 
            //         next + parser_logic::getParams('%', container_);
            //     parser_logic::skipTo('}', container_);

            //     if(is_function == "is defined"){
            //         obj = !obj[name_var].is_null();
            //     }else if(is_function == "is not empty"){
            //         obj = isEmptyDicVar(obj[name_var]);
            //     }
            //     name_var.clear();
            //     break;
            // }else if(next){
            //     // ToDo
            // }
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
                // ToDo Доделать
            }else{
                obj = obj[name_var];
            }
            
            // if()
            // std::cout<< name_var << std::endl;
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
    std::ostringstream oss;
    oss << dict;
    return oss.str();
}

void canto_temp::parser_logic::Variables::setVar(
    std::string var_name, nlohmann::json new_var
){
    (*obj_list_)[var_name] = new_var;
}