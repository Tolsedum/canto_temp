#include "canto_temp/parser_logic/Tag.hpp"

char canto_temp::Tag::localNext(void){
    (*iter_count_)--;
    return container_->next();
}

char canto_temp::Tag::next(){
    if(next_init_){
        current_ = next_;
        next_init_ = false;
    }else{
        current_ = localNext();
    }
    init();
    return current_;
}

void canto_temp::Tag::initCurrent(){
    current_ = container_->current();
}

void canto_temp::Tag::init(){
    if((*iter_count_) <= 0){
        cell_ = Cell::eof;
    }else if(current_ == ' '){
        cell_ = Cell::space;
    }else  if(current_ == '{'){
        if(!next_init_){
            next_ = localNext();
        }
        if(next_ == '{'){
            cell_ = Cell::var_open;
        }else if(next_ == '%'){
            cell_ = Cell::instruction_open;
        }else if(next_ == '#'){
            cell_ = Cell::comment_open;
        }else{
            next_init_ = true;
            cell_= Cell::id;
        }
    }else if(current_ == '}'){
        if(!next_init_){
            next_ = localNext();
        }
        if(next_ == '}'){
            cell_ = Cell::var_close;
        }else{
            next_init_ = true;
            cell_= Cell::id;
        }
    }else if(current_ == '#'){
        if(!next_init_){
            next_ = localNext();
        }
        if(next_ == '}'){
            cell_ = Cell::var_close;
        }else{
            next_init_ = true;
            cell_= Cell::id;
        }
        cell_ = Cell::comment_close;
    }else if(current_ == '%'){
        if(!next_init_){
            next_ = localNext();
        }
        if(next_ == '}'){
            cell_ = Cell::instruction_close;
        }else{
            next_init_ = true;
            cell_= Cell::id;
        }
    }else if(
        ((int)current_ >= 48 &&  (int)current_ <= 57)
    ){
        cell_ = Cell::number;
    }else{
        cell_= Cell::id;
    }
}

char canto_temp::Tag::getNext(){
    return next_;
}

char canto_temp::Tag::getCurrent(){
    return current_;
}