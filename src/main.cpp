// src/main.cpp
#include "libutils/utils.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm> 

namespace fs = std::filesystem;

// Constantes pour les options de la ligne de commande
const std::string DRY_RUN_ARG = "--dry-run";
const std::string AUTO_ARG = "--auto"; // Nouvelle option
const std::string HELP_ARG = "--help";

void show_usage(const std::string& app_name) {
    std::cout << "Utilisation: " << app_name << " <chemin_du_dossier_repos> [" << DRY_RUN_ARG << " | " << AUTO_ARG << "]" << std::endl;
    std::cout << "Exemple: " << app_name << " /home/utilisateur/projets" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  " << DRY_RUN_ARG << " : Simule l'exécution de 'git status' (mode sans action)." << std::endl;
    std::cout << "  " << AUTO_ARG << " : Exécute automatiquement add + commit + push sur tous les dépôts modifiés." << std::endl;
    std::cout << "  " << HELP_ARG << " : Affiche cette aide." << std::endl;
}

int main(int argc, char* argv[]) {
    const std::string app_name = fs::path(argv[0]).filename().string();
    
    // Variables pour les paramètres
    fs::path root_path;
    bool dry_run = false;
    bool auto_mode = false; // Nouvelle variable
    
    // --- 1. Analyse des arguments ---
    
    if (argc < 2) {
        show_usage(app_name);
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == DRY_RUN_ARG) {
            dry_run = true;
        } else if (arg == AUTO_ARG) {
            auto_mode = true;
        } else if (arg == HELP_ARG) {
            show_usage(app_name);
            return 0;
        } else if (root_path.empty()) {
            // Le premier argument non-option est le chemin
            root_path = arg;
        } else {
            std::cerr << "ERREUR: Argument non reconnu ou mal placé: " << arg << std::endl;
            show_usage(app_name);
            return 1;
        }
    }
    
    if (root_path.empty()) {
        std::cerr << "ERREUR: Le chemin du dossier de base est manquant." << std::endl;
        show_usage(app_name);
        return 1;
    }

    // --- Vérifications ---
    if (dry_run && auto_mode) {
         std::cout << "Avertissement: Les modes DRY-RUN et AUTO sont activés. DRY-RUN sera prioritaire." << std::endl;
    }


    // Résoudre le chemin absolu pour une meilleure gestion des commandes shell
    root_path = fs::absolute(root_path);

    // --- 2. Exécution du scanner ---
    try {
        WorkshopApp::RepoScanner scanner(root_path, dry_run, auto_mode);
        scanner.scan();
    } catch (const fs::filesystem_error& e) {
        std::cerr << "ERREUR Filesystem: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "ERREUR Générale: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "ERREUR Inconnue." << std::endl;
        return 1;
    }

    return 0;
}
