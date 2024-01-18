# twenty-game
Un jeu réalisé en C++  

![image](./Interface%20du%20jeu.png)


### Dépendances  
* `g++` pour compiler en C++ 
* `glut` OpenGL pour gérer le moteur de jeu 2D
* `sndfile` Bibliothèque  pour gérer les images
* `portaudio` Bibliothèque pour gérer l'audio

### Compilation

Exécutez la commande suivante

```bash 
g++ -g "./main.cpp" -o "./Le_Jeu" -lfreeglut -lopengl32  -lsndfile -lportaudio; .\Le_Jeu.exe
```

Cela utilisera le compilateur g++ pour construire un exécutable, prenant en entrée le fichier main.cpp et générant l'exécutable Le_Jeu.

> Notice: Cette commande fonctionne sur Windows, adaptez-la pour qu'elle fonctionne sur votre système.


### Architecture globale

![image](./Architecture%20globale%20du%20jeu.png)
