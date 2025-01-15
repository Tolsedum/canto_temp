#ifndef CONTAINER_FILE_HPP
#define CONTAINER_FILE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include "canto_temp/Container.hpp"

namespace canto_temp{
    class FileContainer : public Container{
        bool next_read_;
        std::ifstream file_p_;
        char current_, next_;
    public:
        FileContainer(std::string_view file_name);
        ~FileContainer(){file_p_.close();};
        bool isEnd() override;
        char current() override;
        char next() override;

        std::streamoff getPos(){return file_p_.tellg();};
    };
}

#endif // CONTAINER_FILE_HPP