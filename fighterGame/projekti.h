
typedef struct Weapon {

    unsigned int weaponDamage;
    char * weaponName;

} Weapon;


typedef struct Character {

    char * c_name;
    int hitPoints;
    unsigned int experiencePoints;
    Weapon * c_weapon;

} Character;




Character * add_character (char * buffer, Character * newChar, const int num, Character * charArray[]);    // adds a character to the game

int charAttack (const int num, Character * charArray[], char * buffer, int * playersAlive);     // attack target

int print_character (const int num, Character * charArray[], const int playersAlive);   // prints all added characters after battles sorted according to experience points

int saveChars (char * buffer, Character * charArray[], const int num, char * filename);    // saves all characters to opened file

Character * * reloadChars (Character * charArray[], Character * newChar, int * num, char * filename, Character * charNULL, int * playersAlive);    // loads file and replaces the current setup in memory

void memoryRelease (Character * charArray[], const int num, char * filename);  // releases dynamically allocated memory