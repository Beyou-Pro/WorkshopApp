// src/libutils/utils.cpp

#include "libutils/utils.h"
#include <iostream>
#include <cstdio> // Pour popen/pclose
#include <array>
#include <chrono> // Pour le message de commit automatique
#include <ctime>
#include <sstream>
#include <iomanip> // Pour std::put_time

// CORRECTION: Ajout des includes pour std::transform et ::tolower
#include <algorithm> 
#include <cctype>    

namespace fs = std::filesystem;

namespace WorkshopApp {

// --- Constructeur ---
RepoScanner::RepoScanner(const fs::path& root_path, bool dry_run, bool auto_mode)
    : m_root_path(root_path), m_dry_run(dry_run), m_auto_mode(auto_mode) {
    std::cout << "Scanner initialisé. Chemin de base: " << m_root_path.string() << std::endl;
    if (m_dry_run) {
        std::cout << "MODE: DRY-RUN (aucune commande Git ne sera exécutée)." << std::endl;
    }
    if (m_auto_mode) {
        std::cout << "MODE: AUTO (add+commit+push automatique)." << std::endl;
    }
    if (m_dry_run && m_auto_mode) {
        std::cout << "ATTENTION: Le mode DRY-RUN prime sur le mode AUTO." << std::endl;
    }
}

// --- Vérification du Dépôt Git ---
bool RepoScanner::is_git_repository(const fs::path& path) const {
    return fs::exists(path / ".git");
}

// --- Exécution de la Commande Shell ---
std::string RepoScanner::execute_git_command(const std::string& command, const fs::path& repo_path) const {
    // Note: DRY-RUN est géré par les fonctions appelantes (handle_modified_repo, auto_commit_and_push)
    // Ici, nous exécutons la commande si le mode le permet.
    
    std::string full_command = "cd " + repo_path.string() + " && " + command + " 2>&1"; // Redirige stderr vers stdout

    std::array<char, 512> buffer;
    std::string result;
    
    // popen est la fonction portable pour exécuter un programme et lire sa sortie
    FILE* pipe =
    #ifdef _WIN32
        _popen(full_command.c_str(), "r");
    #else
        popen(full_command.c_str(), "r");
    #endif

    if (!pipe) {
        return "ERREUR: Impossible d'ouvrir le pipe pour la commande.";
    }

    try {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
    } catch (...) {
        #ifdef _WIN32
            _pclose(pipe);
        #else
            pclose(pipe);
        #endif
        throw;
    }
    
    int exit_code = 0;
    #ifdef _WIN32
        exit_code = _pclose(pipe);
    #else
        exit_code = pclose(pipe);
    #endif
    
    return result;
}

// --- Auto Commit et Push ---
void RepoScanner::auto_commit_and_push(const fs::path& repo_path) const {
    // Génération du message de commit
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_c);
    
    std::stringstream ss;
    ss << "GitScanner: auto-commit " << std::put_time(local_time, "%Y-%m-%d %H:%M:%S");
    std::string commit_message = ss.str();
    
    std::cout << "  > Exécution: git add -A" << std::endl;
    std::string add_output = execute_git_command("git add -A", repo_path);
    // Afficher l'output d'add seulement en cas d'erreur ou de mode dry-run

    std::cout << "  > Exécution: git commit -m \"" << commit_message << "\"" << std::endl;
    std::string commit_output = execute_git_command("git commit -m \"" + commit_message + "\"", repo_path);
    
    if (commit_output.find("no changes added to commit") != std::string::npos) {
        std::cout << "  [SUCCESS] Aucune modification à commiter (staged)." << std::endl;
        return;
    } else if (commit_output.find("fatal") != std::string::npos || commit_output.find("error") != std::string::npos) {
        std::cerr << "  [ERREUR] Commit échoué. Sortie: " << commit_output;
        return;
    }

    std::cout << "  [SUCCESS] Commit créé." << std::endl;
    
    std::cout << "  > Exécution: git push" << std::endl;
    std::string push_output = execute_git_command("git push", repo_path);

    if (push_output.find("Everything up-to-date") != std::string::npos || push_output.find("Pushed") != std::string::npos) {
        std::cout << "  [SUCCESS] Push réussi." << std::endl;
    } else if (push_output.find("rejected") != std::string::npos || push_output.find("failed to push") != std::string::npos) {
        std::cerr << "  [ERREUR FATALE PUSH] Le push a été rejeté (divergence?). Le dépôt doit être géré manuellement." << std::endl;
        std::cerr << "  Sortie du Push: \n" << push_output;
    } else {
        std::cout << "  [INFO] Push terminé. Sortie non analysée:\n" << push_output;
    }
}

// --- Gestion du Dépôt Modifié (Interactif ou Auto) ---
void RepoScanner::handle_modified_repo(const fs::path& repo_path, const std::string& status_output) const {
    
    std::cout << "--- Dépôt trouvé: " << repo_path.string() << " ---" << std::endl;
    std::cout << "  CHANGEMENTS DÉTECTÉS:\n" << status_output;
    std::cout << "------------------------------------------" << std::endl;

    if (m_dry_run) {
        std::cout << "  [DRY-RUN] Action proposée: auto-commit+push ou skip." << std::endl;
        return;
    }

    if (m_auto_mode) {
        auto_commit_and_push(repo_path);
        return;
    }

    // --- Mode Interactif ---
    std::string input;
    bool valid_input = false;

    while (!valid_input) {
        std::cout << "  Action pour ce dépôt (s=skip / a=add+commit+push): ";
        std::getline(std::cin, input);
        
        // Convertir en minuscule pour la tolérance
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);

        if (input == "a" || input == "add") {
            auto_commit_and_push(repo_path);
            valid_input = true;
        } else if (input == "s" || input == "skip") {
            std::cout << "  [SKIP] Dépôt ignoré." << std::endl;
            valid_input = true;
        } else {
            std::cout << "  Entrée non valide. Utilisez 's' ou 'a'." << std::endl;
        }
    }
}

// --- Fonction de Scan Principale ---
void RepoScanner::scan() {
    if (!fs::is_directory(m_root_path)) {
        std::cerr << "ERREUR: Le chemin de base n'est pas un répertoire valide: " << m_root_path.string() << std::endl;
        return;
    }

    std::cout << "\nDébut du scan récursif pour les dépôts Git...\n" << std::endl;
    
    auto it = fs::recursive_directory_iterator(m_root_path, fs::directory_options::skip_permission_denied);
    auto end = fs::end(it);

    for (; it != end; ++it) {
        const auto& entry = *it;
        
        if (entry.is_directory()) {
            const fs::path current_path = entry.path();
            
            if (is_git_repository(current_path)) {
                
                // Exécute git status --porcelain (format minimal)
                std::string status = execute_git_command("git status --porcelain", current_path);
                
                if (status.empty() || status.find("ERREUR") != std::string::npos) {
                    // Dépôt propre ou erreur d'exécution de git
                    if (status.find("ERREUR") != std::string::npos) {
                        std::cerr << "--- ERREUR GIT DANS DÉPÔT: " << current_path.string() << " ---" << std::endl;
                        std::cerr << status;
                    } else {
                        // Dépôt propre
                        // std::cout << "Dépôt propre: " << current_path.string() << std::endl;
                    }
                } else {
                    // Dépôt avec modifications
                    handle_modified_repo(current_path, status);
                }

                // Pour éviter de scanner l'intérieur du dépôt .git lui-même
                it.disable_recursion_pending();
            }
        }
    }
    std::cout << "\nScan terminé." << std::endl;
}

} // namespace WorkshopApp
