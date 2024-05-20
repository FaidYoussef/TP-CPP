# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(Documentation)

# Trouver le package Doxygen
find_package(Doxygen REQUIRED)

# Chemin vers le fichier Doxyfile existant
set(DOXYFILE ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

# Ajouter une cible pour générer la documentation
add_custom_target(doc
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating API documentation with Doxygen"
  VERBATIM
)

# Optionnel : Ajouter une cible pour nettoyer les fichiers générés par Doxygen
add_custom_target(doc_clean
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_SOURCE_DIR}/html
  COMMENT "Cleaning up generated documentation"
  VERBATIM
)
