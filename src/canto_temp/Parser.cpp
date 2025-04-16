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
    iter_count_ = container_.size() - container_.pos();
    var_controller_ = std::make_shared<parser_logic::Variables>(
        &obj_list_, container_, iter_count_
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
    std::string& instruction_name, std::string& tag_name, Tag& tag
){
    bool result = false;
    if("end" + tag_name == instruction_name){
        result = true;
        if(tag_name == "if"){
            if_exit_ = true;
        }
        parser_logic::skipBy(
            tag, Tag::Cell::instruction_close
        );
        // parser_logic::skipTo('}', container_);
        // !
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
    
    Tag tag(container_, iter_count_);
    while (container_.isNotEnd()){
        try{
            Tag::Cell cell = tag.getCell();
            if(cell == Tag::Cell::eof){
                not_skip_append = false;
                break;
            }else if(cell == Tag::Cell::var_open){
                tag.next();
                std::string str = 
                    var_controller_->getVar();
                    tag.initCurrent();
                if(!str.empty()){
                    append(str);
                }
            }else if(cell == Tag::Cell::instruction_open){
                tag.next();
                parser_logic::skipSpace(tag);
                std::string instruction_name = parser_logic::getWord(
                    tag, Tag::Cell::id
                );
                if(input_instruction.empty()){
                    input_instruction = instruction_name;
                }
                not_skip_append = false;
                
                if(breakLogic(
                    instruction_name, input_instruction, tag
                )){
                    break;
                }
                tag.init();
                readInstruction(
                    instruction_name, tag
                );
                parser_logic::skipSpace(tag);
            }else if(cell == Tag::Cell::comment_open){
                parser_logic::skipBy (
                    tag, Tag::Cell::comment_close
                );
            }else if(cell == Tag::Cell::id
                || cell == Tag::Cell::space
                || cell == Tag::Cell::number
            ){
                append(tag.getCurrent());
            }else if(cell == Tag::Cell::unknown){
                // ToDo error
            }else if(
                cell == Tag::Cell::var_close
                || cell == Tag::Cell::instruction_close
                || cell == Tag::Cell::comment_close
            ){

            }
        }catch(const std::string& e){
            append(e);
        }
        
        if(container_.isEnd()){
            not_skip_append = false;
            break;
        }
        if(not_skip_append){
            tag.next();
        }
            
        not_skip_append = true;
    }
    if(not_skip_append){
        append(tag.getCurrent());
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
    std::string instruction, Tag &tag
){
    std::string value{};
    if(!instruction.empty()){
        if(instruction == "if"){
            ifInstruction(tag);
        }else if(instruction == "set"){
            setInstruction(tag);
        }else if(instruction == "include"){
            includeInstruction(tag);
        }else if(instruction == "for"){

        }else if(instruction == "foreach"){

        }else if(instruction == "extends"){

        }else if(instruction == "block"){

        }
    }
}

void canto_temp::Parser::ifInstruction(Tag &tag){
    std::string content{};
    int count_if = 0;
    bool skip = false;
    if(
        var_controller_->getBoolDicVar(var_controller_->parseVariable())
    ){
        render("if");
        skip = true;
    }
    
    if(!if_exit_){
        // Tag tag(container_);
        while (container_.isNotEnd()){
            Tag::Cell cell = tag.getCell();
            if(cell == Tag::Cell::instruction_open){
                tag.next();
                
                //! std::string str_tag = parser_logic::getParams(
                //     '%', container_
                // );
                parser_logic::skipSpace(tag);
                std::string instruction_name = 
                    parser_logic::getWord(tag, Tag::Cell::id);
                parser_logic::skipSpace(tag);
                // parser_logic::skipTo("%}", container_);

                if(instruction_name == "endif"
                    // str_tag.find("endif") != std::string::npos
                ){
                    if(count_if == 0){
                        break;
                    }else
                        count_if--;
                }else if(instruction_name == "elseif"
                    // str_tag.find("elseif") != std::string::npos
                ){
                    if(count_if == 0 && !skip){
                        // std::string instruction_name = 
                        //     eraseByNidle(str_tag, " ");
                        // parser_logic::skipTo(' ', container_);

                        if(var_controller_->getBoolDicVar(
                            var_controller_->parseVariable()
                            // var_controller_->getDicVar(str_tag)
                        )){
                            render("if");
                            skip = true;
                        }
                    }
                }else if(
                    instruction_name == "else"
                    // str_tag.find("else") != std::string::npos
                ){
                    if(count_if == 0 && !skip){
                        if(tag.getCurrent() == '}'){
                            tag.next();
                        }
                        render("if");
                        break;
                    }
                }else if(
                    instruction_name == "if"
                    // str_tag.find("if") != std::string::npos
                ){
                    count_if ++;
                }
            }else{
                tag.next();
            }
        }
        if(tag.getCurrent() == '}'){
            tag.next();
        }
    }
    if_exit_ = false;
}

void canto_temp::Parser::setInstruction(Tag &tag){
    parser_logic::skipSpace(tag);
    std::string name_var = 
        parser_logic::getWord(tag, Tag::Cell::id);
    parser_logic::skipSpace(tag);
    if(tag.getCurrent() == '='){
        tag.next();
    
        var_controller_->setVar(
            name_var, 
            var_controller_->parseVariable()
        );
    }else{
        // ToDo throw
    }
}

void canto_temp::Parser::includeInstruction(Tag &tag){
    parser_logic::skipSpace(tag);
    
    std::string file_name{};
    if(tag.getCurrent() == '"' || tag.getCurrent() == '\''){
        file_name = canto_temp::parser_logic::getWord(
            tag, Tag::Cell::id
        );
        file_name.erase(0, 1);
        file_name.erase(file_name.size()-1, 1);
    }else{
        file_name = 
            var_controller_->getVar();
    }
    
    if(std::filesystem::exists(file_name)){
        auto dict = obj_list_;
        ContentSettings contentSettings;
        contentSettings.setFileName(file_name);
        canto_temp::Parser parser(
            std::move((*output_)), 
            std::move(contentSettings)
        );
        parser.assign(dict);
        parser.render();
    }else {
        // ToDo error
    }
}

/***************************** END Private *****************************/