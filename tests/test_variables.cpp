
#include "../canto_temp/Parser.hpp"
#include "../ContentReader.hpp"

void startStrParser(
    std::string& content, 
    std::string& output,
    std::map<std::string, nlohmann::json>& dict
){
    ContentSettings contentSettings;
    
    contentSettings.setContent(content);
    canto_temp::Parser parser(
        std::move(output), 
        std::move(contentSettings)
    );
    parser.assign(dict);
    parser.render();
    output.append(1, '\n');
}

void startParser(
    std::string& file_name, 
    std::string& output,
    std::map<std::string, nlohmann::json>& dict
){
    ContentSettings contentSettings;
    
    contentSettings.setFileName(file_name);

    canto_temp::Parser parser(
        std::move(output), 
        std::move(contentSettings)
    );
    parser.assign(dict);
    parser.render();
    output.append(1, '\n');
}

void pars(
    std::string path,
    std::map<std::string, nlohmann::json> list_vars
){
    std::cout << "********************** " 
        << path << " **********************" 
    << std::endl<< std::endl;
    int file_count = std::distance(
        std::filesystem::directory_iterator(path),
        std::filesystem::directory_iterator{}
    )-1;
    int file_iter = 1;
    std::vector<std::string> list_source_results;

    for (int i = 0; i < file_count; i++){
        std::string file_name = path + "/source"+std::to_string(file_iter)+".html";
        file_iter++;
        std::string output;
        startParser(file_name, output, list_vars);
        std::cout << file_name << std::endl;
        list_source_results.push_back(output);
    }

    std::string file_result = path + "/result.txt";
    std::ifstream i_file(file_result);
        if(i_file.is_open()){
            std::string result, tmp;
            std::vector<std::string> list_result;
            while (std::getline(i_file, tmp)){
                if(tmp != "##END"){
                    result.append(tmp);
                    result.append(1, '\n');
                }else{
                    result.erase(result.size()-1);
                    list_result.push_back(result);
                    result.clear();
                }
            }
            i_file.close();

            for (int i = 0; i < (int)list_result.size(); i++){

                std::cout << "result: ";
                for (auto &&e : list_source_results[i]){
                    std::cout << (int)e << " ";
                }
                std::cout << std::endl<< std::endl;
                
                std::cout << "test:   ";
                for (auto &&e : list_result[i]){
                    std::cout << (int)e << " ";
                }
                std::cout << (int)'\n' << std::endl<< std::endl;

                char res = 
                    (list_result[i] + "\n" == list_source_results[i])
                    ? 'V' : 'X';
                std::cout << 
                    i << ") file test source" << i+1 
                    << ".html " << res << std::endl;
                if(res == 'X'){
                    std::cout 
                        << list_result[i] 
                        << std::endl
                        << list_source_results[i]
                    << std::endl;
                }
            }
        }
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
    
    std::vector<std::string> loop{
        // "tests/current",
        "tests/extends",
        // "tests/comments",
        // "tests/getting_var",
        // "tests/filter_var",
        // "tests/if_else",
        // "tests/instructions",
    };

    for (auto path : loop){
        pars(path, list_vars);
    }
}