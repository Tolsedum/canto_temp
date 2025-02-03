#include <iostream>
#include "canto_temp/Parser.hpp"
#include "canto_temp/ContainerStr.hpp"
#include "canto_temp/FileContainer.hpp"

#include "../tests/test_variables.cpp"

void exist(canto_temp::Container * con){
    std::size_t pos = con->find("world");
    con->setPos(pos);
    std::cout
        << "pos "
        << con->pos() 
        << " current: " << con->current()
    << std::endl;
    

    if(pos != std::string::npos){
        std::cout
            << "found on pos "
            << pos 
        << std::endl;
    }else{
        std::cout 
            << "Not found on pos "
            << pos 
        << std::endl;
    }
}

void testContainer(){
    canto_temp::FileContainer con_f("test");
    exist(&con_f);

    std::string str{"Hello world!"};
    canto_temp::ContainerStr con(str);

    exist(&con);
    
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
    std::map<std::string, canto_temp::Dictionary> xs{
        { "enable", true },
        { "url", "http://example.com" },
        // { "value", "Example" },
        { "value", std::vector<int>{1, 2} },
        { "sss", 15.15 },
    };
    
    std::map<std::string, canto_temp::Dictionary> t;
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

/** 
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

    // testParser();
    // testContainer();
    listTest();

    return 0;
    
}