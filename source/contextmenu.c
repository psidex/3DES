#include "common.h"
#include "contextmenu.h"
#include "sha256.h"
#include "draw.h"

#define ctm_items_len 4
#define ctm_item_str_len 20

char ctm_items[ctm_items_len][ctm_item_str_len];
int ctm_selected;

void setup_ctm(void) {
  strncpy(ctm_items[0], "Get SHA256 Hash", ctm_item_str_len);
  strncpy(ctm_items[1], "Cut", ctm_item_str_len);
  strncpy(ctm_items[2], "Copy", ctm_item_str_len);
  strncpy(ctm_items[3], "Paste", ctm_item_str_len);
}

void draw_context_menu(void) {
  consoleSelect(&topScreen);
  printf(RESET_TO_TOP_LEFT);
  printf("%s%s%s\n\n", BLACK_ON_WHITE, file_arr[selected+scroll].name, RESET);
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
      switch (ctm_selected) {
        case 0:
          sha256_current_file();
          clearscrn();
          draw_context_menu();
          break;
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
