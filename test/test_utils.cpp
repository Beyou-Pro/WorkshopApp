// test/test_utils.cpp

#include "libutils/utils.h" // On inclut le header de la librairie à tester
#include <iostream>
#include <string>

int main() {
    // 1. Récupérer la valeur à tester
    std::string actual_name = WorkshopApp::get_project_name();
    
    // 2. Définir la valeur attendue
    std::string expected_name = "WorkshopApp";
    
    // 3. Effectuer la vérification
    if (actual_name == expected_name) {
        std::cout << "Test reussi: Le nom du projet est correct : " << actual_name << std::endl;
        return 0; // Succès
    } else {
        std::cerr << "TEST ECHOUÉ: Le nom attendu et le nom recu ne correspondent pas." << std::endl;
        std::cerr << "  Attendu: " << expected_name << std::endl;
        std::cerr << "  Reçu: " << actual_name << std::endl;
        return 1; // Échec
    }
}
