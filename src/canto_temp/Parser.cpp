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

canto_temp::Parser::Parser(
    std::string& output,
    ContentSettings& contentSettings,
    std::shared_ptr<parser_logic::Extends> extend
) : output_(&output)
    , extends_(extend){
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
    if(extends_.use_count() == 0){
        extends_ = std::make_shared<parser_logic::Extends>(
            var_controller_
        );
    }
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
                if(input_instruction.empty() 
                    && instruction_name != "extends"
                ){
                    input_instruction = instruction_name;
                }
                not_skip_append = false;
                
                if(!instruction_name.empty() && breakLogic(
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
    // extends_->render((*output_));
    renderExtensions();
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
        if(instruction == "extends"){
            extendsInstruction(tag);
        }else if(instruction == "block"){
            blockInstruction(tag);
        }else if(instruction == "if"){
            ifInstruction(tag);
        }else if(instruction == "set"){
            setInstruction(tag);
        }else if(instruction == "include"){
            includeInstruction(tag);
        }else if(instruction == "for"){

        }else if(instruction == "foreach"){

        }
    }
}

void canto_temp::Parser::renderExtensions(){
    if(extends_->getRender()){
        if_exit_ = false;
        ContentSettings contentSettings;
        contentSettings.setFileName(extends_->getMainFileName());
        container_.contentInit(
            std::forward<ContentSettings>(contentSettings)
        );
        iter_count_ = container_.size() - container_.pos();
        extends_->setSaveBlocks(false);
        extends_->setRender(false);
        render();
        std::cout << (*output_) << std::endl;
    }
}

void canto_temp::Parser::extendsInstruction(Tag &tag){
    if(tag.getCell() == Tag::Cell::space){
        parser_logic::skipSpace(tag);
    }
    
    std::string extends_name_ = var_controller_->getVar();
    tag.next();
    if(extends_name_.empty()){
        // ToDo Throw Error
    }
    extends_->setMainFileName(extends_name_);
    extends_->setRender(true);
}

void canto_temp::Parser::blockInstruction(Tag &tag){
    if(tag.getCell() == Tag::Cell::space){
        parser_logic::skipSpace(tag);
    }
    
    std::string word = 
        parser_logic::getWord(tag, Tag::Cell::id);

    parser_logic::skipBy (
        tag, Tag::Cell::instruction_close
    );
    tag.next();
    if(extends_->getSaveBlocks()){
        extends_->setBlock(word, tag);
    }else{
        parser_logic::Block block = extends_->getBlock(word);
        std::string content{};
        if(block.getInsertParentContent()){
            for (auto &&block : extends_->getBlockContent(word, tag)){
                content.append(block.getContent());
            }
        }
        if(block.notEmpty()){
            content.append(block.getContent());
            auto dict = obj_list_;
            ContentSettings contentSettings;
            contentSettings.setContent(content);
            canto_temp::Parser parser(
                (*output_),
                contentSettings,
                extends_
            );
            parser.assign(dict);
            parser.render();
        }
    }
}

void canto_temp::Parser::ifInstruction(Tag &tag){
    std::string content{};
    int count_if = 0;
    bool skip = false;
    if(
        parser_logic::getBoolDicVar(var_controller_->parseVariable())
    ){
        render("if");
        skip = true;
    }
    
    if(!if_exit_){
        while (container_.isNotEnd()){
            Tag::Cell cell = tag.getCell();
            if(cell == Tag::Cell::instruction_open){
                tag.next();
                
                parser_logic::skipSpace(tag);
                std::string instruction_name = 
                    parser_logic::getWord(tag, Tag::Cell::id);
                parser_logic::skipSpace(tag);

                if(instruction_name == "endif"){
                    if(count_if == 0){
                        break;
                    }else
                        count_if--;
                }else if(instruction_name == "elseif"){
                    if(count_if == 0 && !skip){
                        if(parser_logic::getBoolDicVar(
                            var_controller_->parseVariable()
                        )){
                            render("if");
                            skip = true;
                        }
                    }
                }else if(instruction_name == "else"){
                    if(count_if == 0 && !skip){
                        if(tag.getCurrent() == '}'){
                            tag.next();
                        }
                        render("if");
                        break;
                    }
                }else if(instruction_name == "if"){
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