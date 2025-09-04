#ifndef CLI_HANDLER_H
#define CLI_HANDLER_H

#include <string>
#include <vector>

// Forward-declaration
class Application;

/**
 * @enum CliAction
 * @brief Represents the action that the main function should take after the CLI is handled.
 */
enum class CliAction {
    SUCCESS,
    PRINT_MAIN_HELP,
    PRINT_CALCULATE_HELP,
    PRINT_IMPORT_HELP,
    COMMAND_ERROR // <-- Changed from ERROR to COMMAND_ERROR to avoid macro collision
};

class CliHandler {
public:
    explicit CliHandler(Application& app);

    /**
     * @brief The main entry point for the CLI logic.
     * @return A CliAction code indicating what the caller should do next.
     */
    CliAction run(int argc, char* argv[]);

private:
    Application& app_;

    CliAction handleCalculate(const std::vector<std::string>& args);
    CliAction handleImport(const std::vector<std::string>& args);
};

#endif // CLI_HANDLER_H