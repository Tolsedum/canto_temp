#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <string>
#include "canto_temp/parser_logic/Tag.hpp"
#include "canto_temp/parser_logic/Variables.hpp"

namespace canto_temp{
namespace parser_logic{

    class Block{

        bool insert_parent_content_;
        std::string block_name_, content_;

    public:
        Block(
            std::string block_name,
            std::string content,
            bool insert_parent_content
        )
            : insert_parent_content_(insert_parent_content)
            , block_name_(block_name)
            , content_(content)
        {};
        Block(){};
        ~Block(){};

        bool notEmpty(){return !content_.empty();};
        bool getInsertParentContent(){return insert_parent_content_;};
        std::string getContent(){return content_;};
    };
}// parser_logic
}// canto_temp

#endif // BLOCK_HPP