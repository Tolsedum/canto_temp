#include <iostream>
#include "../canto_temp/Parser.hpp"
#include "../canto_temp/ContainerStr.hpp"
#include "../canto_temp/FileContainer.hpp"


void startParser(
    std::string& file_name, 
    std::string& output,
    std::map<std::string, nlohmann::json>& dict
){
    canto_temp::ContentParser contentParser;
    
    // std::ifstream ifile(file_name);
    // std::string inp, tmp;
    // while (std::getline(ifile, tmp)){
    //     inp.append(tmp);
    //     inp.append(1, '\n');
    // }
    // contentParser.setContent(inp);
    contentParser.setFileName(file_name);

    canto_temp::Parser parser(
        std::move(output), 
        std::move(contentParser)
    );
    parser.assign(dict);
    parser.render();
    output.append(1, '\n');
}

void listTest(){
    std::vector<std::map<std::string, nlohmann::json>> xs;
    xs.push_back({
        { "enable", true },
        { "url", "http://example.com" },
        { "value", "Example" },
    });
    xs.push_back({
        { "enable", false },
        { "url", "undefined" },
        { "value", std::vector<int>{1,2,3,4,5,6,7,8,9} },
    });
    xs.push_back({
        { "enable", true },
        { "url", "http://google.com" },
        { "value", "Google" },
    });

    std::map<std::string, nlohmann::json> list_vars;
    list_vars["title"] = "Sample Site";
    list_vars["xs"] = xs;
    std::map<std::string, nlohmann::json> par;
    par["comments"] = "string";
    par["comments1"] = "d";
    list_vars["content"] = par;
    list_vars["count"] = 1;
    
    for (std::string file_name : {
        // "tests/test_comments.html",
        // "tests/test_getting_var.html",
        // "tests/test_filter_var.html",
        // "tests/test_if_else.html"
        "tests/test_set_instruction.html"
    }){
        std::string output;
        startParser(file_name, output, list_vars);
        std::cout 
            <<"Test " + file_name + ": \n" <<  output 
            << "------------------------------------------------------\n"
        << std::endl;
        output.clear();
    }
}