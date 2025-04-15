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
 * ! Написать о том что выполнил задачу - Тернарный оператор и Оператор нулевой коалесценции
 * ToDo
 * Наследование шаблонов 
 * Включение других шаблонов
 * Вывод предупреждения для отладки
 * Создать throw объект для обработки ошибок
 * Список стандартных фильтров
 * Переделать variables чтобы читало переменные одним заходом
*/

/**
 * Выполнил:
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