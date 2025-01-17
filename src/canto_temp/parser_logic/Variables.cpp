#include "canto_temp/parser logic/Variables.hpp"

std::string canto_temp::parser_logic::Variables::render(
    std::size_t end_tag_pos
){
    std::string tag{}, content{};
    while ((int)end_tag_pos > container_->pos()){
        if(container_->current() == '}'){
            container_->next();
            break;
        }
        if(container_->current() != ' '){
            tag.append(1, container_->current());
        }
        // else if(!tag.empty()){
            
        //     std::cout<< container_->current() << std::endl;
        // }
        
        container_->next();
    }
    content = obj_list_->find(tag)->second.str();
    return content;
}