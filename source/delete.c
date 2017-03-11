#include "delete.h"

void delete_selected(void) {
    consoleSelect(&debugscreen);

    char filepath[MAX_PATH_SIZE];
    strcpy(filepath, current_path);
    strcat(filepath, file_arr[selected+scroll]);

    int ret;

    // If it is a dir
    if (!isfile_arr[selected+scroll]) {
        ret = rmdir(filepath);
        if(!ret) { printf("Directory deleted\n"); }
        else { printf("Deleting non-empty dirs not supported\n"); }
    }

    else {
        ret = remove(filepath);
        if(!ret) { printf("File deleted\n"); }
        else { printf("\x1b[31mError: unable to delete\x1b[0m\n"); }
    }
}
