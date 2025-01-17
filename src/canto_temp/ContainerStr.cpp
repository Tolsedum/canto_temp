#include "canto_temp/ContainerStr.hpp"

void canto_temp::ContainerStr::constructor_(){
    begin_ = container_.begin();
    end_ = container_.end();
    is_end_ = begin_ == end_;
    pos_ = 0;
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
        pos_++;
        if(begin_ == end_){
            begin_--;
            is_end_ = true;
        }
    }
    return begin_[0];
}

int canto_temp::ContainerStr::pos(){
    return pos_;
}

void canto_temp::ContainerStr::setPos(int new_pos){
    if(pos_ != new_pos){
        begin_ = container_.begin() + new_pos;
        pos_ = new_pos;
    }
}

std::size_t canto_temp::ContainerStr::find(
    std::string_view str, std::size_t p
){
    return container_.find(str, p);
}

std::size_t canto_temp::ContainerStr::find(
    char c, std::size_t p
){
    return container_.find(c, p);
}