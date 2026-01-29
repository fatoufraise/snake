# snake

Ce projet est une implémentation du jeu en langage C.
Le jeu s’affiche directement dans le terminal et fonctionne sous Windows, Linux et Mac, grâce à des directives conditionnelles ("#ifdef _WIN32").

Le joueur déplace le serpent pour manger la nourriture (-->*), gagner des points et allonger sa taille… sans toucher les murs ni son propre corps !

Pour compiler

- Linux / Mac

Assurez-vous d’avoir GCC, puis exécutez : gcc snake.c -o snake



- Windows (MinGW recommandé): gcc snake.c -o snake.exe


Execution 

- Linux / Mac : ./snake

- Windows : snake



Contrôles

| Touche                     | Action         |
| -------------------------- | -------------- |
| **Z** ou **Flèche Haut**   | Monter         |
| **S** ou **Flèche Bas**    | Descendre      |
| **Q** ou **Flèche Gauche** | Aller à gauche |
| **D** ou **Flèche Droite** | Aller à droite |
| **P**                      | Pause          |
| **X**                      | Quitter        |



 Symboles du jeu

| Symbole | Signification    |
| ------- | ---------------- |
| `@`     | Tête du serpent  |
| `O`     | Corps du serpent |
| `*`     | Nourriture       |
| `#`     | Mur              |



Système de jeu

* Chaque nourriture = +10 points
* Le serpent grandit après chaque nourriture
* Le jeu accélère tous les 50 points
* Collision avec le mur ou le serpent = Game Over



 Conseils

 Le jeu fonctionne mieux dans un terminal classique, pas dans l’interface de certains IDE.
 Sous Linux/Mac, évitez les terminaux limités (ex : extensions VSCode) si le rendu bug.


