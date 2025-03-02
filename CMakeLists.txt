# Version minimale de CMake
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(MonProjet VERSION 1.0)

# Spécifier le standard C++ à utiliser
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ajouter les chemins d'inclusion pour le code source et les tests
include_directories(${PROJECT_SOURCE_DIR}/include)

# Ajouter les fichiers source
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Créer l'exécutable principal
add_executable(MonExecutable ${SOURCES})

# Optionnel : Lier des bibliothèques si nécessaire
# target_link_libraries(MonExecutable Bibliotheque)

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.0
)

FetchContent_MakeAvailable(Catch2)

# Créer un exécutable pour les tests
add_executable(tests
    tests/test_feal.cpp
    src/feal.cpp
)

target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
)


enable_testing()

# Ajouter un test
add_test(NAME MonTest COMMAND tests)
