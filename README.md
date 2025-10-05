This program will create a dynamically allocated food chain/web array. It does the following:

First it will read the different organisms names in the food chain and add them to our dynamic array.

Second it will read in the predator prey relations indices in <predator> <prey> format. For example, if you entered cat as the first organism, and mouse as the second orgainsm, you might want to enter 0 1 as a relation.
That means, the organism at index 0 (the cat), preys on index 1 (the mouse).

The program then prints out details of our web. It prints the following:

apex predators: organisms not eaten by any other organism in our web.
Producers: organisms that do not eat any other organism
Most flexible eaters: organisms that eat the greatest variety of other organisms
Tastiest food: Most eaten organism
Food web heights: it prints out what height each organism is in the food chain. For example if dog preys on cat, cat on mouse, and mouse on grass, dog would have height 3, cat 2, mouse 1, and grass 0.
Herbivores:
Omnivores:
Carnivores:

At the end a menu will pop up which will show us the following possible commands/things we can do in our program:

command o: will add organism to our web.
command r: will add a new relation in our web.
command x: remove a organism from our web.
command p: print our updated web.
command d: display all characteristics of our web (listed above).
command q: quit program.

NOTE: if you do not enter plants as the lowest on the food chain (organisms that do not eat any other organism), you might get something silly printed out such as a cat being a herbivore when you print the web.
