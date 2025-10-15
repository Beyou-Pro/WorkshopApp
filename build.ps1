# --------------------------------------------------------
# Script d'automatisation de la compilation (PowerShell)
# --------------------------------------------------------

# Arrêter l'exécution si une commande retourne une erreur non-terminante
$ErrorActionPreference = "Stop"

$BUILD_DIR = "build"

Write-Host "1. Création du répertoire de construction: $BUILD_DIR"
# Crée le répertoire s'il n'existe pas
mkdir $BUILD_DIR -Force

Write-Host "2. Entrée dans le répertoire de construction"
cd $BUILD_DIR

Write-Host "3. Configuration de CMake..."
# Configure CMake. Si vous utilisez Visual Studio, CMake détectera
# automatiquement le générateur (ex: "Visual Studio 17 2022").
# Si vous utilisez MinGW, il est préférable d'utiliser -G "MinGW Makefiles".
cmake ..

Write-Host "4. Compilation du projet..."
# Lance la construction avec le générateur configuré
cmake --build .

Write-Host "✅ Compilation terminée avec succès !"

# Retour au répertoire racine pour l'exécution facile
cd ..

# Tente d'exécuter l'application
$ExecutablePath = Join-Path $BUILD_DIR "WorkshopApp.exe"

# Vérifie si l'exécutable existe (le chemin peut varier selon le générateur)
if (Test-Path $ExecutablePath) {
    Write-Host ""
    Write-Host "--- Exécution de l'application ---"
    & $ExecutablePath
    Write-Host "---------------------------"
}
