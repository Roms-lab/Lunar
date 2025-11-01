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
import <unordered_map>;
import <variant>;
import <stdexcept>;
import <sstream>;
import <fstream>; // Added for file I/O

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

    if (script_lines.empty()) { std::cerr << "Could not interpret empty or missing script." << std::endl; return; }

    using LunarValue = std::variant<int, double, std::string, bool>;
    std::unordered_map<std::string, LunarValue> variables;

    std::ofstream outputFile; std::ifstream inputFile;

    const std::string print_var_start = "Lunar::Print(";
    const std::string print_two_arg_prefix = "Lunar::Print(\"";
    const std::string flush_command = "Lunar::Flush();";
    const std::string sleep_prefix = "Lunar::Sleep(";
    const std::string disable_task_mngr = "Lunar::DisableTaskMngr();";
    const std::string enable_task_mngr = "Lunar::EnableTaskMngr();";
    const std::string clear_command = "Lunar::Clear();";
    const std::string ip_command = "Lunar::Ip();";
    const std::string lunar_var = "Lunar::Var";
    const std::string open_file_prefix = "Lunar::OpenFile(";
    const std::string write_file_prefix = "Lunar::WriteFile(";
    const std::string close_file_command = "Lunar::CloseFile();";
    const std::string read_file_prefix = "Lunar::ReadFile(";
    const std::string lunar_v = "Lunar -v";

    for (const std::string& raw_line : script_lines) {
        std::string line = trim(raw_line);
        if (line.empty() || line.starts_with("//") || line.starts_with("--") || line.starts_with("---")) { continue; }

        // --- START OF FLAT COMMAND CHAIN ---

        // Check 1: Handle TWO-ARGUMENT Print (e.g., Print("Status:", var))
        if (line.starts_with(print_two_arg_prefix) && line.ends_with(");")) {
            size_t commaPos = line.find(',');
            if (commaPos != std::string::npos) {
                std::string textPart = line.substr(print_two_arg_prefix.length(), commaPos - print_two_arg_prefix.length() - 1);
                textPart = trim(textPart);

                // Fixed the substring calculation using find_last_of
                size_t varNameStart = commaPos + 1;
                size_t varNameEnd = line.find_last_of(')');
                std::string varNamePart = line.substr(varNameStart, varNameEnd - varNameStart);
                varNamePart = trim(varNamePart);

                if (variables.count(varNamePart)) {
                    LunarValue valueToPrint = variables[varNamePart];
                    std::visit([&](const auto& val) { Lunar::Print(textPart, val); }, valueToPrint);
                }
                else { std::cerr << "Error: Undefined variable in two-argument print: '" << varNamePart << "'\n"; }
            }
            // Note: Removed the 'else' that caused an error for single-arg prints
        }
        // Check 2: Handle single-argument Lunar::Print(...) command (Variables and Literals)
        else if (line.starts_with(print_var_start) && line.ends_with(");")) {
            size_t start = line.find('(') + 1; size_t end = line.find_last_of(')'); std::string content = line.substr(start, end - start); content = trim(content);
            if (content.starts_with('"') && content.ends_with('"')) { std::string actualValue = content.substr(1, content.length() - 2); Lunar::Print(actualValue); }
            else { std::string varName = content; if (variables.count(varName)) { LunarValue valueToPrint = variables[varName]; std::visit([](const auto& val) { Lunar::Print(val); }, valueToPrint); } else { std::cerr << "Error: Undefined variable or invalid print argument: '" << varName << "'\n"; } }
        }
        // Check 3: Variable Declaration and Assignment
        else if (line.starts_with(lunar_var)) {
            size_t typeStart = line.find("Lunar::Var") + std::string("Lunar::Var").length(); size_t nameStart = line.find_first_not_of(" \t", typeStart); size_t nameEnd = line.find_first_of(" \t=", nameStart); std::string varType = line.substr(nameStart, nameEnd - nameStart); nameStart = line.find_first_not_of(" \t", nameEnd); nameEnd = line.find_first_of(" \t=", nameStart); std::string varName = line.substr(nameStart, nameEnd - nameStart); size_t equalsPos = line.find('=', nameEnd); std::string valueExpr = line.substr(equalsPos + 1); valueExpr = trim(valueExpr); if (!valueExpr.empty() && valueExpr.back() == ';') { valueExpr.pop_back(); } valueExpr = trim(valueExpr);
            if (varType == "string" || varType == "s") { if (valueExpr.starts_with('"') && valueExpr.ends_with('"')) { std::string actualValue = valueExpr.substr(1, valueExpr.length() - 2); variables[varName] = actualValue; } else { std::cerr << "Error: Expected a quoted string literal for string type.\n"; } }
            else if (varType == "double" || varType == "float") { try { variables[varName] = std::stod(valueExpr); } catch (...) { std::cerr << "Error: Invalid number format for variable " << varName << std::endl; } }
            else if (varType == "int" || varType == "i") { try { variables[varName] = std::stoi(valueExpr); } catch (...) { std::cerr << "Error: Invalid integer format for variable " << varName << std::endl; } }
            else if (varType == "bool") { if (valueExpr == "true") { variables[varName] = true; } else if (valueExpr == "false") { variables[varName] = false; } else { std::cerr << "Error: Expected 'true' or 'false' for boolean type.\n"; } }
            else { std::cerr << "Error: Unknown or unsupported type '" << varType << "'\n"; }
        }
        // Check 4: Other commands (like Flush)
        else if (line == flush_command) { Lunar::Flush(); }
        else if (line.starts_with(sleep_prefix) && line.ends_with(");")) { size_t start_pos = sleep_prefix.length(); size_t end_pos = line.length() - 2; std::string time_str = line.substr(start_pos, end_pos - start_pos); try { int sleep_time_seconds = std::stoi(time_str); Lunar::Sleep(sleep_time_seconds); } catch (...) { std::cerr << "[Parser Error]: Invalid number format for Sleep command: " << line << std::endl; } }
        else if (line == disable_task_mngr) { Lunar::DisableTaskMngr(); }
        else if (line == enable_task_mngr) { Lunar::EnableTaskMngr(); }
        else if (line == clear_command) { Lunar::Clear(); }
        else if (line == ip_command) { Lunar::Ip(); }
        else if (line == lunar_v) { Lunar::Lunar_V(); }

        // --- START OF NEW FILE I/O LOGIC ---
        else if (line.starts_with(open_file_prefix) && line.ends_with(");")) {
            std::string filenameStr = line.substr(open_file_prefix.length(), line.length() - open_file_prefix.length() - 2); filenameStr = trim(filenameStr);
            if (filenameStr.starts_with('"') && filenameStr.ends_with('"')) { filenameStr = filenameStr.substr(1, filenameStr.length() - 2); if (outputFile.is_open()) outputFile.close(); outputFile.open(filenameStr, std::ios::out | std::ios::trunc); if (!outputFile.is_open()) std::cerr << "Error: Could not open file: " << filenameStr << std::endl; }
            else { std::cerr << "Error: OpenFile expects a literal string filename in quotes.\n"; }
        }
        else if (line.starts_with(write_file_prefix) && line.ends_with(");")) {
            std::string content = line.substr(write_file_prefix.length(), line.length() - write_file_prefix.length() - 2); content = trim(content);
            if (outputFile.is_open()) {
                if (content.starts_with('"') && content.ends_with('"')) { content = content.substr(1, content.length() - 2); outputFile << content << std::endl; }
                else if (variables.count(content)) { LunarValue val = variables[content]; std::visit([&](const auto& v) { outputFile << v << std::endl; }, val); }
                else { std::cerr << "Error: WriteFile expects a literal string in quotes OR a valid variable name (concatenation not supported yet).\n"; }
            }
            else { std::cerr << "Error: No file open for writing.\n"; }
        }
        else if (line == close_file_command) { if (outputFile.is_open()) outputFile.close(); }
        // --- END OF NEW FILE I/O LOGIC ---

        else { std::cerr << "[Parser Error]: Unknown command or syntax error on line: " << line << std::endl; }
        // --- END OF FLAT COMMAND CHAIN ---
    }
}
