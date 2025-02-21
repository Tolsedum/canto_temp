#include <iostream>
#include "canto_temp/Parser.hpp"

#include "../tests/test_variables.cpp"

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