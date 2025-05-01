#ifndef PARSER_HPP
#define PARSER_HPP

#include "ContentReader.hpp"
#include "canto_temp/parser_logic/Extends.hpp"

namespace canto_temp{
    class Parser{
        bool if_exit_;
        int iter_count_;
        std::string* output_;
        ContentReader container_;
        std::shared_ptr<parser_logic::Variables> var_controller_;
        std::map<std::string, nlohmann::json> obj_list_;

        std::shared_ptr<parser_logic::Extends> extends_;
        
        void append(char c);
        void append(std::string str);

        void print();

        /**
         * For class constructor
         */
        void contentInit(ContentSettings&& contentSettings);
        /**
         * @brief Get instruction ( {%instruction%} )
         * @return String result
         */
        void readInstruction(
            std::string instuction, Tag &tag
        );
        /**
         * @brief Start read tag (readVar or readInstruction)
         * @param tag Name tag
         * @return String result
         */
        // std::string readTag(Tag tag);

        void stashComments();

        std::string readInstructionContent(
            std::string instruction_name, std::string tag
        );
        std::string eraseByNidle(
            std::string& str, std::string nidle
        );
        bool breakLogic(
            std::string& instruction_name, std::string& tag_name, Tag& tag
        );

        void forFunctionBody(std::string content);
        void renderExtensions();

        /********************* Instructions *********************/
        void extendsInstruction(Tag &tag);
        void blockInstruction(Tag &tag);
        void ifInstruction(Tag &tag);
        void setInstruction(Tag &tag);
        void includeInstruction(Tag &tag);
        void forInstruction(Tag &tag);
        /********************* END Instructions *********************/
    public:
        Parser(
            std::string& output,
            ContentSettings& contentSettings
        );
        Parser(
            std::string&& output,
            ContentSettings&& contentSettings
        );
        Parser(
            std::string& output,
            ContentSettings& contentSettings,
            std::shared_ptr<parser_logic::Extends> extend
        );
        ~Parser(){};

        void assign(std::map<std::string, nlohmann::json>&);
        void assign(std::string&, std::map<std::string, nlohmann::json>&);

        void render(
            std::string tag_name = ""
        );
        void render(
            std::map<std::string, nlohmann::json> data
        );

        void addFilterFunctions(
            std::string& func_name, 
            std::function<void(nlohmann::json&, nlohmann::json)>
        );
    };
};

#endif // PARSER_HPP