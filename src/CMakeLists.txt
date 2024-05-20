# Créer une bibliothèque à partir d'un ensemble de fichiers de définitions (.cxx) sans main.
# Ces fichiers ne contiennent pas de fonction main
# Remplacer ... par les fichiers nécessaires 
add_library(Vector3D Vector3D.cxx)
add_library(Cellule Cellule.cxx)
add_library(Particule3D Particule3D.cxx)
add_library(Univers Univers.cxx Cellule.cxx Particule3D.cxx Vector3D.cxx)
