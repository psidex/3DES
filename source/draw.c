#include "common.h"
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
  printf(RESET_TO_TOP_LEFT);

  if (strlen(current_path) <= 50) {
    printf("%s%s%s\n", BLACK_ON_WHITE, current_path, RESET);
  } else {
    printf("%s%s%s", BLACK_ON_WHITE, current_path+strlen(current_path)-50, RESET);
  }

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
      if (!file_arr[i].isfile) {
        // print as white text on black background (%-39.39s instead of %s to control min and max width)
        if (i == selected) {
          printf("\n D | %s%-39.39s%s", BLACK_ON_WHITE, file_arr[i+scroll].name, RESET);
        }
        // Else, just print it without highlight
        else {
          printf("\n D | %-39.39s", file_arr[i+scroll].name);
        }
      }
      else {
        if (i == selected) {
          printf("\n F | %s%-39.39s%s", BLACK_ON_WHITE, file_arr[i+scroll].name, RESET);
        }
        else {
          printf("\n F | %-39.39s", file_arr[i+scroll].name);
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
    printf("\n\n\t\t%s- Folder is empty -%s", BLACK_ON_WHITE, RESET);
  }
}

// Clear the screen
void clearscrn(void) {
  int i;
  consoleSelect(&topScreen);
  // Set cursor to top left
  printf(RESET_TO_TOP_LEFT);
  // Fill the screen with blank spaces
  for (i=0; i<37; i++) { printf("%-40.40s", " "); }
  // Set cursor to top left (again)
  printf(RESET_TO_TOP_LEFT);
}

int delete_dialouge(void) {
  clearscrn();
  // Top screen already selected form clearscrn
  printf("\n\n\n\t\t%sDelete %-35.35s%s", FG_RED, file_arr[selected+scroll].name, RESET);
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
