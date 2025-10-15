cmake_minimum_required(VERSION 3.10)

project(WorkshopApp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- 1. DÉFINITION DE LA BIBLIOTHÈQUE INTERNE (libutils) ---
add_library(libutils STATIC
    src/libutils/utils.cpp
)

# Rend le répertoire 'include' visible publiquement pour tout lien à libutils.
# Cela permet à main.cpp de trouver "libutils/utils.h".
target_include_directories(libutils PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- 2. DÉFINITION ET LIAISON DE L'EXÉCUTABLE PRINCIPAL ---
add_executable(WorkshopApp 
    src/main.cpp
)

# Lier l'exécutable à la bibliothèque 'libutils'. 
# Grâce à 'target_include_directories(libutils PUBLIC ...)', 
# l'exécutable hérite de l'include path nécessaire.
target_link_libraries(WorkshopApp PRIVATE 
    libutils
)