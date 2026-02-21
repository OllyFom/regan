#pragma once

#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class BaseCommand {
    public:
    BaseCommand() {
        //std::cout << "BaseCommand constructor" << std::endl;
    }
    ~BaseCommand() {
        //std::cout << "BaseCommand destructor" << std::endl;
    }
};