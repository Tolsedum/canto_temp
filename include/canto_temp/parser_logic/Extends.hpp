#ifndef EXTENDS_HPP
#define EXTENDS_HPP

#include <map>
#include "canto_temp/parser_logic/Block.hpp"
#include "canto_temp/parser_logic/tools.hpp"

namespace canto_temp{
namespace parser_logic{

    class Extends{
    protected:
        bool save_blocks_, render_;
        std::string name_main_template_;
        std::map<std::string, Block> child_block_list_;
        std::shared_ptr<parser_logic::Variables> var_controller_;
    public:
        Extends(
            std::shared_ptr<parser_logic::Variables> var_controller
        )
            : var_controller_(var_controller)
        {save_blocks_ = true; render_ = false;};
        ~Extends(){};

        void setMainFileName(std::string name_main_template)
            {name_main_template_ = name_main_template;};
        void setSaveBlocks(bool state){save_blocks_ = state;};
        void setRender(bool state){render_ = state;};
        void setBlock(
            std::string name_block,
            Tag &tag
        );
        void render(std::string& output);
        bool getRender(){return render_;};
        bool getSaveBlocks(){return save_blocks_;};
        std::string getMainFileName(){return name_main_template_;};
        std::vector<Block> getBlockContent(std::string name_block, Tag &tag);
        Block getBlock(std::string block_name);
    };
}// parser_logic
}// canto_temp

#endif // EXTENDS_HPP