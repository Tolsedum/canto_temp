#ifndef CONTAINER_FILE_HPP
#define CONTAINER_FILE_HPP

#include <fstream>
#include <filesystem>
#include "canto_temp/Container.hpp"

namespace canto_temp{
    class FileContainer : public Container{
        bool next_read_;
        std::ifstream file_p_;
        char current_, next_;
        std::size_t file_size_;
    public:
        FileContainer(std::string_view file_name);
        ~FileContainer(){
            std::cout << "~FileContainer" << std::endl;
            file_p_.close();
        };
        bool isEnd() override;
        char current() override;
        char next() override;
        int pos() override;
        void setPos(int) override;
        std::size_t find(std::string_view str, std::size_t p = 0) override;
        std::size_t find(char c, std::size_t p = 0) override;

        std::streamoff getPos(){return file_p_.tellg();};

        void print() override {std::cout<< "begin_" << std::endl;};
    };
}

#endif // CONTAINER_FILE_HPP