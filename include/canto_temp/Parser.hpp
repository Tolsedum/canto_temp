#ifndef PARSER_HPP
#define PARSER_HPP

#include "canto_temp/ContainerStr.hpp"
#include "canto_temp/FileContainer.hpp"
#include "canto_temp/parser_logic/Variables.hpp"
#include "canto_temp/Dictionary.hpp"

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
        // std::shared_ptr<parser_logic::Instructions> instructions_controller_;
        // std::shared_ptr<parser_logic::Reader> reader_;

        std::map<std::string, Dictionary> obj_list_;
        
        void append(char c);
        void append(std::string str);

        void skipTo(char c);

        void print();

        /**
         * For class constructor
         */
        void contentInit(ContentParser&& contentParser);
        /**
         * @brief Get instruction ( {%instruction%} )
         * @return String result
         */
        std::string readInstruction(
            std::string instuction, std::string data
        );
        /**
         * @brief Start read tag (readVar or readInstruction)
         * @param tag Name tag
         * @return String result
         */
        std::string readTag(char tag);

        /**
         * @brief Get params from tag
         * @param end_tag_pos 
         * @param end_params 
         * @return tag params
         */
        std::string getParams(
            char end_params
        );
        std::string readInstructionContent(
            std::string instruction_name, std::string tag
        );
        std::string eraseByNidle(
            std::string& str, std::string nidle
        );
        bool breakLogic(
            std::string& instruction_name, std::string& tag_name
        );

        /********************* Instructions *********************/
        std::string ifInstruction(std::string value);
        std::string forInstruction(Dictionary value);
        std::string foreachInstruction(Dictionary value);
        std::string extendsInstruction(Dictionary value);
        std::string blockInstruction(Dictionary value);
        std::string includeInstruction(Dictionary value);
        /********************* END Instructions *********************/
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

        void render(
            std::string tag_name = "",
            std::vector<std::string> read_before_instruction = {}
        );
        void render(
            std::map<std::string, Dictionary> data
        );

        void addFilterFunctions(
            std::string& func_name, std::function<void(Dictionary&)>
        );
    };
};

#endif // PARSER_HPP