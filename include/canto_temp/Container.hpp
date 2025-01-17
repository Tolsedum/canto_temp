#ifndef CANTO_TMP_CONTAINER_HPP
#define CANTO_TMP_CONTAINER_HPP

#include <iostream>
#include <string>

namespace canto_temp{
    class Container{
    public:
    
        virtual ~Container(){};
        virtual bool isEnd() = 0;
        virtual char current() = 0;
        virtual char next() = 0;
        virtual int pos() = 0;
        virtual void setPos(int) = 0;
        virtual std::size_t find(std::string_view, std::size_t s = 0) = 0;
        virtual std::size_t find(char, std::size_t s = 0) = 0;

        virtual void print() = 0;
    };
}

#endif // CANTO_TMP_CONTAINER_HPP

/** 
 * ToDo
 * 
 * Доступ к переменным
 * Доступ к элементам массива
 * фильтры
 * Условные блоки
 * Тернарный оператор
 * Оператор нулевой коалесценции
 * Наследование шаблонов 
 * Включение других шаблонов
 * 
 * Создать throw объект для обработки ошибок
*/

/**
 * Выполнил
 * Вставка комментария
 */