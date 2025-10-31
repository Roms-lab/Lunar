// -----------------------------
// --- THIS FILE IS OWNED BY ---
// ---    LUNAR LANGUAGE     ---
// -----------------------------

export module Interpreter;

// Import our own modules
import Lunar;
import Reader;

// Use 'import <...>' for C++20 standard library modules
import <iostream>;
import <string>;
import <vector>;
import <algorithm>;


// Helper function to trim leading/trailing whitespace from a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Loads a .lunar file and interprets the contents line by line.
 * @param filename The name of the file to interpret (e.g., "test").
 */
export void InterpretFile(const std::string& filename) {
    std::vector<std::string> script_lines = ReadLunarFile(filename);

    if (script_lines.empty()) {
        std::cerr << "Could not interpret empty or missing script." << std::endl;
        return;
    }

    const std::string print_command = "Lunar::Print(\"";
    const std::string flush_command = "Lunar::Flush();";
    const std::string sleep_prefix = "Lunar::Sleep(";
    const std::string disable_task_mngr = "Lunar::DisableTaskMngr();";
    const std::string enable_task_mngr = "Lunar::EnableTaskMngr();";
    const std::string clear_command = "Lunar::Clear();";

    for (const std::string& raw_line : script_lines) {
        std::string line = trim(raw_line);

        if (line.starts_with(print_command) && line.ends_with("\");")) {
            std::string content = line.substr(print_command.length(), line.length() - print_command.length() - 3);
            Lunar::Print(content);
        }
        else if (line == flush_command) {
            Lunar::Flush();
        }
        else if (line.starts_with(sleep_prefix) && line.ends_with(");")) {
            size_t start_pos = sleep_prefix.length();
            size_t end_pos = line.length() - 2;
            std::string time_str = line.substr(start_pos, end_pos - start_pos);

            try {
                int sleep_time_seconds = std::stoi(time_str);
                Lunar::Sleep(sleep_time_seconds);
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "[Parser Error]: Invalid number format for Sleep command: " << line << std::endl;
            }
            catch (const std::out_of_range& e) {
                std::cerr << "[Parser Error]: Sleep time out of range: " << line << std::endl;
            }
        }
        else if (line == disable_task_mngr) {
            Lunar::DisableTaskMngr();
        }
        else if (line == enable_task_mngr) {
            Lunar::EnableTaskMngr();
        }
        else if (line == clear_command) {
            Lunar::Clear();
        }
        else if (!line.empty() && !line.starts_with("//")) {
            std::cerr << "[Parser Error]: Unknown command or syntax error on line: " << line << std::endl;
        }
    }
}
