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

void canto_temp::Parser::contentInit(
    ContentParser&& contentParser
){
    if(contentParser.is_content){
        container_ = std::make_shared<ContainerStr>(
            std::move(contentParser.getContent())
        );
    }else if(contentParser.is_file){
        container_ = std::make_shared<FileContainer>(
            contentParser.getFileName()
        );
    }
    var_controller_ = std::make_shared<parser_logic::Variables>(
        &obj_list_, container_
    );
}

void canto_temp::Parser::assign(
    std::map<std::string, Dictionary>& list
){
    for (auto &&obj : list){
        obj_list_.insert(std::pair<std::string, Dictionary>
            (obj.first, obj.second));
    }
}

void canto_temp::Parser::assign(
    std::string& key, std::map<std::string, Dictionary>& obj
){
    obj_list_.insert(std::pair<std::string, Dictionary>(key, obj));
}

std::string canto_temp::Parser::render(
    std::map<std::string, Dictionary> list
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
        try{
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
        }catch(const std::string& e){
            // std::cerr << e.what() << '\n';
            output_->append(e);
        }
        if(container_->isEnd()){
            return *output_;
        }
        container_->next();
    }
    output_->append(1, container_->current());
    return *output_;
}

void canto_temp::Parser::addFilterFunctions(
    std::string& func_name, std::function<void(std::string&)> f
){
    var_controller_->addFilterFunctions(func_name, f);
}