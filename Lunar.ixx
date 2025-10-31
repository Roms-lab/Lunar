// -----------------------------
// --- THIS FILE IS OWNED BY ---
// ---    LUNAR LANGUAGE     ---
// -----------------------------

export module Lunar;

import <iostream>;
import <string>;

// Declare the namespace and define the function within its scope
namespace Lunar {

    // Lunar Print
    
    // 1. Single argument for specific strings (optional but helpful)
    export inline void Print(std::string Text) {
        std::cout << Text << "\n";
    }
    // 2. NEW: Single argument for ANY TYPE (T)
    export template <typename T>
        inline void Print(const T& Var) {
        std::cout << Var << "\n";
    }
    // 3. Two arguments for prefix + variable
    export template <typename T>
        inline void Print(std::string Text, const T& Var) {
        std::cout << Text << Var << "\n";
    }

    // Lunar Input
    export template <typename T>
        inline void Input(std::string Text, T& Var) {
        std::cout << Text;
        std::cin >> Var;
    }

    // Lunar Flush
    export inline void Flush() {
        std::cout.flush();
    }
}
