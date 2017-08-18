#include "common.h"
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
          printf("%sError: unable to delete file %s%s\n", BG_RED, dir->d_name, RESET);
        }
        else {
          printf("%sDeleted file: %s%s\n", FG_MAGENTA, dir->d_name, RESET);
        }
      }
      else {
        delete_dir_recursivley_(new_path_to_delete);
      }
    }
    closedir(d);
    ret = rmdir(path_to_delete);
    if (ret) {
      printf("%sError: unable to delete dir%s\n", BG_RED, RESET);
    }
    else {
      printf("%sDeleted dir %s%s\n", FG_MAGENTA, path_to_delete, RESET);
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
  strcat(filepath, file_arr[selected+scroll].name);

  int ret;

  // If it is a dir
  if (!file_arr[selected+scroll].isfile) {
    ret = rmdir(filepath);
    if (!ret) {
      printf("%s%s deleted%s\n", FG_MAGENTA, file_arr[selected+scroll].name, RESET);
    }
    else {
      delete_dir_recursivley_(filepath);
    }
  }
  else {
    ret = remove(filepath);
    if (!ret) {
      printf("%s%s deleted%s\n", FG_MAGENTA, file_arr[selected+scroll].name, RESET);
    }
    else {
      printf("%sUnable to delete %s%s\n", BG_RED, file_arr[selected+scroll].name, RESET);
    }
  }
}
