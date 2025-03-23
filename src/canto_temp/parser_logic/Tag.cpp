#include "canto_temp/parser_logic/Tag.hpp"

void canto_temp::Tag::initTag(){
    if(current_ == '{'){
        next_ = container_->next();
        if(next_ == '{'){
            cell_ = Cell::var_open;
        }else if(next_ == '%'){
            cell_ = Cell::instruction_open;
        }else if(next_ == '#'){
            cell_ = Cell::comment_open;
        }else{
            cell_= Cell::text;
        }
    }else if(current_ == '}'){
        next_ = container_->next();
        if(next_ == '}'){
            cell_ = Cell::var_open;
        }else{
            cell_= Cell::text;
        }
    }else if(current_ == '%'){
        next_ = container_->next();
        if(next_ == '}'){
            cell_ = Cell::instruction_close;
        }else{
            cell_= Cell::text;
        }
    }else{
        cell_= Cell::text;
    }
}

char canto_temp::Tag::getNext(){
    return next_;
}

char canto_temp::Tag::getCurrent(){
    return current_;
}