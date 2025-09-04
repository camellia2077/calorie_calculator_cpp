#ifndef CLI_PRINTER_H
#define CLI_PRINTER_H

/**
 * @class CliPrinter
 * @brief Provides static methods for printing CLI usage and help messages.
 *
 * This class centralizes all user-facing text output, separating the "view"
 * from the command-handling "controller" logic in CliHandler.
 */
class CliPrinter {
public:
    /**
     * @brief Prints the main usage information for the entire application.
     * @param progName The name of the executable (e.g., from argv[0]).
     */
    static void printMainUsage(const char* progName);

    /**
     * @brief Prints the detailed help message for the 'calculate' command.
     * @param progName The name of the executable.
     */
    static void printCalculateUsage(const char* progName);

    /**
     * @brief Prints the detailed help message for the 'import' command.
     * @param progName The name of the executable.
     */
    static void printImportUsage(const char* progName);
};

#endif // CLI_PRINTER_H