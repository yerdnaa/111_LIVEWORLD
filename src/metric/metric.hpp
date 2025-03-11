#include <cmath>
#include <concepts>
#include <source_location>
#include <stdexcept>
#include <string>
#include <iostream>
#include <algorithm>
#include <vcruntime_typeinfo.h>
#include <vector>

#include "macro.hpp"

#pragma once


#define OVERLOAD_ALL_TYPES \
    OVERLOAD(int) \
    OVERLOAD(float) \
    OVERLOAD(double)

class _A_Metric {

    public:

    #define OVERLOAD(TYPE) virtual void set_direct (TYPE value, bool via_setter = false) = 0;
    OVERLOAD_ALL_TYPES
    #undef  OVERLOAD
};
template<typename T> class Metric;


class _A_Setter {
    
    public:
    
    DELETE_COPY_CONSTR(_A_Setter)
    _A_Setter() { std::cout << "CONSTRUCT SETTER" << std::endl; }
    ~_A_Setter() { std::cout << "DESTRUCT SETTER" << std::endl; }

    virtual void release() = 0;

    #define OVERLOAD(TYPE) virtual void operator() (TYPE value, _A_Metric* metric) = 0;
    OVERLOAD_ALL_TYPES
    #undef  OVERLOAD
};

class M_Setters {

    #define OVERLOAD(TYPE, SETTER_BODY) void operator() (TYPE value, _A_Metric* metric) override SETTER_BODY

    //* Прямой сеттер
    private: struct _Apply : public _A_Setter {
        void release() override {}
        #define SETTER_BODY { metric->set_direct(value); }
        OVERLOAD(int, SETTER_BODY)
        OVERLOAD(float, SETTER_BODY)
        OVERLOAD(double, SETTER_BODY)
        #undef SETTER_BODY
    };

    public: inline static _Apply apply;


    //* Числовые операции (только is_numeric)
    
    // Максимальное число
    private: template <is_numeric T1>
    struct _Max : public _A_Setter {

        T1 max_value;

        _Max(T1 max_value) : max_value(max_value), _A_Setter(){}

        void release() override { delete this; }
        #define SETTER_BODY { M_Setters::apply(value > max_value ? value : max_value, metric); }
        OVERLOAD(int, SETTER_BODY)
        OVERLOAD(float, SETTER_BODY)
        OVERLOAD(double, SETTER_BODY)
        #undef SETTER_BODY
    };
    
    public: template <is_numeric T1>
    static _A_Setter* max(T1 max_value) { return new _Max(max_value); }
    

    // Минимальное число
    private: template <is_numeric T1>
    struct _Min : public _A_Setter {

        T1 min_value;

        _Min(T1 min_value) : min_value(min_value), _A_Setter(){}

        void release() override { delete this; }

        #define SETTER_BODY { M_Setters::apply(value < min_value ? value : min_value, metric); }
        OVERLOAD(int, SETTER_BODY)
        OVERLOAD(float, SETTER_BODY)
        OVERLOAD(double, SETTER_BODY)
        #undef SETTER_BODY
    };
    
    public: template <is_numeric T1>
    static _A_Setter* min(T1 min_value) { return new _Min(min_value); }
    

    // Clamp
    private: template <is_numeric T1, is_numeric T2>
    struct _Clamp : public _A_Setter {

        T1 min_value;
        T2 max_value;

        _Clamp(T1 min_value, T2 max_value) : min_value(min_value), max_value(max_value), _A_Setter(){}

        void release() override { delete this; }

        #define SETTER_BODY { M_Setters::apply(value < min_value ? min_value : ( value > max_value ? max_value : value ), metric) ; }
        OVERLOAD(int, SETTER_BODY)
        OVERLOAD(float, SETTER_BODY)
        OVERLOAD(double, SETTER_BODY)
        #undef SETTER_BODY
    };
    
    public: template <is_numeric T1, is_numeric T2>
    static _A_Setter* clamp(T1 min_value, T2 max_value) { return new _Clamp(min_value, max_value); }

    


    // * Observer pattern
    template <typename T1>
    struct _Notify : public _A_Setter {

        Metric<T1>* listener;

        _Notify(Metric<T1>* listener) : listener(listener), _A_Setter() {}

        void release() override { delete this; }

        #define SETTER_BODY { M_Setters::apply(value, metric); listener->set_direct(value, true); }
        OVERLOAD(int, SETTER_BODY)
        OVERLOAD(float, SETTER_BODY)
        OVERLOAD(double, SETTER_BODY)
        #undef SETTER_BODY
    };
    
    public: template <typename T1>
    static _A_Setter* notify(Metric<T1>* listener) { return new _Notify<T1>(listener); }



    #undef OVERLOAD
    #undef SETTER_BODY_GEN
};




template<typename T>
class Metric : public _A_Metric {

    private:
    
    T _value;
    _A_Setter* _setter;
    
    public:

    void set_setter(_A_Setter* new_setter) {
        _setter->release();
        _setter = new_setter;
    }


    //* Конструкторы

    DELETE_COPY_CONSTR(Metric)

    Metric(T value, _A_Setter* setter = &M_Setters::apply) : _setter(setter) {
        (*this)(value);
    }
    
    template <typename T2>
    Metric(Metric<T2>& other, _A_Setter* setter = &M_Setters::apply) : Metric( T2(other), setter ) {}

    ~Metric() { _setter->release(); }

    
    //* Сеттеры

    #define OVERLOAD(TYPE) \
        void set_direct (TYPE value, bool via_setter = false) override { \
            if (via_setter) (*this)(value); \
            else _value = value; \
        }
        
    template <typename T2>
    T operator() (T2 value) {
        (*_setter)(value, this);
        return _value;
    }

    template <typename T2>
    T operator() (Metric<T2>& other) {
        (*_setter)( static_cast<T2>(other), this);
        return _value;
    }
    
    template <typename T2>
    T operator() (Metric<T2>* other) {
        (*_setter)(static_cast<T2>(*other), this);
        return _value;
    }

    OVERLOAD_ALL_TYPES
    #undef OVERLOAD

    

    //* Перегрузка операторов

    operator T() const {
        return _value;
    }

    // Перегрузки для сравнения чисел с "плавающей" точкой:
    template <std::floating_point T2>
    bool operator== (T2 value) {
        
        // double и float имеют разный epsilon:
        double epsilon = (
            std::numeric_limits<T2>::epsilon() + std::numeric_limits<T>::epsilon()
        ) / 2;

        return 
            (_value >= value - epsilon and _value <= value + epsilon);
    }

    template <std::floating_point T2> bool operator != (T2 value) { return not ( (*this) == value ); }
    template <std::floating_point T2> bool operator >= (T2 value) { return _value > value or ( (*this) == value ); }
    template <std::floating_point T2> bool operator <= (T2 value) { return _value < value or ( (*this) == value ); }
};


#undef OVERLOAD_ALL_TYPES