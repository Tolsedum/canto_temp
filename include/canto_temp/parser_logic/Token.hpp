#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "ContentReader.hpp"

namespace canto_temp{
    
    class Token{
        enum class Cell{
            text,
            id,                 // this, this.foo
            number,             // 1, 2, -1, 5.2, -5.3
            string,             // "this"
            plus,                // +
            minus,               // -
            times,               // *
            slash,               // /
            percent,             // %
            power,               // ^
            comma,               // ,
            dot,                 // .
            colon,               // :
            left_paren,          // (
            right_paren,         // )
            left_bracket,        // [
            right_bracket,       // ]
            left_brace,          // {
            right_brace,         // }
            equal,               // ==
            not_equal,           // !=
            greater_than,        // >
            greater_equal,       // >=
            less_than,           // <
            less_equal,          // <=
            unknown,
            eof
        };
        
        Cell cell_;

        void initToken(char current, char next);
    public:
        Token(char current, char next){
            initToken(current, next);
        };
        ~Token(){};
        Cell getCell(){return cell_;};
    };
}


#endif // TOKEN_HPP