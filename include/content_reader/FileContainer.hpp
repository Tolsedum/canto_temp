#ifndef CONTAINER_FILE_HPP
#define CONTAINER_FILE_HPP

#include <fstream>
#include <filesystem>
#include "content_reader/Container.hpp"

namespace content_reader{
    class FileContainer : public Container{
        bool next_read_;
        std::ifstream file_p_;
        char current_, next_;
        std::size_t file_size_;
    public:
        FileContainer(std::string_view file_name);
        ~FileContainer(){
            file_p_.close();
        };
        bool isEnd() override;
        bool isNotEnd() override;
        char current() override;
        char next() override;
        int pos() override;
        void setPos(int) override;
        std::size_t find(std::string_view str, std::size_t p = 0) override;
        std::size_t find(char c, std::size_t p = 0) override;

        std::streamoff getPos(){return file_p_.tellg();};

        void print() override;
    };
}

#endif // CONTAINER_FILE_HPP