#include "common.h"
#include "sort.h"

// Sort filename array (yay for bubble sort)
void bubble_sort_files(void) {
  bool swapped = true;
  file_entry temp;
  int pass;
  int pass2;

  for (pass=size_of_file_array-1; pass>0; pass--) {
    for (pass2=0; pass2<pass; pass2++) {
      if (strcasecmp(file_arr[pass2].name, file_arr[pass2+1].name) > 0) {
        swapped = true;
        // Swap values in file name array
        temp = file_arr[pass2];
        file_arr[pass2] = file_arr[pass2+1];
        file_arr[pass2+1] = temp;
      }
    }
    if (!swapped) { break; }
  }
}
