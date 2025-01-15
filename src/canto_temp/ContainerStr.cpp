#include "canto_temp/ContainerStr.hpp"

void canto_temp::ContainerStr::constructor_(){
    begin_ = container_.begin();
    end_ = container_.end();
    is_end_ = begin_ == end_;
}

bool canto_temp::ContainerStr::isEnd(){
    auto begin = begin_;
    return is_end_ || ++begin == end_;
};

char canto_temp::ContainerStr::current(){
    return begin_[0];
}

char canto_temp::ContainerStr::next(){
    if(!is_end_){
        begin_++;
        if(begin_ == end_){
            begin_--;
            is_end_ = true;
        }
    }
    return begin_[0];
}