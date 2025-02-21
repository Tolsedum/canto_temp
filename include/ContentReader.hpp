#ifndef CONTENT_READER
#define CONTENT_READER

#include "content_reader/ContainerStr.hpp"
#include "content_reader/FileContainer.hpp"

struct ContentSettings{
    bool is_file, is_content;

    ContentSettings(){
        is_file  = false;
        is_content = false;
    };
    ~ContentSettings(){};

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

class ContentReader{

    std::shared_ptr<content_reader::Container> container_;
    
public:
    ContentReader(){};
    ContentReader(
        ContentSettings& contentParser
    );
    ContentReader(
        ContentSettings&& contentParser
    );
    ~ContentReader(){};

    /**
     * For class constructor
     */
    void contentInit(ContentSettings&& contentSettings);

    bool isEnd();
    bool isNotEnd();
    char current();
    char next();
    int pos();
    void setPos(int);
    std::size_t find(std::string_view, std::size_t s = 0);
    std::size_t find(char, std::size_t s = 0);

    void print();
};

#endif // CONTENT_READER