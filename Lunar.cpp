// -----------------------------
// --- THIS FILE IS OWNED BY ---
// ---    LUNAR LANGUAGE     ---
// -----------------------------

// Import our custom modules
import Reader;
import Lunar;
import Interpreter;

// Use 'import <...>' instead of '#include <...>' where possible in C++20 projects
import <iostream>;
import <fstream>;
import <filesystem>;
import <string>;
import <limits>;
#include <vector> // Required for processing command line arguments properly

int main(int argc, char* argv[]) { // main now accepts command line arguments

    // --- ARGUMENT HANDLING LOGIC ---
    if (argc > 1) {
        std::string firstArg = argv[1]; // Get the first argument provided

        if (firstArg == "-v" || firstArg == "--version") {
            // Run version command logic and exit
            std::cout << "Copyright 2025 Lunar\n";
            std::cout << "Version 0.0.5\n";
            // Pause before exiting
            std::cout << "\n[Process finished] Press Enter to exit." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return 0; // Exit successfully
        }
        else {
            // Assume the argument is a filename to interpret
            std::string source_file_name = firstArg;

            if (!std::filesystem::exists(source_file_name)) {
                std::cerr << "Error: Source file '" << source_file_name << "' not found." << std::endl;
                // Pause before exiting due to error
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                return 1;
            }

            // Run the interpretation process
            // Note: I assume InterpretFile needs "filename" not "filename.lunar" based on your previous code
            // You might need to adjust this if InterpretFile expects the full name including LUNAR_EXTENSION
            InterpretFile(source_file_name);
        }

    }
    else {
        // --- NO ARGUMENTS PROVIDED ---
        // Your previous default behavior (running "test" file) can go here
        std::string source_file_name = "test" + std::string(LUNAR_EXTENSION); // Assuming LUNAR_EXTENSION is defined elsewhere

        if (!std::filesystem::exists(source_file_name)) {
            std::cerr << "Error: Source file '" << source_file_name << "' not found." << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
            return 1;
        }
        InterpretFile("test"); // Again, check if this needs the full filename

    }

    // --- Pause the console window only after execution finishes ---
    std::cout << "\n[Process finished] Press Enter to exit." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}
