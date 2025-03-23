#ifndef TAG_HPP
#define TAG_HPP

#include "ContentReader.hpp"

namespace canto_temp{
    class Tag{
    public:
        enum class Cell{
            text,
            var_open,            // {{
            var_close,           // }}
            instruction_open,    // {%
            instruction_close,   // %}
            comment_open,        // {#
            comment_close,       // #}
        };

    private:
        char current_, next_;
        Cell cell_;
        std::shared_ptr<ContentReader> container_;
        void initTag();
    public:
        Tag(ContentReader &container){
            container_ = std::make_shared<ContentReader>(container);
            current_ = container_->current();
            initTag();
        };
        ~Tag(){};

        char getNext();
        char getCurrent();
        Cell getCell(){return cell_;};

        
    };
}
#endif // TAG_HPP