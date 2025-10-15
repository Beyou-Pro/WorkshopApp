#!/bin/bash
# ----------------------------------------------------
# Script d'automatisation de la compilation (Linux/WSL)
# ----------------------------------------------------

# Arrêter le script si une commande échoue
set -e

BUILD_DIR="build"

echo "1. Création du répertoire de construction: $BUILD_DIR"
# Créer le répertoire s'il n'existe pas
mkdir -p $BUILD_DIR

echo "2. Entrée dans le répertoire de construction"
cd $BUILD_DIR

echo "3. Configuration de CMake..."
# Exécute CMake pour générer les Makefiles
cmake ..

echo "4. Compilation du projet..."
# Lance la construction en utilisant les Makefiles générés
cmake --build .

echo "✅ Compilation terminée avec succès !"

# Retour au répertoire racine pour l'exécution facile
cd ..

# Tente d'exécuter l'application
if [ -f "$BUILD_DIR/WorkshopApp" ]; then
    echo ""
    echo "--- Exécution de l'application ---"
    ./$BUILD_DIR/WorkshopApp
    echo "--------------------------------"
fi
