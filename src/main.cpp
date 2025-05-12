#include <iostream>

#include "canto_temp/Parser.hpp"

#include "../tests/test_variables.cpp"

void testVar(){
    
    // std::vector<std::map<std::string, nlohmann::json>> xs;
    // xs.push_back({
    //     { "enable", false },
    //     { "url", "http://example.com" },
    //     { "value", "Example" },
    // });
    // xs.push_back({
    //     { "enable", false },
    //     { "url", "undefined" },
    //     { "value", std::vector<int>{1,2,3,4,5,6,7,8,9} },
    // });
    // xs.push_back({
    //     { "enable", true },
    //     { "url", "http://google.com" },
    //     { "value", "Google" },
    // });
    std::map<std::string, nlohmann::json> list_vars;
    list_vars["title"] = "Sample Site";
    // list_vars["column"] = std::vector<std::map<std::string, std::string>>{
    //     {{"fruit", "apple"}},
    //     {{"fruit", "orange"}},
    //     {{"fruit", "red"}},
    //     {{"fruit", "red"}}
    // };
    list_vars["column"] = std::vector<std::string>{
        "fruit", "apple", "red"
    };
    list_vars["column2"] = std::vector<int>{
        1, 3, 2
    };
    list_vars["replace"] = std::map<std::string, std::string>{
        {"%this%", "fruit"},
        {"%that%", "oranges"}
    };
    // list_vars["xs"] = xs;
    // std::map<std::string, nlohmann::json> par;
    // par["comments"] = "string";
    // par["comments1"] = "d";
    // list_vars["content"] = par;
    // list_vars["count"] = 1;

    ContentReader container;
    ContentSettings contentSettings;
    
    contentSettings.setContent
        ("{{ 42.55|round }}");

    container.contentInit(
        std::forward<ContentSettings>(contentSettings)
    );

    std::string output{};
    canto_temp::Parser parser(
        std::move(output), 
        std::move(contentSettings)
    );
    parser.assign(list_vars);
    parser.render();
    output.append(1, '\n');
    std::cout << output << std::endl;
}

void parserMony(){
    std::string file_name{"mony.html"};
    if(std::filesystem::exists(file_name)){
        std::ifstream file_p{file_name};
        // file_p.open(file_name.begin(), std::ios::binary);
        std::map<std::string, 
            std::vector<std::string>> mony{};
        int iter = 0;
        
        std::string code, ru, en;
        for (std::string line; std::getline(file_p, line); ){
            canto_temp::parser_logic::ltrim(line);
            canto_temp::parser_logic::rtrim(line);
            if(line.find("<td>") != std::string::npos){
                line = line.substr(4);
                line.erase(line.size()-5);
                if(iter == 1){
                    code = line;
                }else if(iter == 3){
                    ru = line;
                }else if(iter == 4){
                    en = line;
                    mony[code] = {en, ru};
                    // set_data = false;
                }
                iter++;
            }else if(line == "</tr>"){
                iter = 0;
            }
        }
        std::ofstream ru_file("currency/ru/currency_name.len", std::ios::trunc);
        std::ofstream en_file("currency/en/currency_name.len", std::ios::trunc);
        for (auto &&i : mony){
            std::cout << i.first ;
            for (auto &&iter : i.second){
                std::cout << " " <<  iter;
            }
            std::cout << std::endl;
            std::string write{"code: \""};
            write.append(i.first).append("\"\n");
            en_file << write << "len: " << "\""<< i.second[0] <<"\"\n\n";
            ru_file << write << "len: " << "\""<< i.second[1] <<"\"\n\n";
            
        }
        ru_file.close();
        en_file.close();
    }else{
        std::cout<< "not exist: " << file_name << std::endl;
    }
}

