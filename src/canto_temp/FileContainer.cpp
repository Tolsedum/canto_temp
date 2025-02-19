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
    return (int)file_p_.tellg()-1;
}

void canto_temp::FileContainer::setPos(int new_pos){
    if(file_size_ >= new_pos){
        file_p_.clear();
        file_p_.seekg(new_pos, std::ios::beg);
        file_p_.get(current_);
        file_p_.get(next_);
    }
}

void canto_temp::FileContainer::print(){
    int current_pos = pos()-1;
    std::string tmp;
    bool first = true;
    while (std::getline(file_p_, tmp)){
        if(first){
            first = false;
            std::cout<< current_ << next_ << tmp << std::endl;
        }else{
            std::cout<< tmp << std::endl;
        }
        
    }
    file_p_.clear();
    file_p_.seekg(current_pos, std::ios::beg);
    next_read_ = false;
    next();
}

std::size_t canto_temp::FileContainer::find(
    std::string_view str, std::size_t start_pos
){
    std::size_t pos_line = std::string::npos;
    if(start_pos <= file_size_){
        int pos_t = pos();
        if(start_pos > 1) --start_pos;
        file_p_.seekg(start_pos, std::ios::beg);
        if(!isEnd()){
            std::string tmp;
            while (std::getline(file_p_, tmp)){
                std::size_t p = tmp.find(str);
                if(p != std::string::npos){
                    std::size_t test_size = tmp.size() - p;
                    int pos_print = pos();
                    if(pos_print < 0){
                        pos_print = file_size_;
                    }
                    pos_line = pos_print - test_size;
                    break;
                }
                tmp.clear();
            }
        }
        file_p_.clear();
        file_p_.seekg(pos_t, std::ios::beg);
    }
    return pos_line;
}

std::size_t canto_temp::FileContainer::find(
    char c, std::size_t p
){
    std::size_t pos_line = std::string::npos;
    if(p <= file_size_){
        std::streampos pos_t = pos();
        if(p > 1) p = p - 1;
        file_p_.seekg(p, std::ios::beg);
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
        file_p_.seekg(pos_t, std::ios::beg);
    }
    return pos_line;
}