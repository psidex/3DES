#include "common.h"
#include "fs.h"
#include "draw.h"
#include "btn.h"
#include "ctm.h"
#include "err.h"

char current_path[511];
int selected = 0;
int scroll   = 0;

file_entry *file_arr;
int size_of_file_array;

PrintConsole top_screen, instruction_screen, debug_screen;

CB clipboard;
bool quit_for_err = false;

int main(int argc, char **argv) {
  gfxInitDefault();
  consoleInit(GFX_TOP, &top_screen);
  consoleInit(GFX_BOTTOM, &debug_screen);
  consoleInit(GFX_BOTTOM, &instruction_screen);
  // x, y, width, height
  consoleSetWindow(&instruction_screen, 0, 0, 40, 9);
  consoleSetWindow(&debug_screen, 0, 10, 40, 20);

  consoleSelect(&instruction_screen);
  printf("A - cd / open context menu\nB - go up a directory\nX - CD to /\nY - Nothing\nL - Create a new dir\nR - Delete dir/file\nDPAD/Circle pad - up and down\nSTART - close app");
  printf("\n----------------------------------------");

  consoleSelect(&debug_screen);
  printf("Started...\n");

  // Initial allocation
  file_arr = malloc(1 * sizeof(file_entry));
  if (file_arr == NULL) {
    err_show("main.c", __LINE__-2, "file_arr memory allocation failed");
  }

  // For when it is first realloc(ed)
  size_of_file_array = 1;
  // Copy root dir into current_path
  strcpy(current_path, "sdmc:/");
  // Init context menu
  ctm_init();
  // Fill file name array with file names
  fs_populate_filarr(current_path);
  // Print all in root dir
  draw_filearr(1);
  // Main loop
  while (aptMainLoop()) {
    gspWaitForVBlank();
    hidScanInput();

    if (quit_for_err) {
      break;
    }

    u32 kDown = hidKeysDown();

    if (kDown & KEY_START) {
      break;
    }

    else if (kDown & KEY_UP) {
      btn_up();
      draw_filearr(0);
    }

    else if (kDown & KEY_DOWN) {
      btn_down();
      draw_filearr(0);
    }

    else if (kDown & KEY_LEFT) {
      btn_left();
      draw_filearr(0);
    }

    else if (kDown & KEY_RIGHT) {
      btn_right();
      draw_filearr(0);
    }

    else if (kDown & KEY_X) {
      consoleSelect(&debug_screen);
      printf("%sKEY_X pressed%s\n", FG_BLUE, RESET);
      strcpy(current_path, "sdmc:/");
      printf("%snew path: %s%s\n", FG_GREEN, current_path, RESET);
      fs_populate_filarr(current_path);
      draw_filearr(1);
    }

    else if (kDown & KEY_A) { btn_a_pressed(); }
    else if (kDown & KEY_B) { btn_b_pressed(); }
    else if (kDown & KEY_L) { btn_l_pressed(); }
    else if (kDown & KEY_R) { btn_r_pressed(); }

    else if (kDown & KEY_Y) { ; }

    // Flush and swap framebuffers
    gfxFlushBuffers();
    gfxSwapBuffers();
  }

  free(file_arr);
  gfxExit();
  return 0;
}
