cmake_minimum_required(VERSION 3.10)

project(GitScanner CXX) # Renommer le projet pour mieux refléter sa fonction

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- 1. DÉFINITION DE LA BIBLIOTHÈQUE INTERNE (libutils) ---
add_library(libutils STATIC
    src/libutils/utils.cpp
)

# Rend le répertoire 'include' visible publiquement pour tout lien à libutils.
# Le header est nécessaire pour le main.cpp
target_include_directories(libutils PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- 2. DÉFINITION ET LIAISON DE L'EXÉCUTABLE PRINCIPAL ---
add_executable(GitScanner 
    src/main.cpp
)

# Lier l'exécutable à la bibliothèque 'libutils'
# Nous n'avons pas besoin de liens supplémentaires pour std::filesystem car C++17 le rend généralement disponible
target_link_libraries(GitScanner PRIVATE 
    libutils
)
