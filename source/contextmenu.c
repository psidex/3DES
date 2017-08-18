#include "common.h"
#include "contextmenu.h"
#include "sha256.h"
#include "draw.h"

const char *ctm_items[4];
int ctm_items_len = 4;
int ctm_selected;

void draw_context_menu(void) {
  consoleSelect(&topScreen);
  printf(RESET_TO_TOP_LEFT);
  printf("%s%s%s\n\n", BLACK_ON_WHITE, file_arr[selected+scroll], RESET);
  for (int i=0; i<ctm_items_len; i++) {
    if (i == ctm_selected) {
      printf("\t-> %s\n", ctm_items[i]);
    }
    else {
      printf("\t   %s\n", ctm_items[i]);
    }
  }
  printf("\nPress B to go back\n");
}

void open_context_menu(void) {
  ctm_selected = 0;
  ctm_items[0] = "Get SHA256 Hash";
  ctm_items[1] = "Cut";
  ctm_items[2] = "Copy";
  ctm_items[3] = "Paste";

  clearscrn();
  draw_context_menu();
  while (aptMainLoop()) {
    gspWaitForVBlank();
    hidScanInput();
    u32 ctm_kDown = hidKeysDown();

    if (ctm_kDown & KEY_UP) {
      if (!ctm_selected) {
        ctm_selected = ctm_items_len-1;
      }
      else {
        ctm_selected--;
      }
      draw_context_menu();
    }

    else if (ctm_kDown & KEY_DOWN) {
      if (ctm_selected == ctm_items_len-1) {
        ctm_selected = 0;
      }
      else {
        ctm_selected++;
      }
      draw_context_menu();
    }

    else if (ctm_kDown & KEY_A) {
      if (ctm_selected == 0) {
        sha256_current_file();
        clearscrn();
        draw_context_menu();
      }
    }

    else if (ctm_kDown & KEY_B) {
      clearscrn();
      break;
    }

    gfxFlushBuffers();
    gfxSwapBuffers();
  }
}
