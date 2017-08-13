#include "draw.h"

// Print all strings in the file name array and set highlighted line
void print_all_values_in_filear(int clr) {
  if (clr) {
    clearscrn();
  }

  int max_files_to_print;

  if (size_of_file_array < MAX_FILES_ON_SCREEN) {
    max_files_to_print = size_of_file_array;
  }
  else {
    max_files_to_print = MAX_FILES_ON_SCREEN;
  }

  consoleSelect(&topScreen);
  // Moves the cursor to the top left corner of the screen
  printf("\x1b[0;0H");

  if (max_files_to_print > 0) {
    if (scroll > 0) {
      // Print up arrow showing you can scroll up
      printf("/\\\n");
    }
    else {
      // Keep all the files in the same place on screen, erase an arrow if there was one
      printf("  \n");
    }

    int i;

    for (i=0; i<max_files_to_print; i++) {
      // If it is a dir
      if (!isfile_arr[i+scroll]) {
        // print as white text on black background (%-39.39s instead of %s to control min and max width)
        if (i == selected) {
          printf("\n D | \x1b[47;30m%-39.39s\x1b[0m", file_arr[i+scroll]);
        }
        // Else, just print it without highlight
        else {
          printf("\n D | %-39.39s", file_arr[i+scroll]);
        }
      }
      else {
        if (i == selected) {
          printf("\n F | \x1b[47;30m%-39.39s\x1b[0m", file_arr[i+scroll]);
        }
        else {
          printf("\n F | %-39.39s", file_arr[i+scroll]);
        }
      }
    }

    if ( (size_of_file_array > MAX_FILES_ON_SCREEN) && (selected+scroll != size_of_file_array-1) ) {
      // If there are files below on screen
      printf("\n\n\\/");
    }
    else {
      // Erase arrow if there was one
      printf("\n\n  ");
    }
  }
  else {
    printf("\n\n\t\t\x1b[47;30m- Folder is empty -\x1b[0m");
  }
}

// Clear the screen
void clearscrn(void) {
  int i;
  consoleSelect(&topScreen);
  // Set cursor to top left
  printf("\x1b[0;0H");
  // Fill the screen with blank spaces
  for (i=0; i<37; i++) { printf("%-40.40s", " "); }
  // Set cursor to top left (again)
  printf("\x1b[0;0H");
}

int delete_dialouge(void) {
  clearscrn();
  // Top screen already selected form clearscrn
  printf("\n\n\n\t\t\x1b[31mDelete %-35.35s\x1b[0m", file_arr[selected+scroll]);
  printf("\n\n\t\t[A] - Yes\n\t\t[B] - No");

  while (aptMainLoop()) {
    gspWaitForVBlank();
    hidScanInput();
    u32 exitkDown = hidKeysDown();
    if (exitkDown & KEY_A) {
      return 0;
    }
    else if (exitkDown & KEY_B) {
      return 1;
    }
    gfxFlushBuffers();
    gfxSwapBuffers();
  }
  // If something goes wrong (+ stops compile warning)
  return 1;
}
