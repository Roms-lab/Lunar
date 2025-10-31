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

int main() {
    std::string source_file_name = "test" + LUNAR_EXTENSION;

    // --- Check if the file exists before running the interpreter ---
    if (!std::filesystem::exists(source_file_name)) {
        std::cerr << "Error: Source file '" << source_file_name << "' not found." << std::endl;
        // Pause before exiting due to error
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        return 1;
    }
    // ---------------------------------------------------------

    // --- Run the interpretation process ---
    InterpretFile("test");

    // --- Pause the console window only after execution finishes ---
    std::cout << "\n[Process finished] Press Enter to exit." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}
