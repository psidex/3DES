#include "clipboard.h"

// Copy the currently selected path to clipboard
void copy_selected(void) {
    consoleSelect(&debugscreen);
    strncpy(clipboard.path, current_path, MAX_PATH_SIZE);
    strcat(clipboard.path, file_arr[selected+scroll]);
    strncpy(clipboard.filename, file_arr[selected+scroll], MAX_DIR_NAME_SIZE);
    printf("\x1b[32mPath copied to clipboard\x1b[0m\n");
}

void paste(void) {
    consoleSelect(&debugscreen);
    printf("paste here\n");
}
