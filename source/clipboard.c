#include "clipboard.h"
#include "dir.h"

// Copy the currently selected path to clipboard
void copy_selected(void) {
    consoleSelect(&debugscreen);

    if (!isfile_arr[selected+scroll]) {
            printf("\x1b[32mCopying directories not supported\x1b[0m\n");
    }

    else {
        strncpy(clipboard.path, current_path, MAX_PATH_SIZE);
        strcat(clipboard.path, file_arr[selected+scroll]);
        strncpy(clipboard.filename, file_arr[selected+scroll], MAX_DIR_NAME_SIZE);
        printf("\x1b[32mPath copied to clipboard\x1b[0m\n");
    }
}

void paste(void) {
    consoleSelect(&debugscreen);
    printf("PASTE\n");
    printf("PATH in cb: %s\n", clipboard.path);
    printf("NAME in cb: %s\n", clipboard.filename);
    get_all_in_dir(current_path);
}
