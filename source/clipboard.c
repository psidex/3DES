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

// If file with same name already exists, don't copy,
// otherwise, try to copy to cwd [Currently not working]
void paste(void) {
    // Works, but very experimental, not fully tested
    consoleSelect(&debugscreen);
    FILE *fp1;
    FILE *fp2;
    char a;
    char paste_path[MAX_PATH_SIZE];
    int file_iter_count = 0;
    int size;

    strncpy(paste_path, current_path, MAX_PATH_SIZE);
    strcat(paste_path, clipboard.filename);

    // DEBUG
    // printf("PASTE\n");
    // printf("PATH in cb: %s\n", clipboard.path);
    // printf("NAME in cb: %s\n", clipboard.filename);

    fp1 = fopen(clipboard.path, "r");

    if (fp1) {
        fp2 = fopen(paste_path, "w");
        fseek(fp1, 0, SEEK_END);
        size = ftell(fp1);
        fseek(fp1, 0, SEEK_SET);
        printf("Size of file: %i\nCopying...\n", size);

        // Have to use file_iter_count because apparently EOF
        // isn't a thing on 3DS
        while (file_iter_count < size) {
            a = fgetc(fp1);
            // printf("a: %x %c\n", a, a);
            fputc(a, fp2);
            file_iter_count++;
        }
        fclose(fp1);
        fclose(fp2);
        get_all_in_dir(current_path);
        printf("Finished copying\n");
    }

    else { printf("\x1b[32mCannot open file in clipboard\x1b[0m\n"); }
}
