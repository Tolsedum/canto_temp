#include "canto_temp/FileContainer.hpp"

canto_temp::FileContainer::FileContainer(std::string_view file_name){
    file_p_.open(file_name.begin(), std::ios::binary);
    next_read_ = false;
    
    file_size_ = std::filesystem::file_size(file_name);
    
    next();
}

bool canto_temp::FileContainer::isEnd(){
    return file_p_.is_open() && file_p_.eof();
}

bool canto_temp::FileContainer::isNotEnd(){
    return !(file_p_.is_open() && file_p_.eof());
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

int canto_temp::FileContainer::pos(){
    return (int)file_p_.tellg()-2;
}

void canto_temp::FileContainer::setPos(int new_pos){
    file_p_.seekg(new_pos, std::ios_base::beg);
    file_p_.get(current_);
    file_p_.get(next_);
}

std::size_t canto_temp::FileContainer::find(
    std::string_view str, std::size_t p
){
    std::size_t pos_line = std::string::npos;
    if(p <= file_size_){
        int pos_t = pos();
        file_p_.seekg(p, std::ios_base::beg);
        if(!isEnd()){
            std::string tmp;
            while (std::getline(file_p_, tmp)){
                std::size_t p = tmp.find(str);
                if(p != std::string::npos){
                    pos_line = p;
                    break;
                }
                tmp.clear();
            }
        }
        file_p_.clear();
        file_p_.seekg(pos_t, std::ios_base::beg);
    }
    return pos_line;
}

std::size_t canto_temp::FileContainer::find(
    char c, std::size_t p
){
    std::size_t pos_line = std::string::npos;
    if(p <= file_size_){
        std::streampos pos_t = pos();
        file_p_.seekg(p, std::ios_base::beg);
        if(!isEnd()){
            next();
            while (!isEnd()){
                if(c == current()){
                    pos_line = pos();
                    break;
                }
                next();
            }
            if(pos_line == std::string::npos){
                if(c == current()){
                    pos_line = file_size_;
                }
            }
        }
        file_p_.clear();
        file_p_.seekg(pos_t, std::ios_base::beg);
    }
    return pos_line;
}