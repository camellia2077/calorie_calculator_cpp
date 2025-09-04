#include "app/Application.h"
#include "cli/CliHandler.h"
#include "cli/CliPrinter.h" 

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char* argv[]) {
    // 1. Set up the execution environment
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // 2. Create the core application and CLI handler objects
    Application app("food_data.json", "output_config.json");
    CliHandler cli(app);

    // 3. Run the handler to determine the required action
    CliAction action = cli.run(argc, argv);

    // 4. Perform the action determined by the handler
    switch (action) {
        case CliAction::SUCCESS:
            return 0;
        case CliAction::PRINT_MAIN_HELP:
            CliPrinter::printMainUsage(argv[0]);
            return 0;
        case CliAction::PRINT_CALCULATE_HELP:
            CliPrinter::printCalculateUsage(argv[0]);
            return 0;
        case CliAction::PRINT_IMPORT_HELP:
            CliPrinter::printImportUsage(argv[0]);
            return 0;
        case CliAction::COMMAND_ERROR:
            return 1;
        default:
            return 1; // Should not happen
    }
}