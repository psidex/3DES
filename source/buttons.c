#include "buttons.h"
#include "dir.h"
#include "draw.h"
#include "delete.h"

void up(void) {
  if (size_of_file_array != 0) {
    if (selected+scroll == 0) {
      if (size_of_file_array > MAX_FILES_ON_SCREEN) {
        selected = MAX_FILES_ON_SCREEN-1;
        // scroll will be max size it can be
        scroll = size_of_file_array-MAX_FILES_ON_SCREEN;
      }
      else {
        selected = size_of_file_array-1;
      }
    }

    else if (scroll > 0) {
      scroll--;
    }

    else {
      selected--;
    }
  }
}

void down(void) {
  if (size_of_file_array != 0) {
    if (selected+scroll == size_of_file_array-1) {
      selected = 0;
      scroll = 0;
    }

    else if ((selected == MAX_FILES_ON_SCREEN-1) && (selected+scroll < size_of_file_array-1)) {
      scroll++;
    }

    else {
      selected++;
    }
  }
}

void left(void) {
  if (size_of_file_array != 0) {
    selected = 0;
    scroll = 0;
  }
}

void right(void) {
  if (size_of_file_array != 0) {
    if (size_of_file_array > MAX_FILES_ON_SCREEN) {
      selected = MAX_FILES_ON_SCREEN-1;
      scroll = size_of_file_array-MAX_FILES_ON_SCREEN;
    }
    else {
      selected = size_of_file_array-1;
    }
  }
}

void a_pressed(void) {
  if (size_of_file_array != 0) {
    // If it is actually a directory
    if (!isfile_arr[selected+scroll]) {
      strcat(current_path, file_arr[selected+scroll]);
      strcat(current_path, "/");

      consoleSelect(&debugscreen);
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
  if (!strcmp(current_path, "sdmc:/")) {
    consoleSelect(&debugscreen);
    printf("\x1b[32mcurrently in sdmc:/\x1b[0m\n");
  }
  else {
    // move up a directory
    get_ud();
    get_all_in_dir(current_path);
    print_all_values_in_filear();
  }
}

void l_pressed(void) {
  consoleSelect(&debugscreen);
  printf("\x1b[35mBringing up keyboard...\x1b[0m\n");

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

  if (!result) {
    printf("\x1b[35mDir created\x1b[0m\n");
  }
  else {
    printf("\x1b[41mError: Directory creation failed\x1b[0m\n");
  }

  get_all_in_dir(current_path);
  print_all_values_in_filear();
}

void r_pressed(void) {
  if (size_of_file_array != 0) {
    int ret = delete_dialouge();
    if (!ret) {
      delete_selected();
      get_all_in_dir(current_path);
    }
    print_all_values_in_filear();
  }
}
