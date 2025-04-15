#include "canto_temp/parser_logic/Token.hpp"

char canto_temp::Token::localNext(void){
    (*iter_count_)--;
    return container_->next();
}

char canto_temp::Token::next(void){
    if(next_init_){
        current_ = next_;
        next_init_ = false;
    }else{
        current_ = localNext();
    }
    init();
    return current_;
}

void canto_temp::Token::init(void){
    is_operator_ = false;
    if((*iter_count_) <= 0){
        cell_ = Cell::eof;
    }else if(((int)current_ >= 65 &&  (int)current_ <= 90)
      || ((int)current_ >= 97 &&  (int)current_ <= 122)
      || (int)current_ == 95
    ){
        if(current_ == 'i'){
            if(!next_init_){
                next_ = localNext();
            }
            if (next_ == 's') {
                cell_ = Cell::is_equal;
                is_operator_ = true;
            }else{
                next_init_ = true;
                cell_ = Cell::id;
            }
        }else{
            cell_ = Cell::id;
        }
    }else if(
        ((int)current_ >= 48 &&  (int)current_ <= 57)
    ){
        cell_ = Cell::number;
    }else{
      switch (current_) {
        case '+':{
            is_operator_ = true;
            cell_ = Cell::plus;
            break;
          }
        case '-':{
            is_operator_ = true;
            cell_ = Cell::minus;
            break;
          }
        case '*':{
            is_operator_ = true;
            cell_ = Cell::times;
            break;
          }
        case '/':{
            is_operator_ = true;
            cell_ = Cell::slash;
            break;
          }
        case '^':{
            is_operator_ = true;
            cell_ = Cell::power;
            break;
          }
        case '.':{
            cell_ = Cell::dot;
            break;
          }
        case ',':{
            cell_ = Cell::comma;
            break;
          }
        case ':':{
            cell_ = Cell::ternar_else;
            break;
          }
        case '?':{
            if(!next_init_){
                next_ = localNext();
            }
            if(next_ == '?'){
                cell_ = Cell::null_operator;
            }else{
                next_init_ = true;
                cell_ = Cell::ternar_then;
            }
            break;
        }
        case '(':{
            cell_ = Cell::left_paren;
            break;
          }
        case ')':{
            cell_ = Cell::right_paren;
            break;
          }
        case '[':{
            cell_ = Cell::left_bracket;
            break;
          }
        case ']':{
            cell_ = Cell::right_bracket;
            break;
          }
        case ' ':{
            cell_ = Cell::space;
            break;
        }
        case '{':{
            if(!next_init_){
                next_ = localNext();
            }
            if(next_ == '{'){
                cell_ = Cell::var_open;
            }else if(next_ == '%'){
              cell_ = Cell::instruction_open;
            }else{
                next_init_ = true;
                cell_ = Cell::left_brace;
            }
            break;
          }
        case '}':{
            if(!next_init_){
                next_ = localNext();
            }
            if(next_ == '}'){
                cell_ = Cell::var_close;
            }else{
                next_init_ = true;
                cell_= Cell::right_brace;
            }
            break;
          }
        case '%':{
            if(!next_init_){
                next_ = localNext();
            }
            if(next_ == '}'){
                cell_ = Cell::instruction_close;
            }else{
                is_operator_ = true;
                next_init_ = true;
                cell_ = Cell::percent;
            }
            break;
          }
        case '|':{
            if(!next_init_){
                next_ = localNext();
            }
            if(next_ == '|'){
                cell_ = Cell::logic_ore;
                is_operator_ = true;
            }else{
                next_init_ = true;
                cell_ = Cell::bit_ore;
            }
            break;
        }
        case '&':{
            if(!next_init_){
                next_ = localNext();
            }
            if(next_ == '&'){
                cell_ = Cell::logic_and;
                is_operator_ = true;
            }else{
                next_init_ = true;
                cell_ = Cell::bit_and;
            }
            break;
        }
        case '>':{
            is_operator_ = true;
            if(!next_init_){
                next_ = localNext();
            }
            if (next_ == '=') {
                cell_ = Cell::greater_equal;
            }else{
                next_init_ = true;
                cell_ = Cell::greater_than;
            }
            break;
        }
        case '<':{
            is_operator_ = true;
            if(!next_init_){
                next_ = localNext();
            }
            if (next_ == '=') {
                cell_ = Cell::less_equal;
            }else{
                next_init_ = true;
                cell_ = Cell::less_than;
            }
            break;
        }
        case '=':{
            is_operator_ = true;
            if(!next_init_){
                next_ = localNext();
            }
            if (next_ == '=') {
                cell_ = Cell::equal;
            }else{
                next_init_ = true;
                cell_ = Cell::assign;
            }
            break;
        }
        case '!':{
            is_operator_ = true;
            if(!next_init_){
                next_ = localNext();
            }
            if (next_ == '=') {
                cell_ = Cell::not_equal;
            }else{
                next_init_ = true;
                cell_ = Cell::bit_not_equal;
            }
            break;
        }
        case '\"':{
            cell_ = Cell::string;
            break;
        }
        // case '@':
        // case '$':{
        //     //   return scan_id();
        //     cell_ = Cell::unknown;
        //     break;
        // }
        default:{
          cell_ = Cell::unknown;
        } 
    }
  }
    
}