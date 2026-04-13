#### DISCLAIMER PROJET ENTIEREMENT GENERER PAR GEMINI 2.5 A BUT PEDAGOGIQUE

# 🚀 WorkshopApp : Démo d'une Bibliothèque Interne et de Détection d'OS

Ce projet sert d'exemple simple pour démontrer comment créer et lier une **bibliothèque statique interne (`libutils`)** à un exécutable principal, tout en utilisant les **macros de préprocesseur** pour afficher des messages spécifiques au système d'exploitation.

-----

## 🛠️ Dépendances

Ce projet utilise le système de construction **CMake** et un compilateur C++ moderne.

  * **Compilateur C++ :** Compatible avec la norme C++17 (ex. : GCC, Clang, MSVC).
  * **Système de construction :** CMake (version 3.10 ou supérieure).

-----

## ⚙️ Instructions de Compilation

Le processus de compilation utilise CMake pour générer les fichiers de construction (Makefiles, projets Visual Studio, etc.) et les exécuter.

### Étape 1 : Créer le Dossier de Construction

Il est recommandé d'effectuer la compilation dans un dossier séparé (`build`) pour garder le répertoire source propre.

```bash
mkdir build
cd build
```

### Étape 2 : Configurer CMake

Cette étape génère les fichiers de construction pour votre système.

| Système d'exploitation | Commande CMake |
| :--- | :--- |
| **Linux (ou WSL)** | `cmake ..` |
| **Windows (avec MinGW/MSYS2)** | `cmake .. -G "MinGW Makefiles"` |
| **Windows (avec Visual Studio)** | `cmake .. -G "Visual Studio 17 2022"` |

### Étape 3 : Compiler le Projet

Cette commande utilise les fichiers générés à l'étape 2 pour compiler l'exécutable et la bibliothèque.

```bash
cmake --build .
```

-----

## 🏃 Instructions d'Exécution

L'exécutable compilé (`WorkshopApp` ou `WorkshopApp.exe`) se trouve généralement dans le dossier `build` ou dans un sous-dossier de celui-ci (ex. : `build/Debug/` ou `build/Release/`).

| Système d'exploitation | Commande d'Exécution | Résultat Attendu |
| :--- | :--- | :--- |
| **Linux (ou WSL)** | `./WorkshopApp` | `UTIL_LOG: Hello, Linux user! ...` |
| **Windows** | `.\WorkshopApp.exe` | `UTIL_LOG: Hello, Windows user! ...` |

-----

## 🌳 Structure du Projet

Voici une description des fichiers et dossiers clés du projet :

```
.
├── CMakeLists.txt
├── README.md
├── build/                 <-- Dossier créé pour la compilation (ignoré par Git)
├── include/
│   └── libutils/
│       └── utils.h        <-- Fichier d'en-tête public de la bibliothèque
└── src/
    ├── main.cpp           <-- Fichier exécutable principal (détection d'OS)
    └── libutils/
        └── utils.cpp      <-- Implémentation de la bibliothèque 'libutils'
```

| Fichier/Dossier | Rôle |
| :--- | :--- |
| **`CMakeLists.txt`** | Le script de configuration principal pour CMake. Il définit la bibliothèque et l'exécutable. |
| **`include/libutils/utils.h`** | Déclare la fonction `print_message`. Ce chemin est **exposé publiquement** par CMake. |
| **`src/libutils/utils.cpp`** | Contient l'implémentation de `print_message` qui ajoute le préfixe `UTIL_LOG:`. |
| **`src/main.cpp`** | Le point d'entrée du programme. Il contient la logique `#ifdef` pour détecter le système d'exploitation à la compilation. |
