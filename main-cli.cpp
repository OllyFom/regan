#include <iostream>
#include <optional>

#include "accessory/commands_fabric.hpp"
#include "accessory/parse_args.hpp"

#include <boost/program_options.hpp>


int main(int argc, char* argv[]) {

    try {
        
        std::string try_command = "analyse";
        CommandsFabric comm;
        auto command = comm.CreateCommand(try_command);

        // TODO: запуск службы через service-manager для получения сообщений из драйвера

    } catch (const std::exception& e) {
        std::cout << "Something wrong" << std::endl;
    };
    return 0;
}
