#ifndef CANTO_TMP_CONTAINER_HPP
#define CANTO_TMP_CONTAINER_HPP

namespace canto_temp{
    class Container{
    public:
    
        virtual ~Container(){};
        virtual bool isEnd() = 0;
        virtual char current() = 0;
        virtual char next() = 0;
    };
}

#endif // CANTO_TMP_CONTAINER_HPP