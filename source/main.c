#include "common.h"
#include "dir.h"
#include "draw.h"
#include "buttons.h"

char current_path[511];
int selected = 0;
int scroll   = 0;

char **file_arr;
bool *isfile_arr;
int size_of_file_array;

PrintConsole topScreen, instructionscreen, debugscreen;

int MAX_DIR_NAME_SIZE   = 261;
int MAX_FILES_ON_SCREEN = 26;
int MAX_PATH_SIZE       = 511;

bool quit_for_err = false;

CB clipboard;

int main(int argc, char **argv) {

    gfxInitDefault();

    // Initialize console for both screens using the 3 different PrintConsole
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &debugscreen);
    consoleInit(GFX_BOTTOM, &instructionscreen);

    // Set position of these 2 windows
    consoleSetWindow(&instructionscreen, 0, 0, 40, 9);
    consoleSetWindow(&debugscreen, 0, 10, 40, 20);

    consoleSelect(&instructionscreen);
    printf("A - Change Directory\nB - go up a directory\nX - CD to / and reallocate mem\nY - Copy/paste file\nL - Create a new dir\nR - Delete dir/file\nDPAD/Circle pad - up and down\nSTART - close app");
    printf("\n----------------------------------------");

    consoleSelect(&debugscreen);
    printf("Started...\n");

    // Initial allocation for these 2 arrays
    file_arr = malloc(1 * sizeof(char*));
    isfile_arr = malloc(1 * sizeof(bool));

    if ((file_arr == NULL) || (isfile_arr == NULL) ) {
        // Malloc failed, deal with it
        consoleSelect(&debugscreen);
        printf("\x1b[31m!! MALLOC FAILED !!\x1b[0m\n");
        quit_for_err = true;
    }

    // For when it is first realloc(ed)
    size_of_file_array = 1;

    strcpy(current_path, "sdmc:/");

    // Fill file name array with file names
    get_all_in_dir(current_path);

    // Print all in root dir
    print_all_values_in_filear();

    // Main loop
    while (aptMainLoop()) {

        gspWaitForVBlank();
        hidScanInput();

        // TODO: Add a wait or something like that
        // so that the user sees the error message
        if (quit_for_err) { break; }

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
            clearscrn();
            print_all_values_in_filear();
        }

        else if (kDown & KEY_A) { a_pressed(); }
        else if (kDown & KEY_B) { b_pressed(); }
        else if (kDown & KEY_Y) { y_pressed(); }
        else if (kDown & KEY_L) { l_pressed(); }
        else if (kDown & KEY_R) { r_pressed(); }

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
