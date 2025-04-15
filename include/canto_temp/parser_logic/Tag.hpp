#ifndef TAG_HPP
#define TAG_HPP

#include "ContentReader.hpp"

namespace canto_temp{
    class Tag{
    public:
        enum class Cell{
            id,
            number,
            var_open,            // {{
            var_close,           // }}
            instruction_open,    // {%
            instruction_close,   // %}
            comment_open,        // {#
            comment_close,       // #}
            space,
            unknown,
            eof
        };

    private:
        bool next_init_;
        char current_, next_;
        int *iter_count_;
        Cell cell_;
        std::shared_ptr<ContentReader> container_;
        
        char localNext(void);
    public:
        Tag(ContentReader &container, int &iter_count){
            container_ = std::make_shared<ContentReader>(container);
            current_ = container_->current();
            next_init_ = false;
            iter_count_ = &iter_count; //container.size() - container.pos()+1;
            init();
        };
        ~Tag(){};

        void initCurrent();
        void init();
        char next();
        char getNext();
        char getCurrent();
        int getIterCount(){return (*iter_count_);};
        Cell getCell(){return cell_;};

        void print(){container_->print();};
    };
}
#endif // TAG_HPP