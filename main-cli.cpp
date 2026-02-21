#include <iostream>
#include <optional>
#include <exception>

#include "accessory/commands_fabric.hpp"
#include "accessory/parse_args.hpp"

// TODO: unlink svc-manager, must be called by monitor commands
#include "svc-manager.hpp"

#include <boost/program_options.hpp>


int main(int argc, char* argv[]) {

    try {
        
        std::string try_command = "analyse";
        CommandsFabric comm;
        auto command = comm.CreateCommand(try_command);

        // TODO: запуск службы через monitor command (service-manager) для получения сообщений из драйвера
        ServiceMgr svc_mgr;
        if (!svc_mgr.Install()) {
            throw std::runtime_error("Failed to install service");
        }
        if (!svc_mgr.Uninstall()) {
            throw std::runtime_error("Failed to uninstall service");
        }
        
    } catch (const std::exception& e) {
        std::cout << "Something wrong: " << e.what() << std::endl;
    };
    return 0;
}
