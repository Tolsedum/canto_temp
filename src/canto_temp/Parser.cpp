#include "canto_temp/Parser.hpp"

/***************************** Public *****************************/
canto_temp::Parser::Parser(
    std::string& output,
    ContentSettings& contentSettings
): output_(&output){
    contentInit(std::forward<ContentSettings>(contentSettings));
}

canto_temp::Parser::Parser(
    std::string&& output,
    ContentSettings&& contentSettings
) : output_(&output){
    contentInit(std::forward<ContentSettings>(contentSettings));
}

void canto_temp::Parser::contentInit(
    ContentSettings&& contentSettings
){
    if_exit_ = false;
    container_.contentInit(
        std::forward<ContentSettings>(contentSettings)
    );
    var_controller_ = std::make_shared<parser_logic::Variables>(
        &obj_list_, container_
    );
}

void canto_temp::Parser::assign(
    std::map<std::string, nlohmann::json>& list
){
    for (auto &&obj : list){
        obj_list_.insert(std::pair<std::string, nlohmann::json>
            (obj.first, obj.second));
    }
}

void canto_temp::Parser::assign(
    std::string& key, std::map<std::string, nlohmann::json>& obj
){
    obj_list_.insert(std::pair<std::string, nlohmann::json>(key, obj));
}

bool canto_temp::Parser::breakLogic(
    std::string& instruction_name, std::string& tag_name
){
    bool result = false;
    if("end" + tag_name == instruction_name){
        result = true;
        if(tag_name == "if"){
            if_exit_ = true;
        }
        parser_logic::skipTo('}', container_);
    }else if(
        tag_name == "if"
        && (
            instruction_name == "else"
            || instruction_name == "elseif"
        )
    ){
        if_exit_ = false;
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



void canto_temp::Parser::print(){
    std::cout << std::endl 
        << "-----------print BEGIN-----------" << std::endl;
    container_.print();
    std::cout 
        << "----------------------" << std::endl
        << "output: "<< *output_ << std::endl
        << "-----------print END-----------" 
    << std::endl;
}


void canto_temp::Parser::render(
    std::string input_instruction
){
    bool not_skip_append = true;
    while (container_.isNotEnd()){
        try{
            Tag tag(container_);
            if(tag.getCell() == Tag::Cell::var_open){
                container_.next();
                std::string str = var_controller_->getVar(
                    parser_logic::getParams('}', container_)
                );
                container_.next();
                if(!str.empty()){
                    append(str);
                }
            }else if(tag.getCell() == Tag::Cell::instruction_open){
                parser_logic::skipTo(' ', container_);
                
                std::string instruction_name = 
                    parser_logic::getParams({' ', '%'}, container_
                );
                if(input_instruction.empty()){
                    input_instruction = instruction_name;
                }
                not_skip_append = false;
                
                if(breakLogic(
                    instruction_name, input_instruction
                )){
                    break;
                }
                readInstruction(
                    instruction_name
                );
            }else if(tag.getCell() == Tag::Cell::comment_open){
                parser_logic::skipTo("#}", container_);
            }else if(tag.getCell() == Tag::Cell::text){
                append(tag.getCurrent());
            }else if(
                tag.getCell() == Tag::Cell::var_close
                || tag.getCell() == Tag::Cell::instruction_close
                || tag.getCell() == Tag::Cell::comment_close
            ){

            }
        }catch(const std::string& e){
            append(e);
        }
        if(container_.isEnd()){
            not_skip_append = false;
            break;
        }
        if(not_skip_append)
            container_.next();
        not_skip_append = true;
    }
    if(not_skip_append){
        append(container_.current());
    }
    
}

void canto_temp::Parser::render(
    std::map<std::string, nlohmann::json> list
){
    assign(list);
    render();
}

void canto_temp::Parser::addFilterFunctions(
    std::string& func_name, std::function<void(nlohmann::json&)> f
){
    var_controller_->addFilterFunctions(func_name, f);
}

/***************************** END Public *****************************/

/***************************** Private *****************************/




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

void canto_temp::Parser::readInstruction(
    std::string instruction//, std::string data
){
    std::string value{};
    if(!instruction.empty()){
        if(instruction == "if"){
            ifInstruction();
        }else if(instruction == "set"){
            setInstruction();
        }else if(instruction == "for"){

        }else if(instruction == "foreach"){

        }else if(instruction == "extends"){

        }else if(instruction == "block"){

        }else if(instruction == "include"){

        }
    }
}

void canto_temp::Parser::ifInstruction(){
    std::string content{};
    int count_if = 0;
    bool skip = false;
    if(
        var_controller_->getBoolDicVar(var_controller_->getDicVar())
    ){
        render("if");
        skip = true;
    }
    
    if(!if_exit_){
        while (container_.isNotEnd()){
            Tag tag(container_);
            
            if(tag.getCell() == Tag::Cell::instruction_open){
                container_.next();
                std::string tag = parser_logic::getParams(
                    '%', container_
                );
                parser_logic::skipTo("%}", container_);

                if(tag.find("endif") != std::string::npos){
                    if(count_if == 0){
                        break;
                    }else
                        count_if--;
                }else if(tag.find("elseif") != std::string::npos){
                    if(count_if == 0 && !skip){
                        std::string instruction_name = 
                            eraseByNidle(tag, " ");
                        parser_logic::skipTo(' ', container_);

                        if(var_controller_->getBoolDicVar(
                            var_controller_->getDicVar(tag)
                        )){
                            render("if");
                            skip = true;
                        }
                    }
                }else if(tag.find("else") != std::string::npos){
                    if(count_if == 0 && !skip){
                        if(container_.current() == '}'){
                            container_.next();
                        }
                        render("if");
                        break;
                    }
                }else if(tag.find("if") != std::string::npos){
                    count_if ++;
                }
            }else{
                container_.next();
            }
        }
        if(container_.current() == '}'){
            container_.next();
        }
        
    }
    if_exit_ = false;
}

void canto_temp::Parser::setInstruction(){
    std::string name_var = parser_logic::getParams(
        '=', container_
    );
    container_.next();

    std::string var = parser_logic::getParams(
        '%', container_
    );
    
    if(var[0] == '{'){
        // ToDo set array
    }
    // else if(var == "true"){
    //     var_controller_->setVar(name_var, true);
    // }else if(var == "false"){
    //     var_controller_->setVar(name_var, false);
    // }
    else if(var.find('"') != std::string::npos){
        var_controller_->setVar(name_var, var);
    }
    else if(parser_logic::isNumeric(var)){
        if(var.find('.') != std::string::npos){
            var_controller_->setVar(name_var, std::atof(var.c_str()));
        }else{
            var_controller_->setVar(name_var, std::atoi(var.c_str()));
        }
    }
    else{
        var_controller_->setVar(
            name_var, var_controller_->getDicVar(var)
        );
    }
    parser_logic::skipTo('}', container_);
}

/***************************** END Private *****************************/