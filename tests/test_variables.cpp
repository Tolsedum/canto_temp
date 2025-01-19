#include <iostream>
#include "../canto_temp/Parser.hpp"
#include "../canto_temp/ContainerStr.hpp"
#include "../canto_temp/FileContainer.hpp"


void startParser(
    std::string& content, 
    std::string& output,
    std::map<std::string, canto_temp::Dictionary>& dict
){
    canto_temp::ContentParser contentParser;
    contentParser.setContent(content);

    canto_temp::Parser parser(
        std::move(output), 
        std::move(contentParser)
    );
    parser.assign(dict);
    parser.render();
    output.append(1, '\n');
}

void testComment(
    std::map<std::string, canto_temp::Dictionary>& dict
){
    std::string output;
    for (std::string content : {
        "{#\n Test \n#}<div>{{ xs[0].url }}</div>{#\n Test \n#}",
        "{#\n Test \n#}<div>{{ xs[1].url }}</div>{#\n Test \n#}",
        "{#\n Test \n#}<div>{{ xs[2].url }}</div>{#\n Test \n#}",
        "<div>{#\n {{ xs[2].url }} \n#}</div>",
        "<span>{#\n Very big comment string --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \n#}</span>",
    })
    {
        startParser(content, output, dict);
    }
    std::cout 
        <<"Test comments: \n" <<  output 
        << "------------------------------------------------------\n"
    << std::endl;
    output.clear();
}

void testInsertVars(
    std::map<std::string, canto_temp::Dictionary>& dict
){
    std::string output;
    for (std::string content : {
        "<div>test: {{ test }}</div>",
        "<div>{{ xs[0].url }}</div>",
        "<div>{{ xs[1].value[3] }}</div>",
        "<div>{{ title }}</div>"
    })
    {
        startParser(content, output, dict);
    }
    std::cout 
        <<"Test inserting variables: \n" <<  output 
        << "------------------------------------------------------\n"
    << std::endl;
    output.clear();
}

void testFilterVars(
    std::map<std::string, canto_temp::Dictionary>& dict
){
    std::string output;
    for (std::string content : {
        "<div>{{ xs[0].url|upper }}</div>",
        "<div>{{ xs[1].value[3] | upper }}</div>",
        "<div>{{ title  | upper }}</div>",
        "<div>{{ title  | upperf }}</div>"
    }){
        startParser(content, output, dict);
    }
    std::cout 
        <<"Test filters variables: \n" <<  output 
        << "------------------------------------------------------\n"
    << std::endl;
    output.clear();
}

void testIfElse(
    std::map<std::string, canto_temp::Dictionary>& dict
){
    std::string output;
    for (std::string content : {
        "{% if content.comments %} 1 {% endif %}",
        "{% if content.comments is not empty %} 2 {% endif %}",
        "{% if content.comments is defined %} 3 {% endif %}",
        "{% if count > 0 %} 4 {% endif %}"
    }){
        startParser(content, output, dict);
    }
    std::cout 
        <<"Test if else: \n" <<  output 
        << "------------------------------------------------------\n"
    << std::endl;
    output.clear();
}


void listTest(){
    std::vector<std::map<std::string, canto_temp::Dictionary>> xs;
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

    std::map<std::string, canto_temp::Dictionary> list_vars;
    list_vars["title"] = "Sample Site";
    list_vars["xs"] = xs;

    // testComment(list_vars);
    // testInsertVars(list_vars);
    // testFilterVars(list_vars);
    testIfElse(list_vars);
}