#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <stdarg.h>
#include "projekti.h"



int main (void) {

    Character * newChar = NULL;     // initializes new character which is passed to add_character function

    Character * (* reloadArray) = NULL;     // array that stores return values of reloadChars function

    Character * (* array) = NULL;     // array that stores return values of add_character function
    array = calloc (1, sizeof(Character *));

    int repeat = 1;     // While loop repeat until Q
    char buffer [80];       // buffer for user input which is passed to add_character function
    int charNum = 0;    // calculates number of added characters
    int playersAlive = 0;   // calculates how many players are still alive

    char * filename = NULL; // Name of the file to which characters will be copied
    filename = calloc(30, sizeof(char));


    // Null characters allocation. Nullcharacter goes to the end of Character * array.

    Character * charNULL = NULL;  

        charNULL = calloc(1, sizeof(Character));

        charNULL->c_name = calloc(2, sizeof(char));

        charNULL->c_weapon = calloc(1, sizeof(Weapon));

        charNULL->c_weapon->weaponName = calloc(10, sizeof(char));


    // random number seed

        time_t t;

        srand((unsigned) time(&t));   


    while (repeat) {

        printf("\nPlease type in a command:\t\tExample:\nA for character addition\t\tA Name HP Weapon Damage\nL for listing added characters\t\tL\nH for commencing an attack\t\tH Attacker Target\nW for saving characters\t\t\tW Filename\nO for reloading characters\t\tO\nQ for quitting the game\t\t\tQ\n");
        char *cmd = fgets(buffer, 80, stdin);
        if (cmd == NULL) {
            printf("\nfgets returned NULL\n");
            break;
        }
    
        switch (cmd[0]) {
            case ('A'):

                newChar = add_character(buffer, newChar, charNum, array);

                if (newChar != 0) {

                    if (charNum == 0) {

                    printf("\nAdd successful. There is %d player in the list.\n", charNum + 1);

                    } else {

                        printf("\nAdd successful. There are %d players in the list.\n", charNum + 1);
                    }
                }

                if (newChar != 0) {
                    array = realloc(array, sizeof(Character *) * (charNum + 2) + 1);
                    array [charNum] = newChar;
                    charNum++;
                    playersAlive++;
                    array [charNum] = charNULL;
                }
                
            break;

            case ('H'):

                if (charAttack(charNum, array, buffer, &playersAlive) == -1)
                    printf("\nMinimum 2 players need to be added to unlock this command. Please add at least one more player.\n");

            break;

            case ('L'):

                if (print_character(charNum, array, playersAlive) == -1)
                    printf("\nNo added characters yet. Please add characters before printing.\n");

            break;

            case ('W'):

                if (saveChars(buffer, array, charNum, filename) == -1)
                    fprintf(stderr, "\nUnable to save the characters. Please try again.\n");

            break;

            case ('O'):

                if ((reloadArray = reloadChars(array, newChar, &charNum, filename, charNULL, &playersAlive)) == NULL)
                    printf("\nError opening file. Please try again.\n");
                if (reloadArray != NULL)  {
                    array = reloadArray;
                    charNULL = array[charNum];
                }

            break;

            case ('Q'):

                memoryRelease(array, charNum, filename);   // quits the program no matter if memory was released correctly or not
                printf("\nThank you for playing! Until the next time gamer.\n\n");
                repeat = 0;

            break;
            
            default:

                printf("\nInvalid command! Please type command again.\n");

            break;
        }

    }

    return 0;
}


// Function 1 ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Adds a character with given name. The character has initially HP hit points, and the given weapon, with maximum damage as indicated in “weapondamage”.
// A Bilbo 25 Dagger 8

