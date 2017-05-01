#include "buttons.h"
#include "dir.h"
#include "draw.h"
#include "delete.h"
#include "clipboard.h"

// TODO: Fix: Closing the lid while a new aptMainLoop
// loop is active causes the 3ds to not "wake up"

void up(void) {
    if (size_of_file_array == 0){ ; }

    // If selected is 0 and there is no scroll, skip to the bottom
    else if (selected+scroll == 0) {
        // If there is a need for scroll
        if (size_of_file_array > MAX_FILES_ON_SCREEN) {
            // Arrays are 0 indexed, so max files -1 = highest index of array shown
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
    else { selected = 0; scroll = 0; }
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
        // If it is actually a directory
        if (!isfile_arr[selected+scroll]) {
            consoleSelect(&topScreen);
            clearscrn();

            strcat(current_path, file_arr[selected+scroll]);
            strcat(current_path, "/");

            consoleSelect(&debugscreen);
            printf("path is dir\n");
            printf("\x1b[32mnew path: %s\x1b[0m\n", current_path);

            get_all_in_dir(current_path);
            print_all_values_in_filear();
        }

        else {
            consoleSelect(&debugscreen);
            printf("path is file\n");
        }
    }
}

void b_pressed(void) {
    consoleSelect(&debugscreen);

    if (!strcmp(current_path, "sdmc:/")) { printf("\x1b[32mcurrently in sdmc:/\x1b[0m\n"); }

    // move up a directory
    else {
        get_ud();
        get_all_in_dir(current_path);
        clearscrn();
        print_all_values_in_filear();
    }
}

void y_pressed(void) {
    consoleSelect(&debugscreen);

    if (size_of_file_array == 0){ printf("\x1b[32mCannot copy from empty dir\x1b[0m\n"); }

    else if (strcmp(clipboard.filename, "")) {
        consoleSelect(&topScreen);
        clearscrn();
        printf("\n\n\n\t\tCopy or paste?");
        printf("\n\n\t\t[A] - Copy\n\t\t[X] - Paste\n\t\t[B] - Cancel");

        // TODO: Find correct numbers for the %-25.25s
        printf("\n\n\n\t\tName of file in clipboard:\n\n\t\t%-25.25s", clipboard.filename);

        while (aptMainLoop()) {
            hidScanInput();
            u32 exitkDown = hidKeysDown();

            if (exitkDown & KEY_A) {
                copy_selected();
                break;
            }
            else if (exitkDown & KEY_X) {
                paste();
                break;
            }
            else if (exitkDown & KEY_B) {
                break;
            }
        }

        // Paste has get_all_in_dir() in it
        consoleSelect(&topScreen);
        clearscrn();
        print_all_values_in_filear();
    }

    // Nothing in clipboard so you can only copy into it
    else { copy_selected(); }
}

void l_pressed(void) {
    consoleSelect(&debugscreen);
    printf("Bringing up keyboard...\n");

    SwkbdState swkbd;
    char newdirname[MAX_DIR_NAME_SIZE];

    swkbdInit(&swkbd, SWKBD_TYPE_WESTERN, 1, 260);
    swkbdSetHintText(&swkbd, "New directory name here");
    swkbdInputText(&swkbd, newdirname, sizeof(newdirname));

    // Create the path to be test/created
    char path_to_create[MAX_PATH_SIZE];
    strncpy(path_to_create, current_path, MAX_PATH_SIZE);
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
        clearscrn();
        consoleSelect(&topScreen);
        printf("\n\n\n\t\t\x1b[31mDelete %-35.35s\x1b[0m", file_arr[selected+scroll]);
        printf("\n\n\t\t[A] - Yes\n\t\t[B] - No");

        while (aptMainLoop()) {
            hidScanInput();
            u32 exitkDown = hidKeysDown();
            if (exitkDown & KEY_A) {
                delete_selected();
                get_all_in_dir(current_path);
                break;
            }
            else if (exitkDown & KEY_B) { break; }
        }

        consoleSelect(&topScreen);
        clearscrn();
        print_all_values_in_filear();
    }
}
