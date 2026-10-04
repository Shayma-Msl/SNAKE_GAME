SNAKE - Compilation et exécution

Compilation du jeu
------------------
Le projet contient un Makefile qui automatise la compilation de tous les fichiers source.
Depuis un terminal placé dans le dossier du projet, exécuter :

make

Cette commande compile les différents modules du jeu ainsi que la bibliothèque graphique et crée l'exécutable nommé "snake".

Exécution du jeu
----------------
Pour lancer le jeu avec la taille de carte par défaut, exécuter :

./snake

Il est également possible de choisir la taille de la carte en donnant sa dimension en argument.
Par exemple, pour lancer le jeu avec une carte de 25 x 25 cases :

./snake 25

Nettoyage des fichiers compilés
-------------------------------
Pour supprimer les fichiers objets et l'exécutable générés lors de la compilation, exécuter :

make clean


SNAKE - Fonctionnalités supplémentaires

En plus des fonctionnalités demandées dans le sujet, le jeu contient les fonctionnalités suivantes :

- Système de score :
  le joueur gagne 10 points à chaque fruit mangé.

- Système de niveaux :
  le niveau augmente tous les 3 fruits mangés.

- Difficulté progressive :
  la vitesse du serpent augmente progressivement au cours de la partie.

- Ajout progressif d'obstacles :
  de nouveaux murs sont ajoutés lors du passage au niveau suivant.

- Interface graphique améliorée :
  le terrain utilise un affichage en damier et des représentations graphiques personnalisées
  pour le serpent, les fruits et les murs.

- Écran d'accueil :
  un menu graphique avec un bouton START est affiché avant le début de la partie.

- Affichage des informations de jeu :
  le niveau actuel et le score sont affichés pendant la partie.

- Écran de changement de niveau :
  un écran de transition indique le nouveau niveau atteint.

- Écran de fin de partie :
  un écran GAME OVER affiche le score obtenu lorsque le joueur perd.

