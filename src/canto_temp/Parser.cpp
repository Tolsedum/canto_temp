#include "canto_temp/Parser.hpp"

/***************************** Public *****************************/
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
        &obj_list_
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

bool canto_temp::Parser::breakLogic(
    std::string& instruction_name, std::string& tag_name
){
    bool result = false;
    if("end" + tag_name == instruction_name){
        result = true;
    }else if(
        tag_name == "if"
        && (
            instruction_name == "else"
            || instruction_name == "elseif"
        )
    ){
        result = true;
    }
    
    return result;
}

void canto_temp::Parser::append(char c){
    output_->append(1, c);
}
void canto_temp::Parser::append(std::string str){
    output_->append(str);
}

void canto_temp::Parser::skipTo(char c){
    while (container_->isNotEnd()){
        container_->next();
        if(container_->current() == c){
            container_->next();
            break;
        }
    }
}

void canto_temp::Parser::print(){
    container_->print();
    std::cout << *output_ << std::endl;
}

void canto_temp::Parser::render(
    std::string input_instruction,
    std::vector<std::string> read_before_instruction
){
    bool not_skip_append = true;
    while (container_->isNotEnd()){
        try{
            char current = container_->current();
            if(current == '{' 
                // || current == '%'
            ){
                char next = container_->next();
                if(next == '%'){
                    container_->next();
                    std::string tag = getParams('%');
                    skipTo('}');
                    std::string instruction_name = 
                        eraseByNidle(tag, " ");
                    if(input_instruction.empty()){
                        input_instruction = instruction_name;
                    }
                    not_skip_append = false;
                    if(breakLogic(
                        instruction_name, input_instruction
                    )){
                        break;
                    }
                    append(readInstruction(
                        instruction_name, tag
                    ));
                }else if(next == '{' || next == '#'){
                    append(readTag(next));
                }else{
                    append(current);
                    append(next);
                }
            }else{
                append(current);
            }
        }catch(const std::string& e){
            append(e);
        }
        if(container_->isEnd()){
            not_skip_append = false;
            break;
        }
        if(not_skip_append)
            container_->next();
        not_skip_append = true;
    }
    if(not_skip_append){
        append(container_->current());
    }
    
}

void canto_temp::Parser::render(
    std::map<std::string, Dictionary> list
){
    assign(list);
    render();
}

void canto_temp::Parser::addFilterFunctions(
    std::string& func_name, std::function<void(Dictionary&)> f
){
    var_controller_->addFilterFunctions(func_name, f);
}

/***************************** END Public *****************************/

/***************************** Private *****************************/

std::string canto_temp::Parser::getParams(
    // std::size_t end_tag_pos,
    char end_params
){
    std::string tag{};
    while (container_->isNotEnd()){
        if(container_->current() == '{'){
            //! IS throw Error
            throw "Not valid signature";
        }
        if(container_->current() == '}' 
            || container_->current() == end_params // '%'
        ){
            break;
        }
        tag.append(1, container_->current());
        container_->next();
    }
    var_controller_->ltrim(tag);
    var_controller_->rtrim(tag);
    return tag;
}

std::string canto_temp::Parser::eraseByNidle(
    std::string& str, std::string nidle
){
    std::string first{str};
    std::size_t pos = str.find(nidle);
    if(pos != std::string::npos){
        first.erase(pos);
        str.erase(0, first.size()+1);
    }
    return first;
}

std::string canto_temp::Parser::readInstruction(
    std::string instuction, std::string data
){
    std::string value{};
    if(!instuction.empty()){
        if(instuction == "if"){
            value = ifInstruction(data);
        }else if(instuction == "for"){

        }else if(instuction == "foreach"){

        }else if(instuction == "extends"){

        }else if(instuction == "block"){

        }else if(instuction == "include"){

        }
    }
    return value;
}

std::string canto_temp::Parser::readTag(
    char tag
){
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
            std::string str = var_controller_->getVar(getParams('}'));
            container_->next();
            if(!str.empty()){
                return str;
            }
            break;
        } 
        // case '%':{// Instructions
        //     std::string tag = getParams('%');
        //     std::string str = readInstruction(tag);
        //     if(!str.empty()){
        //         return str;
        //     }
        //     break;
        // }
        default: {
            break;
        }
    }
    return std::string();
}

std::string canto_temp::Parser::ifInstruction(std::string data){
    std::string content{};
    int count_if = 0;
    bool skip = false;

    if(data.find(" ") != std::string::npos){
        std::string instruction = data;
        data = eraseByNidle(instruction, " ");
        if(data.find("is defined")){
            
        }else if(data.find("is not empty")){

        }
    }
    
    Dictionary value = var_controller_->getDicVar(data);

    if(value){
        render("if");
        skip = true;
    }
    while (container_->isNotEnd()){
        char current = container_->next();
        if(current == '{' 
            && container_->next() == '%'
        ){
            container_->next();
            std::string tag = getParams('%');
            skipTo('}');
            if(tag.find("endif") != std::string::npos){
                if(count_if == 0){
                    break;
                }else
                    count_if--;
            }else if(tag.find("elseif") != std::string::npos){
                if(count_if == 0 && !skip){
                    std::string instruction_name = 
                        eraseByNidle(tag, " ");
                    
                    Dictionary v = var_controller_->getDicVar(tag);
                    if(v){
                        render("if");
                        skip = true;
                    }
                }
            }else if(tag.find("else") != std::string::npos){
                if(count_if == 0 && !skip){
                    render("if");
                    skip = true;
                }
            }else if(tag.find("if") != std::string::npos){
                count_if ++;
            }
        }
    }
    return content;
}

/***************************** END Private *****************************/