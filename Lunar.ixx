// -----------------------------
// --- THIS FILE IS OWNED BY ---
// ---    LUNAR LANGUAGE     ---
// -----------------------------

export module Lunar;

// Use 'import <...>' for C++20 standard library modules
import <iostream>;
import <string>;
import <chrono>;
import <thread>;
import <limits>;

// Declare the namespace and define the function within its scope
namespace Lunar {

    // Lunar Print (Single argument overload for simple strings/literals)
    export inline void Print(std::string Text) {
        std::cout << Text << "\n";
    }

    // NEW: Lunar Print (Single argument overload for ANY type T)
    export template <typename T>
        inline void Print(const T& Var) {
        std::cout << Var << "\n";
    }

    // Existing Template Print (Two arguments):
    export template <typename T>
        inline void Print(std::string Text, const T& Var) {
        std::cout << Text << Var << "\n";
    }

    // Lunar Input
    export template <typename T>
        inline void Input(std::string Text, T& Var) {
        std::cout << Text;
        std::cin >> Var;
        // Basic buffer clearing
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Lunar Flush
    export inline void Flush() {
        std::cout.flush();
    }

    // NEW: Lunar Sleep
    export inline void Sleep(int Time) {
        std::this_thread::sleep_for(std::chrono::seconds(Time));
    }
}
