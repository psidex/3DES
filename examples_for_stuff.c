#include <libgen.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void get_ud(char current_wd[]) { // Get upper directory
    // for example: char cwd[30] = "sdmc:/3ds/3dsfileio/";
    // Upper directory would be "sdmc:/3ds/"

    dirname( current_wd ); // Init and set to directory
}

char* get_new_wd(char *current_wd, char *file_selected) {
  // return current_wd+"/"+file_selected
  char *new = strcat(current_wd, file_selected);
  return new;
}

int main(void) {
    char cwd[30] = "sdmc:/3ds/3dsfileio/test/";
    printf("\ncwd before: %s\n\n", cwd);

    char another_cwd[30] = "sdmc:/3ds/3dsfileio/blah/";
    char to_cd_to[30] = "test2";

    dirname(cwd);

    char *moving_to = get_new_wd(another_cwd, to_cd_to);

    printf("\n\ncwd: %s\nmoving_to: %s\n\n", cwd, moving_to);
    return 0;
}
