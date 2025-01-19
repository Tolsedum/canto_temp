#include "canto_temp/parser logic/Variables.hpp"

std::string canto_temp::parser_logic::Variables::eraseByNidle(
    std::string& str, char nidle
){
    std::size_t pos = str.find(nidle);
    std::string first{str};
    first.erase(pos);
    str.erase(0, first.size()+1);
    return first;
}

void canto_temp::parser_logic::Variables::addFilterFunctions(
    std::string func_name, std::function<void(std::string&)> func
){
    if(func_.find(func_name) == func_.end()){
        func_[func_name] = func;
    }else{
        // ToDo throw
        std::cout << "function exists" << std::endl;
    }
}

std::string canto_temp::parser_logic::Variables::getVar(
    std::string tag
){
    // std::vector<std::string> tags;
    std::string value{};
    
    if(tag.find('.') != std::string::npos 
        || tag.find('[') != std::string::npos 
        || tag.find('|') != std::string::npos 
    ){
        std::string name_var{}, name_function{};
        std::string* tmp = &name_var;
        bool set_name_function = false;
        Dictionary obj = (*obj_list_);
        char pre;
        for (std::size_t i = 0; i < tag.size(); i++){
            if(set_name_function){
                tmp = &name_function;
            }
            if(tag[i] == '|'){
                set_name_function = true;
            }else if(
                !set_name_function 
                && (tag[i] == '.' || tag[i] == '[' || tag[i] == ']')
            ){
                if(tag[i] == ']' && (pre != '\'' && pre != '"')){
                    int iter = std::atoi(name_var.c_str());
                    obj = obj[iter];
                }else if(!name_var.empty()){
                    obj = obj[name_var];
                }
                name_var.clear();
            }else if(tag[i] != ' ' && tag[i] != '"' && tag[i] != '\''){
                tmp->append(1, tag[i]);
            }
            pre = tag[i];
        }
        
        if(!name_var.empty()){
            obj = obj[name_var];
        }
        value = obj.toString();
        if(!name_function.empty()){
            auto f = func_.find(name_function);
            if(f != func_.end()){
                f->second(value);
            }else{
                // ToDo throw
                
                value.append(std::string(
                    "\nWarning to Line: "
                        + std::to_string(__LINE__) 
                        + " Filter '"
                        + name_function
                        + "' is not exists\n"
                ));
            }
        }
    }else{
        auto v = (*obj_list_)[tag];
        value = v.toString();
    }
    return value;
}

std::string canto_temp::parser_logic::Variables::render(
    std::size_t end_tag_pos
){
    std::string tag{}, content{};
    if(std::shared_ptr<Container> spt = container_.lock()){
        while ((int)end_tag_pos > spt->pos()){
            if(spt->current() == '}'){
                spt->next();
                break;
            }
            if(spt->current() != ' '){
                tag.append(1, spt->current());
            }
            spt->next();
        }
    }
    
    content = getVar(tag);
    return content;
}