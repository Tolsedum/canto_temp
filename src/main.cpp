#include <iostream>
#include "canto_temp/Parser.hpp"
#include "canto_temp/ContainerStr.hpp"
#include "canto_temp/FileContainer.hpp"

#include "../tests/test_variables.cpp"

void exist(canto_temp::Container * con){
    std::size_t pos = con->find("#}");
    con->setPos(pos);
    std::cout
        << "pos " << con->pos() 
        << " current: " << con->current()
    << std::endl;

    if(pos != std::string::npos){
        std::cout
            << "found on pos " << pos 
        << std::endl;
    }else{
        std::cout 
            << "Not found on pos " << pos
        << std::endl;
    }
}

void print(canto_temp::Container* container_){
    std::cout << std::endl 
        << "-----------print BEGIN-----------" << std::endl;
    container_->print();
    std::cout 
        << "-----------print END-----------" 
    << std::endl;
}

void skipTo(char c, canto_temp::Container* container_){
    // ToDo Пропускает символ
    while (container_->isNotEnd()){
        // char test = container_->current();
        if(container_->current() == c){
            container_->next();
            break;
        }
        container_->next();
    }
}

void testContainer(){
    canto_temp::FileContainer con_f("tests/test_if_else.html");
    // exist(&con_f);
    skipTo('#', &con_f);
    print(&con_f);

    std::string str =
        std::string("<!DOCTYPE html>/n") +
        std::string("{#<div>{% if content.comments1 is not empty %} content.comments1 is not empty {% endif %}</div>#}/n") +
        std::string("<div>{% if content.comments is defined %} content.comments is defined {% endif %}</div>\n") +
        std::string("<div>{% if content.comments is not empty %} comments is not empty {% elseif content.comments %} content.comments1 {% else %} else {% endif %}</div>\n") +
        std::string("<div>{% if content.comments %} 1 {% if content.comments %} 2 {% if content.comments %} <div>{{ title }}</div> {% endif %}{% endif %}{% endif %}</div>\n") +
        std::string("<div>{% if content.comments %} comments {% elseif content.comments1 %} {% if content.comments1 %} comments1 {% else %} ---------------- {% endif %} {% else %} else {% endif %}</div>\n") +
        std::string("<div>{% if content.comments is not empty %} 2 {% endif %}</div>\n") +
        std::string("<div>{% if count > 0 %} 4 {% endif %}</div>")
    ;
    canto_temp::ContainerStr con(str);
    skipTo('#', &con);
    print(&con);
    // exist(&con);
    
    // std::string str{"Hello world!"};
    // canto_temp::ContainerStr con(str);
    // int second = 0;
    // while (!con.isEnd()){
    //     std::cout<< con.current();
    //     con.next();
    //     second++;
    // }
    // std::cout<< con.current() << std::endl;


    // canto_temp::FileContainer con_1("test");
    // second = 0;
    // while (!con_1.isEnd()){
    //     std::cout<< con_1.current();
    //     con_1.next();
    //     second++;
    // }
    // std::cout<< con_1.current() << std::endl;
}

void testParser(){
    std::map<std::string, nlohmann::json> xs{
        { "enable", true },
        { "url", "http://example.com" },
        // { "value", "Example" },
        { "value", std::vector<int>{1, 2} },
        { "sss", 15.15 },
    };
    
    std::map<std::string, nlohmann::json> t;
    t["title"] = "Sample Site";
    t["xs"] = xs;

    std::string temp{"{#\n Test \n#}<div>{{ xs.value.[0] }}</div>"};
    std::string output;

    canto_temp::ContentParser contentParser;
    contentParser.setContent(temp);

    canto_temp::Parser parser(
        std::move(output), 
        std::move(contentParser)
    );
    parser.assign(t);
    
    // parser.render();
    std::cout << output << std::endl;
}


void testJson(){
    nlohmann::json jest = {""};
    // std::string v = jest;
    std::cout<< "jest " <<  jest.empty() << std::endl;
}

void testFind(){
    std::size_t file_b, str_b;

    canto_temp::FileContainer con_f("tests/test_if_else.html");
    file_b = con_f.find('#');

    std::string str =
        std::string("<!DOCTYPE html>/n") +
        std::string("{#<div>{% if content.comments1 is not empty %} content.comments1 is not empty {% endif %}</div>#}/n") +
        std::string("<div>{% if content.comments is defined %} content.comments is defined {% endif %}</div>\n") +
        std::string("<div>{% if content.comments is not empty %} comments is not empty {% elseif content.comments %} content.comments1 {% else %} else {% endif %}</div>\n") +
        std::string("<div>{% if content.comments %} 1 {% if content.comments %} 2 {% if content.comments %} <div>{{ title }}</div> {% endif %}{% endif %}{% endif %}</div>\n") +
        std::string("<div>{% if content.comments %} comments {% elseif content.comments1 %} {% if content.comments1 %} comments1 {% else %} ---------------- {% endif %} {% else %} else {% endif %}</div>\n") +
        std::string("<div>{% if content.comments is not empty %} 2 {% endif %}</div>\n") +
        std::string("<div>{% if count > 0 %} 4 {% endif %}</div>")
    ;
    canto_temp::ContainerStr con(str);
    str_b = con.find('#');

    std::cout 
        << "file_b: " << file_b << std::endl
        << "str_b: " << str_b
    << std::endl;
}

/** https://ruden.github.io/twig-docs-ru/templates.html
 * ToDo
 * Создание кастомных переменных
 * Тернарный оператор
 * Оператор нулевой коалесценции
 * Наследование шаблонов 
 * Включение других шаблонов
 * 
 * Вывод предупреждения для отладки
 * Создать throw объект для обработки ошибок
 * Список стандартных фильтров
 * Переделать variables чтобы читало переменные одним заходом
*/

/**
 * Выполнил:
 * Условные блоки
 * Фильтры
 * Вставка комментария
 * Доступ к переменным
 * Доступ к элементам массива
 * Доступ к vector
 */

int main(){ // int argc, char *argv[]

    // testJson();
    // testParser();
    // testContainer();
    listTest();
    // testFind();

    return 0;
    
}