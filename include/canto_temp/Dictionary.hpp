#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <iostream>
#include <functional>
#include <string>
#include <memory>
#include <sstream>

namespace canto_temp{
    class Dictionary{
    struct ParentValue {
        virtual ~ParentValue() {}
        virtual std::string toString() const = 0;
        virtual bool get() const = 0;
        virtual Dictionary get(std::string name) = 0;
        virtual Dictionary get(int iter) = 0;
    };

    template<class T>
    struct Value : ParentValue{
        T obj;
        Value(T obj) : obj(std::move(obj)) { }
        ~Value(){};

        template<
            class U, 
            int = sizeof(std::declval<U>() ? true : false)
        >
        bool get_(int) const {
            return static_cast<bool>(obj);
        }

        template<class>
        bool get_(...) const {
            throw std::string(
                std::string(typeid(obj).name()) + 

                "\nWarning to Line: "
                    + std::to_string(__LINE__) 
                    + " This value does not evaluate\n"
            );
        }
        
        virtual bool get() const override {
            return get_<T>(0);
        }

        template<
            class U,
            int = sizeof(
                std::declval<std::stringstream&>() 
                    << std::declval<U>()
            )
        >
        std::string toString_(int) const {
            std::stringstream ss;
            ss << obj;
            return ss.str();
        }

        template<class U>
        std::string toString_(...) const {
            throw std::string(
                "\nWarning to Line: "
                    + std::to_string(__LINE__) 
                    + " This value does not have operator<<()\n"
            );
        }

        virtual std::string toString() const override {
            return toString_<T>(0);
        }

        template<
            class U, 
            int = sizeof(
                std::declval<U>()[std::declval<std::string>()]
            )
        >
        Dictionary get_(int, std::string name) {
            return obj[std::move(name)];
        }
        
        template<class U>
        Dictionary get_(long, std::string, ...) {
            throw std::string(
                "\nWarning to Line: "
                    + std::to_string(__LINE__) 
                    + " This value does not have operator[]()\n"
            );
        }

        virtual Dictionary get(std::string name) override {
            return get_<T>(0, std::move(name));
        }


        template<
            class U,
            int = sizeof(
                std::declval<U>()[std::declval<int>()]
            )
        >
        Dictionary get_(int, int iter) {
            return obj[iter];
        }

        template<class U>
        Dictionary get_(long, int, ...) {
            throw std::string(
                "\nWarning to Line: "
                    + std::to_string(__LINE__) 
                    + " This value does not have operator[]()\n"
            );
        }

        virtual Dictionary get(int iter) override{
            return get_<T>(0, iter);
        }
        
    };
    std::shared_ptr<ParentValue> value_;
public:
    ~Dictionary(){};
    Dictionary() = default;
    Dictionary(const Dictionary&) = default;
    Dictionary(Dictionary&&) = default;
    Dictionary& operator=(const Dictionary&) = default;
    Dictionary& operator=(Dictionary&&) = default;

    template<class T>
    Dictionary(T v) : value_(new Value<T>(std::move(v))) { }

    template<class T>
    void operator=(T v) { 
        value_.reset(new Value<T>(std::move(v))); 
    }

    explicit operator bool() const { return value_->get(); }

    std::string toString() const { 
        if(value_){
            return value_->toString();
        }else{
            throw std::string(
                "\nWarning to Line: "
                    + std::to_string(__LINE__) 
                    + " Empty value\n"
            );
        }
    }

    Dictionary operator[](std::string name) {
        return value_->get(std::move(name)); 
    }

    Dictionary operator[](int iter) {
        return value_->get(iter); 
    }
};
}

#endif // OBJECT_HPP