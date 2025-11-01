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
import <algorithm>; // Needed for std::find_first_not_of/last_not_of in trim
import <unordered_map>;
import <variant>;
import <stdexcept>; // Needed for std::invalid_argument/out_of_range in Sleep
import <sstream>; // Needed for stringstream (though we moved away from it)


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

    using LunarValue = std::variant<int, double, std::string, bool>;
    std::unordered_map<std::string, LunarValue> variables;

    // Define constants locally within the function scope
    const std::string print_command = "Lunar::Print(\""; // Original
    const std::string print_var_start = "Lunar::Print("; // Used for general print handling
    const std::string flush_command = "Lunar::Flush();";
    const std::string sleep_prefix = "Lunar::Sleep(";
    const std::string disable_task_mngr = "Lunar::DisableTaskMngr();";
    const std::string enable_task_mngr = "Lunar::EnableTaskMngr();";
    const std::string clear_command = "Lunar::Clear();";
    const std::string ip_command = "Lunar::Ip();";
    const std::string lunar_var = "Lunar::Var";

    for (const std::string& raw_line : script_lines) {
        std::string line = trim(raw_line);

        // --- START OF FLAT COMMAND CHAIN ---

        if (line.starts_with(print_var_start) && line.ends_with(");")) {
            // Logic for handling Print("...") AND Print(variable)
            size_t start = line.find('(') + 1;
            size_t end = line.find_last_of(')');
            std::string content = line.substr(start, end - start);
            content = trim(content);

            if (content.starts_with('"') && content.ends_with('"')) {
                std::string actualValue = content.substr(1, content.length() - 2);
                Lunar::Print(actualValue);
            }
            else {
                std::string varName = content;
                if (variables.count(varName)) {
                    LunarValue valueToPrint = variables[varName];
                    std::visit([](const auto& val) { Lunar::Print(val); }, valueToPrint);
                }
                else { std::cerr << "Error: Undefined variable or invalid print argument: '" << varName << "'\n"; }
            }
        }
        if (line.starts_with(print_var_start) && line.ends_with(");")) {
            size_t start = line.find('(') + 1;
            size_t end = line.find_last_of(')');
            std::string content = line.substr(start, end - start);
            content = trim(content);

            if (content.starts_with('"') && content.ends_with('"')) {
                std::string actualValue = content.substr(1, content.length() - 2);
                Lunar::Print(actualValue);
            }
            else {
                std::string varName = content;
                if (variables.count(varName)) {
                    LunarValue valueToPrint = variables[varName];
                    std::visit([](const auto& val) { Lunar::Print(val); }, valueToPrint);
                }
                else { std::cerr << "Error: Undefined variable or invalid print argument: '" << varName << "'\n"; }
            }
        }
        // Check 2: Variable Declaration and Assignment (Now includes bool, int, double, string)
        else if (line.starts_with(lunar_var)) {
            size_t typeStart = line.find("Lunar::Var") + std::string("Lunar::Var").length();
            size_t nameStart = line.find_first_not_of(" \t", typeStart);
            size_t nameEnd = line.find_first_of(" \t=", nameStart);
            std::string varType = line.substr(nameStart, nameEnd - nameStart);
            nameStart = line.find_first_not_of(" \t", nameEnd);
            nameEnd = line.find_first_of(" \t=", nameStart);
            std::string varName = line.substr(nameStart, nameEnd - nameStart);
            size_t equalsPos = line.find('=', nameEnd);
            std::string valueExpr = line.substr(equalsPos + 1);
            valueExpr = trim(valueExpr);
            if (!valueExpr.empty() && valueExpr.back() == ';') { valueExpr.pop_back(); }
            valueExpr = trim(valueExpr);

            if (varType == "string" || varType == "s") {
                if (valueExpr.starts_with('"') && valueExpr.ends_with('"')) {
                    std::string actualValue = valueExpr.substr(1, valueExpr.length() - 2);
                    variables[varName] = actualValue;
                }
                else { std::cerr << "Error: Expected a quoted string literal for string type.\n"; }
            }
            else if (varType == "double" || varType == "float") {
                try { variables[varName] = std::stod(valueExpr); }
                catch (...) { std::cerr << "Error: Invalid number format for variable " << varName << std::endl; }
            }
            else if (varType == "int" || varType == "i") {
                try { variables[varName] = std::stoi(valueExpr); }
                catch (...) { std::cerr << "Error: Invalid integer format for variable " << varName << std::endl; }
            }
            else if (varType == "bool") {
                if (valueExpr == "true") { variables[varName] = true; }
                else if (valueExpr == "false") { variables[varName] = false; }
                else { std::cerr << "Error: Expected 'true' or 'false' for boolean type.\n"; }
            }
            else { std::cerr << "Error: Unknown or unsupported type '" << varType << "'\n"; }
        }

        // Check 3: Other commands (like Flush)
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
        else if (line == ip_command) {
            Lunar::Ip();
        }
        else if (!line.empty() && !line.starts_with("//")) {
            // General catch-all for unknown commands or comments
            std::cerr << "[Parser Error]: Unknown command or syntax error on line: " << line << std::endl;
        }
        // --- END OF FLAT COMMAND CHAIN ---
    }
}
