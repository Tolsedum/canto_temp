#include "canto_temp/parser_logic/Extends.hpp"

canto_temp::parser_logic::Block 
canto_temp::parser_logic::Extends::getBlock(
    std::string block_name
){
    auto iter = child_block_list_.find(block_name); 
    if(iter != child_block_list_.end()){
        return iter->second;
    }
    return Block();
}
std::vector<canto_temp::parser_logic::Block>
canto_temp::parser_logic::Extends::getBlockContent(
    std::string name_block, Tag &tag
){
    std::string content{};
    std::vector<parser_logic::Block> ret_value{};
    bool insert_parent_content = false;
    
    while (tag.getIterCount()){
        Tag::Cell cell = tag.getCell();
        if(cell == Tag::Cell::eof){
            break;
        }else if(cell == Tag::Cell::var_open){
            tag.next();
            parser_logic::skipSpace(tag);
            std::string word = 
                parser_logic::getWord(tag, Tag::Cell::id);
            if(word == "parent()"){
                insert_parent_content = true;
                parser_logic::skipBy(tag, Tag::Cell::var_close);
            }else{
                content.append(
                    "{{" 
                    + word 
                    + parser_logic::getStringByCell(
                        tag, Tag::Cell::var_close)
                    + "}}"
                );
            }
        }else if(cell == Tag::Cell::instruction_open){
            tag.next();
            parser_logic::skipSpace(tag);
            std::string word = 
                parser_logic::getWord(tag, Tag::Cell::id);
            if(word == "block"){
                parser_logic::skipSpace(tag);
                word = parser_logic::getWord(tag, Tag::Cell::id);
                parser_logic::skipBy (
                    tag, Tag::Cell::instruction_close
                );
                tag.next();
                for (auto &&bl : getBlockContent(word, tag)){
                    ret_value.push_back(bl);
                }
            }else if(word == "endblock"){
                parser_logic::skipBy(
                    tag, 
                    Tag::Cell::instruction_close
                );
                tag.next();
                break;
            }else{
                content.append(
                    "{%" 
                    + word 
                    + parser_logic::getStringByCell(
                        tag, Tag::Cell::instruction_close)
                    + "%}"
                );
            }
        }else{
            content.append(1, tag.getCurrent());
        }
        tag.next();
    }
    
    if(!empty(content)){
        ret_value.push_back(Block(
            name_block, content, insert_parent_content
        ));
    }
    return ret_value;
}

void canto_temp::parser_logic::Extends::setBlock(
    std::string name_block, Tag &tag
){
    if(save_blocks_){
        for (auto &&block : getBlockContent(name_block, tag)){
            if(block.notEmpty()){
                child_block_list_[name_block] = block;
            }
        }
    }
}

void canto_temp::parser_logic::Extends::render(std::string& output){
    std::cout << output << std::endl;
}