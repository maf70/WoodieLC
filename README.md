# Controleur de chaudiere à plaquette de bois

Ce controleur est une version low cost et plus robuste du projet woodie:
https://github.com/maf70/Woodie.git

Le meme pcb est reutilisé sans le PI zero.
Des connexions ont été rajoutées entre les capteurs/commandes entre le connecteur PI à 40 pins et les entrées/sorties du Nano (ATMega328P).

## Description

La chaudiere se compose de:
- Un moteur entrainant une vis sans fin pour convoyer les plaquettes de la trémie vers le foyer.
- un opto-coupleur(s) (LM393) pour controler la rotation de la vis
- un moteur entrainant un ventilateur pour activer la combustion
- un capteur de temperature (DS18B20) de l'eau en sortie de chaudiere
- un capteur de temperature (DS18B20) de sécurité placé sur la vis.

Fonctionnement de la chaudiere:
- En debut de cycle (durée nominale 1 minutes), la vis et le ventilateur sont actionnés
- au bout de 10 secondes, la vis d'alimentation est arretée
- au bout de 50 minutes, le ventilateur est arreté
- au bout de 1 minute, le cycle recommence
- Si la temperature de l'eau est atteinte en fin de cycle, arret des cycles de chauffe
- Dès que la temperature redescend en dessous du seuil bas, redemarrage des cycles de chauffe
- Controle en permanance que les capteurs de temperatures sont toujours valides et que la temperature de securité n'est pas atteinte
- En cas de blocage de la vis, on arrete le moteur puis on le fait tourner en sens inverse pendant quelques secondes
- Verification que le moteur tourne en sens inverse

Securité / detections d'erreur
- Si la sonde de température sur la vis se declenche (seuil haut), on arrete pour éviter que le feu n'atteigne la trémie.
- Si la temperature de l'eau atteint un seuil bas, on arrete l'alimentation pour éviter de remplir le foyer car la chaudiere est probablement eteinte.
- si le nombre de blocage moteur atteint le max, on arrete egalement la chaudiere pour ne pas fatiguer le moteur.
- Si les 2 sondes de temperature ne sont pas correctement detectées (panne ou faux contact), on arrete également la chaudiere.

Le hardware du controleur de la chaudiere :
![](images/pcb.jpg)
- un arduino NANO avec un 328P
- D'une carte 4 relais pour commander le moteur et son inversion, ainsi que le ventilateur
- D'un afficheur SSD1306 pour indiquer l'etat des capteurs et gerer les differents mode du controleur.
- De 4 boutons.

Pour les tests, un banc de test remplace la chaudiere :
![](images/Bench.jpg)

- structure à base de jouet "Knex"
- un petit moteur 12V remplace le moteur de la vis
- celui-ci entraine un cd-rom sur lequel est positionné l'optocoupleur
- un spot à led (3 W) actionné par le relais du ventilateur simule la combustion activée par la ventilation.
- la sonde temperature de l'eau est positionné sous le spot
- la sonde securité mesure la temperature ambiante et peut etre placée sous le spot pour simuler un probleme.
- les consignes de temperatures sont adaptées en fonction du banc

## Mode d'emploi

### Mise sous tension:
- la chaudiere demarre toute seule si la temperature de l'eau n'est pas trop basse, ce qui veut dire qu'elle n'est probablement pas éteinte (cas d'une coupure de courant)
- La chaudiere se met en erreur si l'eau est trop froide. Une action manuelle doit etre effectuée pour démarrer la chaudiere.

### Ecran de fonctionnement:
![](images/Screen_ON.jpg)
Quel que soit l'état de la chaudiere, la premiere ligne indique toujours l'état des capteurs:
- Temperature de l'eau
- Compteur optique
- Nb de blocage
- Temperature de la vis

En fonction de la temperature de l'eau, la deuxieme ligne indique:
- REPOS : Le seuil haut a été atteint, on arrete les cycles de chauffe.
- CHAUFFE : Le seuil bas a été atteint, on enchaine les cycle de chauffe
- DEMARRAGE : Le seuil de securité a été atteint, la premiere chauffe a été déclenchée manuellmeent


La troisieme ligne indique:
- le nombre de secondes dans le cycle courant
- le nombre de cycle depuis le declenchement de la chauffe.

La quatrieme ligne:
- Le nombre de cycle de la derniere chauffe
- Le temps écoulé entre les deux dernieres chauffe

### Ecran ARRET
![](images/Screen_stop.jpg)

### Ecran REGLAGE
![](images/Screen_Settings.jpg)

### Ecran STATS
![](images/Screen_Stats.jpg)

### Ecran VERSION
![](images/Screen_Version.jpg)

### Ecran ERREUR
![](images/Screen_Error.jpg)





