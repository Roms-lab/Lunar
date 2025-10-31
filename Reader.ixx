// -----------------------------
// --- THIS FILE IS OWNED BY ---
// ---    LUNAR LANGUAGE     ---
// -----------------------------

export module Reader;

// Use 'import <...>' for C++20 standard library modules
import <iostream>;
import <fstream>;
import <string>;
import <vector>;
import <filesystem>;

// Export the constant so other modules/files can access LUNAR_EXTENSION
export const std::string LUNAR_EXTENSION = ".lunar";

/**
 * @brief Reads a Lunar source file line by line into a vector of strings.
 * [...]
 */
export std::vector<std::string> ReadLunarFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::string full_filename = filename;
    if (!full_filename.ends_with(LUNAR_EXTENSION)) {
        full_filename += LUNAR_EXTENSION;
    }
    std::ifstream file_stream(full_filename);
    if (!file_stream.is_open()) {
        std::cerr << "Error: Could not open file '" << full_filename << "'." << std::endl;
        return lines;
    }
    std::string line;
    while (std::getline(file_stream, line)) {
        lines.push_back(line);
    }
    return lines;
}
