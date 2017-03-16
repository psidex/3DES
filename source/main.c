#include "common.h"
#include "dir.h"
#include "draw.h"
#include "buttons.h"

// contains current path
char current_path[511];
// selected file
int selected = 0;
// A pointer to an array which is full of pointers which will point to strings (only way to create a dynamic/resizable array)
char **file_arr;
// Same as file_arr, will be used "alongside" file_arr to check whether something is a file or directory
bool *isfile_arr;
// Size of file name array
int size_of_file_array;
// Used to offset what is printed from the file array, to allow "scrolling"
int scroll = 0;
// One PrintConsole for each screen
PrintConsole topScreen, instructionscreen, debugscreen;
// Max len of file/directory name
int MAX_DIR_NAME_SIZE = 261;
// What it says on the tin
int MAX_FILES_ON_SCREEN = 26;
// ^
int MAX_PATH_SIZE = 511;

int main(int argc, char **argv) {

    gfxInitDefault();

    // Initialize console for both screens using the two different PrintConsole
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &debugscreen);
    consoleInit(GFX_BOTTOM, &instructionscreen);

    consoleSetWindow(&instructionscreen, 0, 0, 40, 8);
    consoleSetWindow(&debugscreen, 0, 9, 40, 21);

    consoleSelect(&instructionscreen);
    printf("A - Change Directory\nB - go up a directory\nX - CD to / and reallocate mem\nL - Create a new dir\nR - Delete dir/file\nDPAD/Circle pad - up and down\nSTART - close app");
    printf("\n----------------------------------------");

    consoleSelect(&debugscreen);
    printf("Started...\n");

    // Initial allocation for these 2 arrays
    file_arr = malloc(1 * sizeof(char*));
    isfile_arr = malloc(1 * sizeof(bool));

    // For when it is first realloc(ed)
    size_of_file_array = 1;

    strcpy(current_path, "sdmc:/");

    get_all_in_dir(current_path);      // Fill file name array with file names

    print_all_values_in_filear();      // Print all in /

    // Main loop
    while (aptMainLoop()) {

        gspWaitForVBlank();
        hidScanInput();

        /*
        consoleSelect(&topScreen);
        printf("\x1b[2J");

        Sets the cursor to the top left of the screen, meaning the screen does not have to be cleared
        (which means no flickering) and because all strings are forced to be the same length (in draw.c)
        re-drawing the whole thing does not leave any artefacts behind. The screen has to be cleared
        if the directory is changed, otherwise text will be left on screen.
        */

        u32 kDown = hidKeysDown();

        if (kDown & KEY_START) { break; }

        else if (kDown & KEY_UP) {
            up();
            print_all_values_in_filear();
        }

        else if (kDown & KEY_DOWN) {
            down();
            print_all_values_in_filear();
        }

        else if (kDown & KEY_LEFT) {
            left();
            print_all_values_in_filear();
        }

        else if (kDown & KEY_RIGHT) {
            right();
            print_all_values_in_filear();
        }

        else if (kDown & KEY_X) {
            consoleSelect(&debugscreen);
            printf("\x1b[34mKEY_X pressed\x1b[0m\n");
            strcpy(current_path, "sdmc:/");
            printf("\x1b[32mnew path: %s\x1b[0m\n", current_path);
            get_all_in_dir(current_path);
            consoleSelect(&topScreen);
            printf("\x1b[2J");
            print_all_values_in_filear();
        }

        else if (kDown & KEY_A) {
            consoleSelect(&topScreen);
            printf("\x1b[2J");
            a_pressed();
        }

        else if (kDown & KEY_B) {
            b_pressed();
            get_all_in_dir(current_path);
            consoleSelect(&topScreen);
            printf("\x1b[2J");
            print_all_values_in_filear();
        }

        else if (kDown & KEY_L ) {
            l_pressed();
            get_all_in_dir(current_path);
            consoleSelect(&topScreen);
            printf("\x1b[2J");
            print_all_values_in_filear();
        }

        else if (kDown & KEY_R ) {
            r_pressed();
            get_all_in_dir(current_path);
            consoleSelect(&topScreen);
            printf("\x1b[2J");
            print_all_values_in_filear();
        }

        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();
    }

    // Clear up allocated memory
    for (int i=0; i<size_of_file_array; i++) { free(file_arr[i]); }
    free(file_arr);
    free(isfile_arr);
    gfxExit();
    return 0;
}
