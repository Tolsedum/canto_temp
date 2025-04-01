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
            not_equal,           // !=
            bit_not_equal,       // !
            greater_than,        // >
            greater_equal,       // >=
            less_than,           // <
            less_equal,          // <=
            space,               // space on board
            unknown,
            eof
        };
    protected:
        bool next_init_;
        char current_, next_;
        Cell cell_;
        std::shared_ptr<ContentReader> container_;
        
    public:
        Token(ContentReader &container){
            container_ = std::make_shared<ContentReader>(container);
            current_ = container_->current();
            next_init_ = false;
            initToken();
        };
        ~Token(){};
        char getNext(){return next_;};
        char getCurrent(){return current_;};
        Cell getCell(){return cell_;};
        void initToken();

        char next();
    };
}


#endif // TOKEN_HPP