Character * add_character(char * buffer, Character * newChar, const int num, Character * charArray[]) {

    // allocate memory for character struct

    newChar = calloc(1, sizeof(Character));

    if (newChar == NULL) printf("\nMemory allocation failed for character struct!\n"); // memory allocation failure

    // allocate memory for character string

    newChar->c_name = calloc(30, sizeof(char));

    if (newChar->c_name == NULL) printf("\nMemory allocation failed for character string!\n"); // memory allocation failure 

    // allocate memory for weapon struct

    newChar->c_weapon = calloc(1, sizeof(Weapon));
    
    if (newChar->c_weapon == NULL) printf("\nMemory allocation failed for weapon struct!\n"); // memory allocation failure

    // allocate memory for weapon string

    newChar->c_weapon->weaponName = calloc(30, sizeof(char));

    if (newChar->c_weapon->weaponName == NULL) printf("\nMemory allocation failed for weapon string!\n"); // memory allocation failure

    // set hit points to zero

    newChar->hitPoints = 0;

    // set experience points to zero

    newChar->experiencePoints = 0;

    // add user input to variables

    if (sscanf(buffer, "A %s %d %s %d", newChar->c_name, &(newChar->hitPoints), newChar->c_weapon->weaponName, &(newChar->c_weapon->weaponDamage)) < 4) {

        printf("\nInvalid add command: %s\nPlease try again.\n", buffer);

        free(newChar->c_weapon->weaponName);

        free(newChar->c_weapon);

        free(newChar->c_name);

        free(newChar);

        return 0;

    } else if (1) {

        for (int i = 0; i < num; i++) {

            if (strcmp(charArray[i]->c_name, newChar->c_name) == 0) {

                printf("\nThis character name already exists, please choose another one.\n");

                free(newChar->c_weapon->weaponName);

                free(newChar->c_weapon);

                free(newChar->c_name);

                free(newChar);

                return 0;
            } 
        }
    } 

    return newChar;
}



// Function 2 ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Character “attacker” will attack towards character “target”. As the result target’s hit points will be decremented by the damage caused by the weapon.
// H Bilbo Smeagol.

int charAttack(const int num, Character * charArray[], char * buffer, int * playersAlive) {

    printf("\n--------------------------------------------------------------------------------------------------------------\n");

    if (num == 1) {

        return -1;

    }

    char * attacker;    // stores user input for attacker

    char * target;      // stores user input for target

    attacker = calloc(20, sizeof (char));

    target = calloc(20, sizeof (char));
    
    if (sscanf(buffer, "H %s %s", attacker, target) < 2) {

        printf("\nInvalid add command: %s\n", buffer);
    }

    else 

        printf("\nNow it's time to battle! ---->\n\n");

    int damageGiven;    

    int gainedExp;  // experience points are damage given / 2

    int j = 0;    // attacker iteration
    int k = 0;    // target iteration

    int countAtt = 0;   // count attacker iteration
    int countTar = 0;   // count target iteration

    int ok = 1;


    // if attacker found -> ready to attack

    while (ok) {

        for ( ; j < num; j++) {

            if (strcmp(charArray[j]->c_name, attacker) != 0) {

                countAtt++;
                continue;

            } else if ((strcmp(charArray[j]->c_name, attacker) == 0) && (charArray[j]->hitPoints != 0) && (strcmp(target, attacker) != 0)) {
                
                printf("\n%s is ready to attack with the %s\n", charArray[j]->c_name, charArray[j]->c_weapon->weaponName);
                break;
            
            } else {

                break;
            }
        }

        if (countAtt == num) {

            printf("\nAttacker not found! Please type command again.\n");
            break;

        } else if (charArray[j]->hitPoints == 0) {

            printf("\n%s is dead! Please choose another character to attack with.\n", charArray[j]->c_name);
            break;

        } else if ((strcmp(target, attacker) == 0)) {

            printf("\nCommiting suicide not allowed! Please choose an opponent.\n");
            break;

        }


        // if target found, battle continues

        for ( ; k < num; k++) {

            if (strcmp(charArray[k]->c_name, target) != 0) {

                countTar++;
                continue;

            } else if ((strcmp(charArray[k]->c_name, target) == 0) && (charArray[k]->hitPoints != 0)) {
                
                printf ("\n%s's %s is moving towards %s\n", charArray[j]->c_name, charArray[j]->c_weapon->weaponName, charArray[k]->c_name);
                break;
            
            } else {

                break;

            }
        }

        if (countTar == num) {

            printf("\nTarget not found! Please type command again.\n");

            break;   

        } else if (charArray[k]->hitPoints == 0) {

            printf("\n%s is already dead! There is no use to attack %s again. Please choose another target.\n", charArray[k]->c_name, charArray[k]->c_name);

            break;
        }


        printf("\n*** Pum! Pum! Pum! ***\n");


        damageGiven = charArray[j]->c_weapon->weaponDamage / ((rand() % 2) + 1);

        
        printf("\n%s shot inflicted %d damage to %s\n", charArray[j]->c_weapon->weaponName, damageGiven, charArray[k]->c_name);


        gainedExp = damageGiven / 2;


        charArray[j]->experiencePoints += gainedExp;

        charArray[k]->hitPoints -= damageGiven;


        printf("\n-> %s gained %d experience points\n", charArray[j]->c_name, gainedExp);

        printf("\n-> %s lost %d of hitpoints\n", charArray[k]->c_name, damageGiven);

        if (charArray[k]->hitPoints < 0 || charArray[k]->hitPoints == 0) {
            charArray[k]->hitPoints = 0;
            (*playersAlive)--;
            printf("\n%s just died! %d players remaining alive.\n", charArray[k]->c_name, *playersAlive);
        }

        printf("\n%s has %d of hitpoints remaining\n", charArray[k]->c_name, charArray[k]->hitPoints); 

        ok = 0;

    }
    
    free(attacker);     // frees memory for attacker which is allocated at the beginning of this function
    free(target);       // frees memory for target which is allocated at the beginning of this function

    return 0;
}



