#include <concepts>
#include <source_location>
#include <stdexcept>
#include <string>

#pragma once

template <typename T>
concept is_numeric = std::integral<T> || std::floating_point<T>;


void INIT_ERROR(const std::source_location & loc, const std::string & message) {
    throw std::runtime_error(
        "\n\t" + message + " at " +
        std::string(loc.file_name()) + " :: line " +
        std::to_string(loc.line()) + " inside `" +
        loc.function_name() + "`"
    );
}


template <typename T>
class Metric {
private:
    
    T _value;
    bool _locked;

public:
    
    Metric() : _locked(true) {}
    Metric(T value) : _value(value), _locked(false) {}
    Metric(Metric<T> & other) : _value(other()), _locked(false) {}

    Metric(const Metric &) = delete;
    Metric(Metric &&) = delete;
    Metric &operator=(const Metric &) = delete;
    Metric &operator=(Metric &&) = delete;


    Metric& operator() (T value) {
        _locked = false;
        _value = value; // TODO setter
        return *this;
    }

    Metric& operator() (Metric<T> & other) {
        _locked = false;
        _value = other(); // TODO setter
        return *this;
    }

    T operator() (const std::source_location loc = std::source_location::current()) {
        if (_locked) {
            INIT_ERROR(loc, "Locked metric access");
        }
        return _value;
    }
    
    bool is_locked() { return _locked; }
    bool is_available() { return not _locked; }
    
    void lock() { _locked = true; }
};