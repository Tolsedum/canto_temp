#include "canto_temp/Parser.hpp"

canto_temp::Parser::Parser(
    std::string& output,
    ContentParser& contentParser
): output_(&output){
    contentInit(contentParser);
}

canto_temp::Parser::Parser(
    std::string&& output,
    ContentParser& contentParser
) : output_(&output){
    contentInit(contentParser);
}

void canto_temp::Parser::contentInit(
    ContentParser& contentParser
){
    if(contentParser.is_content){
        // ContainerStr content(
        //     std::move(contentParser.getContent())
        // );
        container = new ContainerStr(
            std::move(contentParser.getContent())
        );
    }else if(contentParser.is_file){
        container = new FileContainer(contentParser.getFileName());
        // FileContainer content(contentParser.getFileName());
        // container = &content;
    }
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

std::string canto_temp::Parser::render(){
    // ToDo парсить

    while (!container->isEnd()){
        char current = container->current();
        if(current == '{'){
            if(container->next() == '%' || container->next() == '{'){
                // ToDo Считываем заголовок и содержимое блока
            }else{
                output_->append(1, current);
            }
        }else{
            output_->append(1, current);
        }
        container->next();
    }
    output_->append(1, container->current());
    return *output_;
}