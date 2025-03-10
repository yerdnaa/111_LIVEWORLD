
#include <iomanip>
#include <iostream>
#include <vector>

#include "test_metric.hpp"

#define TEST(name) { std::cout << "START " #name "\n"; if ( not name() ) failed_tests.push_back(#name); }


int main(int argc, char *argv[]) {
    
    setlocale(LC_ALL, ".UTF8");
    std::cout << std::setprecision(17) << std::fixed;

    std::cout << "\n\n====================================\n";
    std::cout << "RUN TESTS\n";
    std::vector<std::string> failed_tests = {};
    

    TEST(test_metric_1)
    TEST(test_metric_setter)
    TEST(test_metric_direct)
    TEST(test_metric_comparison)
    TEST(test_metric_round)
    TEST(test_metric_loop)
    TEST(test_metric_observer)


    if (failed_tests.empty()) {
        std::cout << "\nTESTS COMPLETED SUCCESSFULLY\n";
    } else {
        std::cout << "\nTESTS FAILED:\n";
        for (std::string & str : failed_tests) {
            std::cout << str << "\n";
        }
    }
    std::cout << "====================================\n\n";

    return 0;
}