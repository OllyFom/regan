#include <iostream>
#include <optional>

#include "accessory/commands_fabric.hpp"
#include "accessory/parse_args.hpp"

#include <boost/program_options.hpp>


//#include <boost> command line parser like cli edr

int main(int argc, char* argv[]) {

    try {
        
        std::string try_command = "analyse";
        CommandsFabric comm;
        auto command = comm.CreateCommand(try_command);

    } catch (const std::exception& e) {
        std::cout << "Something wrong" << std::endl;
    };
    return 0;
}