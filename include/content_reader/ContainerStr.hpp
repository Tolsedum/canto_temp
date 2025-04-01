#ifndef CONTAINER_STR_HPP
#define CONTAINER_STR_HPP

#include "content_reader/Container.hpp"

namespace content_reader{
    class ContainerStr : public Container{
        bool is_end_;
        int pos_;
        std::size_t content_size_;
        std::string_view container_;
        std::string_view::const_iterator begin_;
        std::string_view::const_iterator end_;

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
        bool isNotEnd() override;
        char current() override;
        char next() override;
        int pos() override;
        void setPos(int) override;
        std::size_t find(std::string_view str, std::size_t p = 0) override;
        std::size_t find(char c, std::size_t p = 0) override;
        std::size_t size() override;

        void print() override {std::cout<< begin_ << std::endl;};
    };
}

#endif // CONTAINER_STR_HPP