// Function 3 ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Prints all characters added to the game, each on their own line. For each character all the above described properties must be printed (name, HP, Exp, Weapon).
// The characters should be ordered by their experience points: the chactacter with most experience points will be printed first. However, the dead characters should be printed at the end of the list, regardless of their experience points.

int print_character(const int num, Character * charArray[], const int playersAlive) {

    if (num == 0) {

        return -1;
        
    }

    printf("\nList of added characters:\n");


    // sort by experience points
    
    for (int i = 0; i < num; i++) {

            int biggest_idx = i;

            for (int j = i; j < num; j++) {
                if (charArray[j]->experiencePoints > charArray[biggest_idx]->experiencePoints)
                    biggest_idx = j;
            }

            void * temp = charArray[i];   // stores current value
            charArray[i] = charArray[biggest_idx];   // stores smallest value to current
            charArray[biggest_idx] = temp;    // stores current value to smallest value's index
        }


    // sort alphabetically

    for (int i = 0; i < num; i++) {

        for (int j = i + 1; j < num; j++)
                
            if ((strcmp(charArray[i]->c_name, charArray[j]->c_name) > 0)  && (charArray[i]->experiencePoints == charArray[j]->experiencePoints)) {

                void * temp = charArray[i]; 
                charArray[i] = charArray[j]; 
                charArray[j] = temp; 
            }
    }


    // dead character (hp = 0) to last in the array

    int k = 0, b = 0, c = 0;

    void * temp2 = NULL;
    
    for ( ; b < num; b++) {

        while (charArray[k]->hitPoints != 0) {
            k++;
        }

        temp2 = charArray[k];

        c = k + 1;

        if (charArray[k]->hitPoints == 0 && k < num - 1) {

            for ( ; k < num - 1; k++) {
                
                charArray[k] = charArray[c];
                c++;
            }

            charArray[num - 1] = temp2;
        }

        else
            break;
        
        k = 0;
    }


    // sorts dead characters by experience points

    int playersDead = num - playersAlive;

    int d = playersAlive;

    if (playersDead > 1) {

        for ( ; d < num; d++) {

            int biggest_idx = d;

            for (int j = d; j < num; j++) {

                if (charArray[j]->experiencePoints > charArray[biggest_idx]->experiencePoints)
                    biggest_idx = j;
            }

            void * temp3 = charArray[d];   // stores current value
            charArray[d] = charArray[biggest_idx];   // stores smallest value to current
            charArray[biggest_idx] = temp3;    // stores current value to smallest value's index
        }
    }
    

    // print list

    int n = 0;

    while (charArray[n]->c_name[0] != '\0') {
        printf("Name: %-10s\tWeapon: %-10s\tExp: %-5d\tHP: %-5d\n", charArray[n]->c_name, charArray[n]->c_weapon->weaponName, charArray[n]->experiencePoints, charArray[n]->hitPoints);
        n++;
    }

    return 0;
}



