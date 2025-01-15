#include "canto_temp/FileContainer.hpp"

canto_temp::FileContainer::FileContainer(std::string_view file_name){
    file_p_.open(file_name.begin(), std::ios::binary);
    next_read_ = false;
    next();
}

bool canto_temp::FileContainer::isEnd(){
    return file_p_.is_open() && file_p_.eof();
}

char canto_temp::FileContainer::current(){
    return current_;
}

char canto_temp::FileContainer::next(){
    if(file_p_.is_open() && !file_p_.eof()){
        if(next_read_){
            current_ = next_;
        }else{
            file_p_.get(current_);
        }
        next_read_ = true;
        if(!file_p_.eof()){
            file_p_.get(next_);
        }
    }else{
        current_ = next_;
    }
    return current_;
}