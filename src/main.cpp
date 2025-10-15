// main.cpp

// Inclusion du header de notre nouvelle librairie
#include "libutils/utils.h"
#include <string>

int main() {
    std::string os_message;
    
    // Détection du système d'exploitation à la compilation
    #if defined(_WIN32)
        // Macro standard pour les systèmes 32-bit et 64-bit de Microsoft Windows
        os_message = "Hello, Windows user! This message came from the internal library.";
    #elif defined(__linux__)
        // Macro standard pour les systèmes Linux
        os_message = "Hello, Linux user! This message came from the internal library.";
    #else
        // Message par défaut pour toute autre plateforme (macOS, BSD, etc.)
        os_message = "Hello, unknown OS user! This message came from the internal library.";
    #endif

    // Utilisation de la fonction de la bibliothèque
    WorkshopApp::print_message(os_message);
    
    return 0;
}