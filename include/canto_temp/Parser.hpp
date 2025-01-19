#ifndef PARSER_HPP
#define PARSER_HPP

#include "canto_temp/parser logic/Variables.hpp"
#include "canto_temp/ContainerStr.hpp"
#include "canto_temp/FileContainer.hpp"

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
        std::shared_ptr<Container> container_;
        std::shared_ptr<parser_logic::Variables> var_controller_;

        std::map<std::string, Dictionary> obj_list_;
        
        void contentInit(ContentParser&& contentParser);
        std::string readContent(char tag);
        
        std::string readVariableBlock();
        
    public:
        Parser(
            std::string& output,
            ContentParser& contentParser
        );
        Parser(
            std::string&& output,
            ContentParser&& contentParser
        );
        ~Parser(){};

        void assign(std::map<std::string, Dictionary>&);
        void assign(std::string&, std::map<std::string, Dictionary>&);

        std::string render();
        std::string render(std::map<std::string, Dictionary>);

        void addFilterFunctions(
            std::string& func_name, std::function<void(std::string&)>
        );
    };
};

#endif // PARSER_HPP