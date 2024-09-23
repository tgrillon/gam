# Maillages et Géométrie Algorithmique 

Ce projet a été réalisé dans le cadre de l'unité d'enseignement **Maillages et Géométrie Algorithmique** dans le cadre du M2 - ID3D à l'Université Claude Bernard Lyon 1.  

# Structure du projet

```
├── .vscode               # Configuration vscode. 
|   └── settings.json       # Définir les arguments de commande ici.           
├── data                  
|   ├── obj                 # Les fichiers .obj sont sauvegardés ici.          
|   |   └──queen.obj          # Fichier de sortie.
|   ├── off                 # Fichier .off.
|   |   └──queen.off          # Fichier d'entrer.
|   ├── shaders             # Shaders utilisés pour le rendu avec GKit.
|   └── gradient.png        # Texture utilisée pour la diffusion de chaleur. 
├── GAM                   # Code 
|   ├── Include             # Fichiers .h.  
|   |   ├── Application.h     # Classe pour l'affichage (utilise GKit). 
|   |   ├── geometry.h        # Définitions de plusieurs objets : Vertex, Face, Vector, etc...
|   |   ├── TMesh.h           # Classe du maillage triangulaire. 
|   |   └── utils.h           # Utilitaires
|   └── Source              # Fichiers .cpp.  
|       └── main.cpp          
├── .gitignore            # Fichier .gitignore.
├── CMakeLists.txt        # Configuration Cmake.
└── README.md             # Fichier README du projet.
```

# Prérequis

Ce projet a uniquement été compilé/exécuté sous linux.

### Gkit2light

La librairie Gkit2light est utilisée pour visualiser le maillage. Elle doit être installée sur votre machine et la variable CMake `GKIT_DIR` est à définir (correspond au chemin absolue vers le répertoire `gkit2light` sur votre machine) : `GKIT_DIR` = /path/to/gkit2light.

- Pour installer gkit2light : https://perso.univ-lyon1.fr/jean-claude.iehl/Public/educ/M1IMAGE/html/group__installation.html

# Compilation

## Via vscode 

- Configurer cmake sous vscode : https://code.visualstudio.com/docs/cpp/cmake-linux
- Le fichier `.vscode/settings.json` permet de définir les arguments de commande :
```json
{
  "files.associations": {
    ...
  }, 
  "cmake.debugConfig": {
    "args": ["queen"] <- passer le nom du fichier sans l'extension 
  }
}
```
- Définir la variable `GKIT_DIR` : 
  - `F1`->`CMake: Edit CMake Cache (UI)`
  - `GKIT_DIR`: `/path/to/gkit2light`

- Lancer le programme : `CTRL+F5`

## En ligne de commande 

- Configuration/Build 
```
cd /path/to/gam && cmake -B build -DGKIT_DIR=/path/to/gkit2light && cmake --build build/ -t GAM 
``` 

- Exécution 
```
./build/GAM <nom du fichier sans extension (par défault: queen)>
```

# Rapport 

## Maillage triangulaire
![Maillage triangulaire](./data/rapport/mesh.png "Maillage triangulaire")
## Diffusion de la chaleur 
## Normales 

Pour le calcul des normales, on utilise la formule de cotangente du Laplacien qui, pour chaque sommet $i$, nous donne la courbure moyenne $H$ multipliée par la normale $n_i$ : 
$$
-2Hn_i=\frac{1}{2A_i}\sum_j{(\text{cot}\alpha_{ij}+\text{cot}\beta_{ij})(u_j-u_i)}
$$ 

La courbure peut être positive ou négative selon si la forme est concave ou convexe. Si l'on prend la valeur calculée précédemment comme étant la normal, certaines seront inversées :

![Normales inversées](./data/rapport/normal_orientation.png "Normales inversées")

Pour déterminer si une normale doit être inversée, on effectue un test de direction avec l'une des normales des faces voisines au sommet. Si le produit scalaire entre la normale de la face voisine et le vecteur calculé précédemment est négatif, on l'inverse. On obtient ceci :
![Smooth normal](./data/rapport/smooth_normal.png "Smooth normal")

![Smooth normal](./data/rapport/normal2.png "Smooth normal")

L'application permet d'afficher les normales aux sommets ainsi que les normales aux faces en appuyant sur la touche `n`:
![Normal](./data/rapport/normal.png "Normal")
## Courbure  

![Courbure](./data/rapport/courbure.png "Courbure")