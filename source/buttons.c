#include "buttons.h"
#include "dir.h"
#include "draw.h"
#include "delete.h"
#include "hex.h"

void up(void) {
    if (size_of_file_array == 0){ ; }

    // if selected is 0 and there is no scroll, skip to the bottom
    else if (selected+scroll == 0) {
        // If there is a need for scroll
        if (size_of_file_array > MAX_FILES_ON_SCREEN) {
            // arrays are 0 indexed, so max files -1 = highest index of array shown
            selected = MAX_FILES_ON_SCREEN-1;
            // scroll will be max size it can be
            scroll = size_of_file_array-MAX_FILES_ON_SCREEN;
        }
        else { selected = size_of_file_array-1; }
    }

    else if (scroll > 0) { scroll--; }

    else { selected--; }
}

void down(void) {
    if (size_of_file_array == 0){ ; }

    // If selected+scroll are at the largest index of the file array (arrays are 0 indexed!)
    else if (selected+scroll == size_of_file_array-1) {
        selected = 0;
        scroll = 0;
    }

    // If selected = highest index and selected+scroll < the max index for the array
    else if ((selected == MAX_FILES_ON_SCREEN-1) && (selected+scroll < size_of_file_array-1)) { scroll++; }

    else { selected++; }
}

void left(void) {
    if (size_of_file_array == 0){ ; }
    // If selected+scroll are at the largest index of the file array (arrays are 0 indexed!)
    else {
        selected = 0;
        scroll = 0;
    }
}

void right(void){
    if (size_of_file_array == 0){ ; }
    else {
        // If there is a need for scroll
        if (size_of_file_array > MAX_FILES_ON_SCREEN) {
            // arrays are 0 indexed, so max files -1 = highest index of array shown
            selected = MAX_FILES_ON_SCREEN-1;
            // scroll will be max size it can be
            scroll = size_of_file_array-MAX_FILES_ON_SCREEN;
        }
        else { selected = size_of_file_array-1; }
    }
}

void a_pressed(void) {
    if (size_of_file_array == 0){ ; }

    else {
        consoleSelect(&topScreen);
        clearscrn();

        consoleSelect(&debugscreen);
        char temp_path[MAX_PATH_SIZE];
        strcpy(temp_path, current_path);
        strcat(temp_path, file_arr[selected+scroll]);

        // If it is actually a directory
        if (!isfile_arr[selected+scroll]) {
            printf("path is dir\n");
            strcat(temp_path, "/");
            strcpy(current_path, temp_path);
            printf("\x1b[32mnew path: %s\x1b[0m\n", current_path);
            get_all_in_dir(current_path);
            print_all_values_in_filear();
        }

        else {
            printf("path is file\n");
            start_hex(temp_path);
            print_all_values_in_filear();
        }
    }
}

void b_pressed(void) {
    consoleSelect(&debugscreen);

    if (!strcmp(current_path, "sdmc:/")) { printf("\x1b[32mcurrently in sdmc:/\x1b[0m\n"); }

    // move up a directory
    else { get_ud(); }

    get_all_in_dir(current_path);
    consoleSelect(&topScreen);
    clearscrn();
    print_all_values_in_filear();
}

void l_pressed(void) {
    consoleSelect(&debugscreen);
    printf("Bringing up keyboard...\n");

    SwkbdState swkbd;
    char newdirname[260];
    SwkbdButton button = SWKBD_BUTTON_NONE;

    swkbdInit(&swkbd, SWKBD_TYPE_WESTERN, 1, 260);
    swkbdSetHintText(&swkbd, "New directory name here");
    button = swkbdInputText(&swkbd, newdirname, sizeof(newdirname));

    // Create the path to be test/created
    char path_to_create[MAX_PATH_SIZE];
    strcpy(path_to_create, current_path);
    strcat(path_to_create, newdirname);

    printf("\x1b[32mpath to create: %s\x1b[0m\n", path_to_create);

    int result;

    // will fail is path already exists
    result = mkdir(path_to_create, 0700);

    if (result == 0) {
        printf("Dir created\n");
    }
    else {
        printf("\x1b[31mError: Directory creation failed\x1b[0m\n");
    }

    get_all_in_dir(current_path);
    consoleSelect(&topScreen);
    clearscrn();
    print_all_values_in_filear();
}

void r_pressed(void) {
    if (size_of_file_array == 0){ ; }

    else {
        consoleInit(GFX_TOP, &topScreen);
        consoleSelect(&topScreen);
        printf("\n\n\n\t\t\x1b[31mDelete %s?\x1b[0m", file_arr[selected+scroll]);
        printf("\n\n\t\t[A] - Yes\n\t\t[B] - No");

        while (1) {
            hidScanInput();
            u32 exitkDown = hidKeysDown();
            if (exitkDown & KEY_A) {
                delete_selected();
                break;
            }
            else if (exitkDown & KEY_B) { break; }
        }
        get_all_in_dir(current_path);
        consoleSelect(&topScreen);
        printf("\x1b[2J");
        print_all_values_in_filear();
    }
}
