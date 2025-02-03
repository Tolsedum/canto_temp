#include "canto_temp/parser_logic/Variables.hpp"

void canto_temp::parser_logic::Variables::addFilterFunctions(
    std::string func_name, std::function<void(Dictionary&)> func
){
    if(func_.find(func_name) == func_.end()){
        func_[func_name] = func;
    }else{
        // ToDo throw
        std::cout << "function exists" << std::endl;
    }
}


// trim from start (in place)
void canto_temp::parser_logic::Variables::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void canto_temp::parser_logic::Variables::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

canto_temp::Dictionary canto_temp::parser_logic::Variables::getDicVar(
    std::string tag
){
    rtrim(tag);
    ltrim(tag);

    Dictionary obj = (*obj_list_);
    if(tag.find('.') != std::string::npos 
        || tag.find('[') != std::string::npos 
        || tag.find('|') != std::string::npos 
    ){
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
    }else{
        return (*obj_list_)[tag];
    }
    return obj;
}

std::string canto_temp::parser_logic::Variables::getVar(
    std::string tag
){
    
    Dictionary dict = getDicVar(tag);
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
   
    return dict.toString();
}