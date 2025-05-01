#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "ContentReader.hpp"

namespace canto_temp{
    
    class Token{
    public:
        enum class Cell{
            /**
             * Унарные операции:
             * ++; --; ~; !; -; +; &;
             *   Бинарные операции:
             *   *; /; %; +; -; <<; >>; <; <=; >; >=; ==; !=; &; ^; 
             *   |; &&; ||; =; *=; /=; %= +=; -=; <<=; >>=; &=; |=; ^=;
             *   Тернарные:
             *   ?; :;
             */
            // text,
            id,                  // this, this.foo
            instruction_open,    // {%
            instruction_close,   // %}
            logic_ore,           // ||
            logic_and,           // &&
            bit_ore,             // |
            bit_and,             // &
            number,              // 1, 2, -1, 5.2, -5.3
            string,              // "this"
            assign,              // =
            plus,                // +
            minus,               // -
            times,               // *
            slash,               // /
            percent,             // %
            power,               // ^
            comma,               // ,
            dot,                 // .
            ternar_else,         // :
            ternar_then,         // ?
            null_operator,       // ??
            left_paren,          // (
            right_paren,         // )
            left_bracket,        // [
            right_bracket,       // ]
            left_brace,          // {
            var_open,            // {{
            right_brace,         // }
            var_close,           // }}
            equal,               // ==
            is_equal,            // is 
            in_equal,            // in
            not_equal,           // !=
            bit_not_equal,       // !
            greater_than,        // >
            greater_equal,       // >=
            less_than,           // <
            less_equal,          // <=
            space,               // space on board
            bool_value,          // true, false
            unknown,
            eof
        };
    protected:
        bool next_init_, is_operator_;
        char current_, next_;
        int *iter_count_;
        Cell cell_;
        std::string word;
        std::shared_ptr<ContentReader> container_;
        
        char localNext(void);
    public:
        Token(ContentReader &container, int *iter_count){
            container_ = std::make_shared<ContentReader>(container);
            current_ = container_->current();
            next_init_ = false;
            is_operator_ = false;
            iter_count_ = iter_count;// container.size() - container.pos()+1;
            init();
        };
        ~Token(){};

        void init(void);
        bool isOperator(void){return is_operator_;};
        char getNext(void){return next_;};
        char getCurrent(void){return current_;};
        char next(void);
        int getIterCount(){return (*iter_count_);};
        Cell getCell(void){return cell_;};

        void print(){container_->print();};
    };
}


#endif // TOKEN_HPP