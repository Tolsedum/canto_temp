#ifndef CONTAINER_STR_HPP
#define CONTAINER_STR_HPP

#include <string>
#include "canto_temp/Container.hpp"

namespace canto_temp{
    class ContainerStr : public Container{
        std::string_view container_;
        std::string_view::const_iterator begin_;
        std::string_view::const_iterator end_;
        bool is_end_;

        void constructor_();
    public:
        ContainerStr(std::string&& str)
            : container_(str)
        {
            constructor_();
        }
        ContainerStr(std::string& str)
            : container_(str)
        {
            constructor_();
        }
        ~ContainerStr(){};
        bool isEnd() override;
        char current() override;
        char next() override;
    };
}

#endif // CONTAINER_STR_HPP