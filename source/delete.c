#include "delete.h"

void delete_dir_recursivley_(char path_to_delete[MAX_PATH_SIZE]) {
  consoleSelect(&debugscreen);

  DIR *d;
  d = opendir(path_to_delete);
  int ret;

  if (d) {
    struct dirent *dir;
    char new_path_to_delete[MAX_PATH_SIZE];

    while ((dir = readdir(d)) != NULL) {
      strncpy(new_path_to_delete, path_to_delete, MAX_PATH_SIZE);
      strcat(new_path_to_delete, "/");
      strcat(new_path_to_delete, dir->d_name);
      if (dir->d_type == 8) {
        ret = remove(new_path_to_delete);
        if (ret) {
          printf("\x1b[41mError: unable to delete file %s\x1b[0m\n", dir->d_name);
        }
        else {
          printf("\x1b[35mDeleted file: %s\x1b[0m\n", dir->d_name);
        }
      }
      else {
        delete_dir_recursivley_(new_path_to_delete);
      }
    }
    closedir(d);
    ret = rmdir(path_to_delete);
    if (ret) {
      printf("\x1b[41mError: unable to delete dir\x1b[0m\n");
    }
  }
  else {
    // Dir doesen't exist?
    closedir(d);
  }
}

void delete_selected(void) {
  consoleSelect(&debugscreen);

  char filepath[MAX_PATH_SIZE];
  strncpy(filepath, current_path, MAX_PATH_SIZE);
  strcat(filepath, file_arr[selected+scroll]);

  int ret;

  // If it is a dir
  if (!isfile_arr[selected+scroll]) {
    ret = rmdir(filepath);
    if (!ret) {
      printf("\x1b[35m%s deleted\x1b[0m\n", file_arr[selected+scroll]);
    }
    else {
      delete_dir_recursivley_(filepath);
    }
  }
  else {
    ret = remove(filepath);
    if (!ret) {
      printf("\x1b[35m%s deleted\x1b[0m\n", file_arr[selected+scroll]);
    }
    else {
      printf("\x1b[41mUnable to delete %s\x1b[0m\n", file_arr[selected+scroll]);
    }
  }
}
