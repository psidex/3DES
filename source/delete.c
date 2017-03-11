#include "delete.h"

void delete_dir_recursivley(char path_to_delete[MAX_PATH_SIZE]) {
    consoleSelect(&debugscreen);

    DIR *d;
    d = opendir(path_to_delete);
    int ret;

    if (d) {
        struct dirent *dir;
        char new_path_to_delete[MAX_PATH_SIZE];

        while ((dir = readdir(d)) != NULL) {
            strcpy(new_path_to_delete, path_to_delete);
            strcat(new_path_to_delete, "/");
            strcat(new_path_to_delete, dir->d_name);

            if (dir->d_type == 8) {
                ret = remove(new_path_to_delete);
                if(ret) { printf("\x1b[31mError: unable to delete file\x1b[0m\n"); }
            }

            else {
                delete_dir_recursivley(new_path_to_delete);
            }
        }
    }
    closedir(d);
    ret = rmdir(path_to_delete);
    if(ret) { printf("\x1b[31mError: unable to delete dir\x1b[0m\n"); }
}

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
        else { delete_dir_recursivley(filepath); }
    }

    else {
        ret = remove(filepath);
        if(!ret) { printf("File deleted\n"); }
        else { printf("\x1b[31mError: unable to delete\x1b[0m\n"); }
    }
}