void parserLang(){
    std::string file_name{"ht.html"};
    if(std::filesystem::exists(file_name)){
        std::ifstream file_p{file_name};
        // file_p.open(file_name.begin(), std::ios::binary);
        bool next_data = false;
        bool set_data = false;
        std::map<std::string, 
            std::vector<std::string>> contry{};
        int iter = 0;
        
        std::string pre_line, code, ru, en;
        for (std::string line; std::getline(file_p, line); ){
            canto_temp::parser_logic::ltrim(line);
            canto_temp::parser_logic::rtrim(line);
            if(line == "<tr>"){
                set_data = true;
            }else if(line == "</tr>"){
                set_data = false;
                next_data = false;
                en.clear();
                ru.clear();
                code.clear();
                iter = 0;
            }
            if(set_data){
                if(pre_line == "<p>"){
                    next_data = true;
                }
                
                std::size_t pos = line.find('<');
                if(next_data && pos == std::string::npos){
                    if(iter == 0){
                        en = line;
                    }else if(iter == 1){
                        ru = line;
                    }else if(iter == 2){
                        code = line;
                        contry[code] = {en, ru};
                        set_data = false;
                    }
                    iter++;
                }
            }
            pre_line = line;
        }
        std::ofstream ru_file("countrys/ru/country_name.len", std::ios::trunc);
        std::ofstream en_file("countrys/en/country_name.len", std::ios::trunc);
        for (auto &&i : contry){
            std::cout << i.first ;
            for (auto &&iter : i.second){
                std::cout << " " <<  iter;
            }
            std::cout << std::endl;
            std::string write{"code: \""};
            write.append(i.first).append("\"\n");
            en_file << write << "len: " << "\""<< i.second[0] <<"\"\n\n";
            ru_file << write << "len: " << "\""<< i.second[1] <<"\"\n\n";
            
        }
        ru_file.close();
        en_file.close();
    }else{
        std::cout<< "not exist: " << file_name << std::endl;
    }
}


/** https://ruden.github.io/twig-docs-ru/templates.html
 * ToDo
 * Реализовать некоторые стандартные фильтры
 * Добавление фильтров клиента
 * Добавление инструкций клиента
 * Экранирование HTML
 * Вывод предупреждения для отладки
 * Создать throw объект для обработки ошибок
*/

/*
    V   abs
    V   batch
    V    capitalize
    V    column
        convert_encoding
    V    country_name
    V    currency_name
        currency_symbol
        data_uri
        date
        date_modify
    V    default
    V    escape
        filter
        find
    V    first
    V    format
        format_currency
        format_date
        format_datetime
        format_number
        format_time
        html_to_markdown
        inky_to_html
        inline_css
    V    join
    V    json_encode
    V    keys
        language_name
    V    last
    V    length
        locale_name
    V   lower
        map
        markdown_to_html
    V    merge
    V    nl2br
        number_format
        plural
        raw
        reduce
    V    replace
    V    reverse
    V    round
        shuffle
        singular
        slice
        slug
        sort
        spaceless
        split
        striptags
        timezone_name
        title
        trim
        u
    V   upper
        url_encode
*/

/**
 * Выполнил:
 * Добавлен тег for
 * Наследование шаблонов 
 * Перенести getBoolDicVar, isEmptyDicVar и compare в tools
 * Включение других шаблонов (include)
 * Тернарный оператор
 * Оператор нулевой коалесценции
 * Создание кастомных переменных
 * Создать токены (с набором символов) для чтения синтаксиса
 * Условные блоки
 * Фильтры
 * Вставка комментария
 * Доступ к переменным
 * Доступ к элементам массива
 * Доступ к vector
 */

int main(){ // int argc, char *argv[]
    std::string str{"&gt;  A 'quote' is <b>\"bold\"</b>   &frasl;"};
    std::cout <<
        canto_temp::parser_logic::entityParser(str)
    << std::endl;

    // testVar();
    // parserMony();
    // parserLang();
    // exit(0);
    // listTest();
    
    return 0;
    
}