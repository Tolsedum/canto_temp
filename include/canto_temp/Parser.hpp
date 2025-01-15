#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <vector>
#include "canto_temp/Container.hpp"
#include "canto_temp/ContainerStr.hpp"
#include "canto_temp/FileContainer.hpp"
#include "canto_temp/Object.hpp"

namespace canto_temp{
    struct ContentParser{
        bool is_file, is_content;

        ContentParser(){
            is_file  = false;
            is_content = false;
        };
        ~ContentParser(){};

        void setFileName(const std::string& file_name){
            is_file = true;
            is_content = false;
            file_name_ = file_name;
            content_.clear();
        }
        void setContent(const std::string& content){
            is_file = false;
            is_content = true;
            content_ = content;
            file_name_.clear();
        }
        std::string& getContent(){
            return content_;
        }
        std::string& getFileName(){
            return file_name_;
        }
    private:
        std::string file_name_;
        std::string content_;
    };
    class Parser{
        std::string* output_;
        std::map<std::string, Object> obj_list_;
        Container* container;
        
        void contentInit(ContentParser& contentParser);
    public:
        Parser(
            std::string& output,
            ContentParser& contentParser
        );
        Parser(
            std::string&& output,
            ContentParser& contentParser
        );
        ~Parser(){delete container;};

        void assign(std::map<std::string, Object>&);
        void assign(std::string&, std::map<std::string, Object>&);

        std::string render();
        std::string render(std::map<std::string, Object>);
    };
};

#endif // PARSER_HPP