#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <functional>
#include <string>
#include <memory>
#include <sstream>

namespace canto_temp{
    class Object{
        struct holder {
            virtual ~holder() { }
            virtual bool cond() const = 0;
            virtual void map(
                const std::function<void (Object)>& f
            ) const = 0;
            virtual std::string str() const = 0;
            virtual Object get(std::string name) = 0;
        };

        template<class T>
        struct holder_impl : holder {
            T obj;
            holder_impl(T obj) : obj(std::move(obj)) { }

            template<
                class U, 
                int = sizeof(std::declval<U>() ? true : false)
            >
            bool cond_(int) const {
                return static_cast<bool>(obj);
            }
            template<class>
            bool cond_(...) const {
                throw "This value does not evaluate.";
            }
            
            virtual bool cond() const override {
                return cond_<T>(0);
            }

            template<
                class U,
                int = sizeof(
                    std::declval<U>().begin(),
                    std::declval<U>().end()
                )
            >
            void map_(
                int, const std::function<void (Object)>& f
            ) const {
                for (auto&& v: obj) {
                    f(v);
                }
            }

            template<class>
            void map_(
                long, 
                const std::function<void (Object)>&, ...
            ) const {
                throw "This value does not have begin() or end().";
            }

            virtual void map(
                const std::function<void (Object)>& f
            ) const override {
                map_<T>(0, f);
            };

            template<
                class U,
                int = sizeof(
                    std::declval<std::stringstream&>() 
                        << std::declval<U>()
                )
            >
            std::string str_(int) const {
                std::stringstream ss;
                ss << obj;
                return ss.str();
            }

            template<class U>
            std::string str_(...) const {
                throw "This value does not have operator<<().";
            }

            virtual std::string str() const override {
                return str_<T>(0);
            }

            template<
                class U,
                int = sizeof(
                    std::declval<U>()[std::declval<std::string>()]
                )
            >
            Object get_(int, std::string name) {
                return obj[std::move(name)];
            }
            
            template<class U>
            Object get_(long, std::string, ...) {
                throw "This value does not have operator[]().";
            }

            virtual Object get(std::string name) override {
                return get_<T>(0, std::move(name));
            }
        };

        std::shared_ptr<holder> holder_;

    public:
        Object() = default;
        Object(const Object&) = default;
        Object(Object&&) = default;
        Object& operator=(const Object&) = default;
        Object& operator=(Object&&) = default;

        template<class T>
        Object(T v) : holder_(new holder_impl<T>(std::move(v))) { }

        template<class T>
        void operator=(T v) { 
            holder_.reset(new holder_impl<T>(std::move(v))); 
        }

        explicit operator bool() const { return holder_->cond(); }
        void map(const std::function<void (Object)>& f) const {
            holder_->map(f); 
        }
        std::string str() const { return holder_->str(); }
        Object operator[](std::string name) {
            return holder_->get(std::move(name)); 
        }
    };
}

#endif // OBJECT_HPP