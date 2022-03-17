#include <stdio.h>
#include "./db.c"
#include "./include/mysql.h"
#include "./menu.c"

char input[256] = {0};
char* getInput() {
    scanf("%s", input);
    return input;
};

int main() {
    initDB();
    Menu* menu = &topMenu;
    while (1) {
        menu->showMenu();

        string input = getInput();
        menu = menu->exec(input);
    }

    return EXIT_SUCCESS;
}