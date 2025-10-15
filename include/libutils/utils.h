// include/libutils/utils.h
#ifndef LIBUTILS_UTILS_H
#define LIBUTILS_UTILS_H

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>

namespace WorkshopApp {

/**
 * Classe utilitaire pour scanner des répertoires et exécuter des commandes Git.
 */
class RepoScanner {
public:
    /**
     * Initialise le scanner.
     * @param root_path Le chemin de base à scanner.
     * @param dry_run Si vrai, n'exécute aucune commande Git (simule).
     * @param auto_mode Si vrai, exécute auto-commit+push sur les dépôts modifiés.
     */
    RepoScanner(const std::filesystem::path& root_path, bool dry_run, bool auto_mode);

    /**
     * Lance le parcours récursif et l'analyse.
     */
    void scan();

private:
    std::filesystem::path m_root_path;
    bool m_dry_run;
    bool m_auto_mode; // Nouveau mode automatique

    /**
     * Vérifie si le chemin donné est le répertoire racine d'un dépôt Git.
     */
    bool is_git_repository(const std::filesystem::path& path) const;

    /**
     * Exécute une commande shell et retourne le résultat (ou une erreur).
     * @param command La commande à exécuter.
     * @param repo_path Le chemin du dépôt pour l'exécution.
     * @return La sortie de la commande.
     */
    std::string execute_git_command(const std::string& command, const std::filesystem::path& repo_path) const;

    /**
     * Gère l'interaction ou l'auto-commit pour un dépôt avec des modifications.
     */
    void handle_modified_repo(const std::filesystem::path& repo_path, const std::string& status_output) const;

    /**
     * Exécute la séquence git add -A, commit et push.
     */
    void auto_commit_and_push(const std::filesystem::path& repo_path) const;
};

} // namespace WorkshopApp

#endif // LIBUTILS_UTILS_H
