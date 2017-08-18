#include "common.h"
#include "dir.h"
#include "draw.h"
#include "buttons.h"
#include "contextmenu.h"

char current_path[511];
int selected = 0;
int scroll   = 0;

file_entry *file_arr;
int size_of_file_array;

PrintConsole topScreen, instructionscreen, debugscreen;

bool quit_for_err = false;

int main(int argc, char **argv) {

  gfxInitDefault();

  // Initialize console for both screens using the 3 different PrintConsole
  consoleInit(GFX_TOP, &topScreen);
  consoleInit(GFX_BOTTOM, &debugscreen);
  consoleInit(GFX_BOTTOM, &instructionscreen);

  // Set position of these 2 windows (x, y, width, height)
  consoleSetWindow(&instructionscreen, 0, 0, 40, 9);
  consoleSetWindow(&debugscreen, 0, 10, 40, 20);

  consoleSelect(&instructionscreen);
  printf("A - cd / open context menu\nB - go up a directory\nX - CD to /\nY - Nothing\nL - Create a new dir\nR - Delete dir/file\nDPAD/Circle pad - up and down\nSTART - close app");
  printf("\n----------------------------------------");

  consoleSelect(&debugscreen);
  printf("Started...\n");

  // Initial allocation
  file_arr = malloc(1 * sizeof(file_entry));

  if (file_arr == NULL) {
    // Malloc failed, deal with it
    consoleSelect(&debugscreen);
    printf("%s!! MALLOC FAILED !!%s\n", BG_RED, RESET);
    quit_for_err = true;
  }

  // For when it is first realloc(ed)
  size_of_file_array = 1;

  strcpy(current_path, "sdmc:/");

  // Init context menu
  setup_ctm();

  // Fill file name array with file names
  get_all_in_dir(current_path);

  // Print all in root dir
  print_all_values_in_filear(1);

  // Main loop
  while (aptMainLoop()) {

    gspWaitForVBlank();
    hidScanInput();

    if (quit_for_err) {
      // ToDo: Add some sort of sleep here so user can read error messages
      break;
    }

    u32 kDown = hidKeysDown();

    if (kDown & KEY_START) {
      break;
    }

    else if (kDown & KEY_UP) {
      up();
      print_all_values_in_filear(0);
    }

    else if (kDown & KEY_DOWN) {
      down();
      print_all_values_in_filear(0);
    }

    else if (kDown & KEY_LEFT) {
      left();
      print_all_values_in_filear(0);
    }

    else if (kDown & KEY_RIGHT) {
      right();
      print_all_values_in_filear(0);
    }

    else if (kDown & KEY_X) {
      consoleSelect(&debugscreen);
      printf("%sKEY_X pressed%s\n", FG_BLUE, RESET);
      strcpy(current_path, "sdmc:/");
      printf("%snew path: %s%s\n", FG_GREEN, current_path, RESET);
      get_all_in_dir(current_path);
      print_all_values_in_filear(1);
    }

    else if (kDown & KEY_A) { a_pressed(); }
    else if (kDown & KEY_B) { b_pressed(); }
    else if (kDown & KEY_L) { l_pressed(); }
    else if (kDown & KEY_R) { r_pressed(); }

    // Flush and swap framebuffers
    gfxFlushBuffers();
    gfxSwapBuffers();
  }

  free(file_arr);
  gfxExit();
  return 0;
}
