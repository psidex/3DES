#include "draw.h"

// Print all 'things' in the file name array
void print_all_values_in_filear(void) {
    int max_files_to_print;

    if (size_of_file_array < MAX_FILES_ON_SCREEN) { max_files_to_print = size_of_file_array; }
    else { max_files_to_print = MAX_FILES_ON_SCREEN; }

    //consoleInit(GFX_TOP, &topScreen);
    consoleSelect(&topScreen);
    // Clear the screen (using escape sequence)
    printf("\x1b[0;0H");

    if (max_files_to_print > 0) {
        if (scroll > 0) { printf("/\\\n"); } // Print up arrow showing you can scroll up
        else { printf("    \n"); }           // Keep all the files in the same place on screen, erase an arrow if there was one

        int i;

        // Print directories first
        for (i=0; i<max_files_to_print; i++) {
            // If it is a dir
            if (!isfile_arr[i+scroll]) {
                // print as white text on black background (%-39.39s instead of %s to control min and max width)
                if (i == selected) { printf("\n <dir>  | \x1b[47;30m%-39.39s\x1b[0m", file_arr[i+scroll]); }
                // Else, just print it without highlight
                else { printf("\n <dir>  | %-39.39s", file_arr[i+scroll]); }
            }

            else {
                if (i == selected) { printf("\n <file> | \x1b[47;30m%-39.39s\x1b[0m", file_arr[i+scroll]); }
                else { printf("\n <file> | %-39.39s", file_arr[i+scroll]); }
            }
        }

        // If there are files below on screen
        if ( (size_of_file_array > MAX_FILES_ON_SCREEN) && (selected+scroll != size_of_file_array-1) ) { printf("\n\n\\/ "); }
        // Erase arrow if there was one
        else { printf("\n\n    "); }
    }
    else { printf("\n\n\t\t\x1b[47;30m- Folder is empty -\x1b[0m"); }
}
