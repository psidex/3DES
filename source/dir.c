#include "common.h"
#include "dir.h"
#include "sort.h"

// Sets current_path to its upper directory
void get_ud(void) {
  consoleSelect(&debugscreen);

  char path_to_iterate[MAX_PATH_SIZE];
  strncpy(path_to_iterate, current_path, MAX_PATH_SIZE);

  char looking_for[] = "/";
  char *token;
  char dummy1[MAX_PATH_SIZE];
  char dummy2[MAX_PATH_SIZE];

  // ToDo: Check this out
  // Sometimes there is random stuff in here for some reason, so clean both before starting
  strcpy(dummy1, "");
  strcpy(dummy2, "");

  token = strtok(path_to_iterate, looking_for);

  while( token != NULL ) {
    strcat(dummy1, token);
    strcat(dummy1, "/");

    // If dummy1 has been fully constructed into current_path
    if (!strcmp(dummy1, current_path)) {
      // dummy2 happens after this, so will have 1 less token
      strncpy(current_path, dummy2, MAX_PATH_SIZE);
      break;
    }

    strcat(dummy2, token);
    strcat(dummy2, "/");

    // get the next token
    token = strtok(NULL, looking_for);
  }
  printf("\x1b[32mnew path: %s\x1b[0m\n", current_path);
}

// Fills file array with all files in a given directory
void get_all_in_dir(char dir_to_show[]) {
  consoleSelect(&debugscreen);

  // 2 of each for 2 iterations
  DIR *d;
  DIR *nd;
  d = opendir(dir_to_show);
  nd = opendir(dir_to_show);

  if (d) {
    // Get rid of strings in file name array
    for (int i = 0; i < size_of_file_array; i++) {
      free(file_arr[i]);
    }

    struct dirent *dir;
    struct dirent *ndir;
    int count = 0;
    selected  = 0;
    scroll    = 0;

    // While readdir returns something other than NULL. The variable dir will change each loop
    while ((dir = readdir(d)) != NULL) {
      // Count how many files there are
      count++;
    }

    size_of_file_array = count;
    count = 0;

    // Create a 2D array of char pointers the size of the amount of files in the chosen directory
    file_arr = realloc(file_arr, (size_of_file_array+1) * sizeof(char*));

    // Set each pointer inside the array to point to a "string"
    for (int i = 0; i < size_of_file_array; i++) {
      file_arr[i] = malloc(MAX_DIR_NAME_SIZE * sizeof(char));
    }

    // Do the same with isfile_arr, except with bools
    isfile_arr = realloc(isfile_arr, (size_of_file_array+1) * sizeof(bool));

    if ((file_arr == NULL) || (isfile_arr == NULL) ) {
      // Malloc failed, deal with it
      printf("%s!! MALLOC FAILED !!%s\n", BG_RED, RESET);
      quit_for_err = true;
    }

    else {
      // Iterate over dir again, this time adding filenames to created 2D array
      while ((ndir = readdir(nd)) != NULL) {
        // Get d_name from the dir struct and copy into array
        strncpy(file_arr[count], ndir->d_name, MAX_DIR_NAME_SIZE);
        // If d_type is a file
        isfile_arr[count] = (ndir->d_type == 8);
        count++;
      }
      bubble_sort_files();
    }
  }
  closedir(d);
  closedir(nd);
}
