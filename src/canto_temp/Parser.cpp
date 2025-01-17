#include "canto_temp/Parser.hpp"

canto_temp::Parser::Parser(
    std::string& output,
    ContentParser& contentParser
): output_(&output){
    contentInit(std::forward<ContentParser>(contentParser));
}

canto_temp::Parser::Parser(
    std::string&& output,
    ContentParser&& contentParser
) : output_(&output){
    contentInit(std::forward<ContentParser>(contentParser));
}

canto_temp::Parser::~Parser(){
    delete container_;
    delete var_controller_;
};

void canto_temp::Parser::contentInit(
    ContentParser&& contentParser
){
    if(contentParser.is_content){
        container_ = new ContainerStr(
            std::move(contentParser.getContent())
        );
    }else if(contentParser.is_file){
        container_ = new FileContainer(contentParser.getFileName());
    }
    var_controller_ = new parser_logic::Variables(&obj_list_, container_);
}

void canto_temp::Parser::assign(
    std::map<std::string, Object>& list
){
    for (auto &&obj : list){
        obj_list_.insert(std::pair<std::string, Object>
            (obj.first, obj.second));
    }
}

void canto_temp::Parser::assign(
    std::string& key, std::map<std::string, Object>& obj
){
    obj_list_.insert(std::pair<std::string, Object>(key, obj));
}

std::string canto_temp::Parser::render(
    std::map<std::string, Object> list
){
    assign(list);
    return render();
}

std::string canto_temp::Parser::readContent(char tag){
    container_->next();
    switch (tag){
        case '#': {// Comment
            std::size_t pos = container_->find(
                "#}", container_->pos()
            );
            if(pos != std::string::npos){
                container_->setPos(pos+1);
            }else{
                // ToDo throw
            }
            break;
        }
        case '{':{// Print or set variables
            
            std::size_t pos = container_->find(
                "}}", container_->pos()
            );
            if(pos != std::string::npos){
                ++pos;
                return var_controller_->render(pos);
            }else{
                // ToDo throw
            }
            break;
        } 
        case '%':{// Instructions
            std::size_t pos = container_->find("%}", container_->pos());
            if(pos != std::string::npos){
                // container_->setPos(pos+1);
            }else{
                // ToDo throw
            }
            break;
        }
        default: {
            break;
        }
    }
    return std::string();
}

std::string canto_temp::Parser::render(){
    while (!container_->isEnd()){
        char current = container_->current();
        if(current == '{'){
            char next = container_->next();
            if(next == '%' 
                || next == '{'
                || next == '#'
            ){
                output_->append(readContent(next));
            }else{
                output_->append(1, current);
                output_->append(1, next);
            }
        }else{
            output_->append(1, current);
        }
        container_->next();
    }
    output_->append(1, container_->current());
    return *output_;
}