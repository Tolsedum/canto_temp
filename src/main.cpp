#include <iostream>
#include "canto_temp/Parser.hpp"

#include "../tests/test_variables.cpp"

void testVar(){
    std::vector<std::map<std::string, nlohmann::json>> xs;
    xs.push_back({
        { "enable", false },
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

    ContentReader container;
    ContentSettings contentSettings;
    
    contentSettings.setContent("{{ true }}");
    container.contentInit(
        std::forward<ContentSettings>(contentSettings)
    ); 
    
    // canto_temp::parser_logic::Variables var(&list_vars, container);

    // std::cout << var.getVar() << std::endl;
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
        capitalize
        column
        convert_encoding
        country_name
        currency_name
        currency_symbol
        data_uri
        date
        date_modify
        default
        escape
        filter
        find
        first
        format
        format_currency
        format_date
        format_datetime
        format_number
        format_time
        html_to_markdown
        inky_to_html
        inline_css
        join
        json_encode
        keys
        language_name
        last
        length
        locale_name
    V   lower
        map
        markdown_to_html
        merge
        nl2br
        number_format
        plural
        raw
        reduce
        replace
        reverse
        round
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
    // testVar();
    // exit(0);
    listTest();
    
    return 0;
    
}