Ce document cherche à résumer l'ensemble des features à implémenter.

# Caméra

Les mouvements que doit réaliser la caméra sont les suivants :
- Dans une salle, si le joueur est à moins de `ScreenSize/2` d'un mur, rester figée (indépendamment en X et Y).
- Dans une salle, si le joueur est à plus de `ScreenSize/2` d'un mur, suivre le joueur (indépendamment en X et Y).
- Lors d'une transition entre deux salles, se déplacer d'une salle à une autre (à la façon d'un Zelda 2D).
- Fonction permettant de déplacer la caméra lors d'events

# Joueur

Features générales :
- Déplacements droite / gauche
- Saut
- Prendre des dégats en cas de collision avec un tir ennemi

## Hors du mecha

- Tirer avec la touche **[à définir]**.
- Intéragir avec des objets avec la touche **[à définir]** (permet notamment de monter à bord du mecha).

## A bord du mecha

- Tirer avec la touche **[à définir]** (le missile est différent).
- Sortir du mecha avec la touche **[à définir]** (même touche que pour l'intégrer).

# Ennemis

- Réagir quand le joueur est dans un rayon visible de X pixels ou si attaqué
- Possibilité de poursuivre le joueur (selon les types d'ennemis - à détailler par la suite)