// Function 4 ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Saves all characters in the game (including the dead ones) to file named “filename”.

int saveChars(char * buffer, Character * charArray[], const int num, char * filename) {

    if (sscanf(buffer, "W %s", filename) < 1) {

        printf("\nInvalid add command: %s\n", buffer);

        return 0;
    }

    FILE *file = fopen(filename,"w+");
    if (!file) {

        return -1;
        
    } else {

        for (int i = 0; i < num; i++) {
            if (fputc(65, file) == EOF)
                return -1;
            if (fputc(32, file) == EOF)
                return -1;    
            if (fputs(charArray[i]->c_name, file) == EOF)
                return -1;
            if (fputc(32, file) == EOF)
                return -1;
            if (fprintf(file, "%d", charArray[i]->hitPoints) == EOF)
                return -1;
            if (fputc(32, file) == EOF)
                return -1;
            if (fputs(charArray[i]->c_weapon->weaponName, file) == EOF)
                return -1;
            if (fputc(32, file) == EOF)
                return -1;
            if (fprintf(file, "%d", charArray[i]->c_weapon->weaponDamage) == EOF)
                return -1;
            if (fputc(10, file) == EOF)
                return -1;
        }

        printf("\nCharacters saved successfully to %s.\n", filename);

        fclose(file);


        return 0;
    }

    return 0;
}



// Function 5 ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Loads characters from file name “filename”, replacing the current setup in memory.

Character * * reloadChars(Character * charArray[], Character * newChar, int * num, char * filename, Character * charNULL, int * playersAlive) {

    Character * (* newArray) = NULL;     // array that stores return values of add_character function
    
    char * filefile = NULL;     // stringcopies filename for printing purposes below of this function

    filefile = calloc (20, sizeof(char));

    FILE *file = fopen(filename, "r");
    if (!file) {

        free(filefile);

        return 0;

    } else {

        for (int i = 0; i <= (*num); i++) {
            
            free(charArray[i]->c_weapon->weaponName);

            free(charArray[i]->c_weapon);

            free(charArray[i]->c_name);

            free(charArray[i]);

        }

        free(charArray);

        newArray = calloc (1, sizeof(Character *));

        *num = 0;
        *playersAlive = 0;

        strcpy (filefile, filename);

        char input [80];

        charNULL = calloc(1, sizeof(Character));

        charNULL->c_name = calloc(2, sizeof(char));

        charNULL->c_weapon = calloc(1, sizeof(Weapon));

        charNULL->c_weapon->weaponName = calloc(2, sizeof(char));

        while (fgets(input, 80, file)) {

            newChar = add_character(input, newChar, *num, newArray);
            newArray = realloc(newArray, sizeof(Character *) * ((*num) + 2) + 1);
            newArray[*num] = newChar;
            newArray[*num]->hitPoints = 100;
            (*num)++;
            (*playersAlive)++;
            newArray[*num] = charNULL;
        }
    }

    printf("\nCharacters loaded successfully from %s. There are now %d characters in the game.\n", filefile, *num);
    free(filefile);
    fclose(file);

    return newArray;    
}



// Function 6 ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Quits program and releases all allocated memory. This function must be implemented in order to detect possible memory leaks.

void memoryRelease(Character * charArray[], const int num, char * filename) {

    for (int i = 0; i <= num; i++) {

        free(charArray[i]->c_weapon->weaponName);

        free(charArray[i]->c_weapon);

        free(charArray[i]->c_name);

        free(charArray[i]);
    }

    free(charArray);
    
    free(filename);    // frees memory for filename = calloc (30, sizeof(char)) which is allocated in int main

    return;
}