#include "metric/metric.hpp"
#include <iomanip>
#include <iostream>


struct _MS_Apply {

    #define OVERLOAD_TYPE(TYPE, BODY)\
        virtual void operator() (TYPE) BODY

    OVERLOAD_TYPE(int, {\
            std::cout << "message 0" << std::endl;\
        })
    OVERLOAD_TYPE(float, {\
            std::cout << "message 0" << std::endl;\
        })
    OVERLOAD_TYPE(double, {\
            std::cout << "message 0" << std::endl;\
        })
    OVERLOAD_TYPE(long, {\
            std::cout << "message 0" << std::endl;\
        })

    #undef OVERLOAD_TYPE
};

struct _MS_Apply_2 : _MS_Apply {
    
    #define OVERLOAD_TYPE(TYPE)\
        void operator() (TYPE) override {\
            std::cout << "message 3" << std::endl;\
        }

    OVERLOAD_TYPE(int)
    OVERLOAD_TYPE(float)
    OVERLOAD_TYPE(double)
    OVERLOAD_TYPE(long)

    #undef OVERLOAD_TYPE
};


int main(int argc, char *argv[]) {

    setlocale(LC_ALL, ".UTF8");
    std::cout << std::setprecision(17) << std::fixed;
    
    _MS_Apply_2 target;
    _MS_Apply* var = &target;
    
    int x;
    float y;

    (*var)(x);
    (*var)(y);
}

