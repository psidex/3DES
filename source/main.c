#include <stdio.h>     // Basic functions
#include <stdlib.h>    // malloc and free
#include <string.h>    // String manipulation
#include <dirent.h>    // For opendir() and readdir()
#include <3ds.h>       // Main 3ds lib (libctru)
#include <sys/types.h> // for comparing what is in stat()
#include <sys/stat.h>  // stat() for directory identification
#include <libgen.h>    // for dirname()

// all malloc / realloc statements create the length needed +1, because there needs to be room for the end character

// Colours
// red = mem related output, blue = button related output, green = path related output, white = other

// Define global variables
int max_dir_name_size = 260;          // Max len of file/directory name
int arrowpos;                         // Int of which item in the file array is selected, so a "->" will be printed next to the selected item
char **file_arr;                      // A pointer to an array which is full of pointers which will point to strings (only way to create a dynamic/resizable array)
int size_of_file_array;               // Size of file name array
int array_set = 0;                    // Changed to 1 after it is malloc(ed)
int offset = 0;                       // Used to offset what is printed from the file array, to allow "scrolling"
int max_files_on_screen = 27;         // What it says on the tin
PrintConsole topScreen, bottomScreen; // One PrintConsole for each screen
char default_dir[6] = "sdmc:/";       // default direction shown when launched/X btn pressed
char *current_path;                   // Array of chars which will contain the current path being used, max len of 260 chars


void get_ud(void) {                                                        // Gets the upper directory of the current_path
    consoleSelect(&bottomScreen);

    char *path_to_iterate = malloc((strlen(current_path)+1)*sizeof(char)); // New path variable

    strcpy(path_to_iterate, current_path);                                 // Copy current path into new var

    char looking_for[1] = "/";                                             // looking for the token "/"
    char *token;                                                           // needed for strtok()
    int dummylen = strlen(current_path)+1;                                 // length of mem to allocate for each dummy
    char *dummy1 = malloc(dummylen*sizeof(char));                          // These 2 keep track of the latest path created
    char *dummy2 = malloc(dummylen*sizeof(char));                          //

    strcpy(dummy1, ""); // Sometimes there is random stuff in here for some reason, so clean both before starting
    strcpy(dummy2, ""); //

                                                  // Great explanation here http://stackoverflow.com/a/3890186
    token = strtok(path_to_iterate, looking_for); // Get the first token

    while( token != NULL ) {                      // Walk through other tokens
        printf("\x1b[32mToken: %s\x1b[0m\n", token);

        strcat(dummy1, token);                    // add token (for example "sdmc:" into dummy1
        strcat(dummy1, "/");                      // / needed

        if(strstr(dummy1, current_path) != NULL) {
            current_path = realloc(current_path, (strlen(dummy2)+1)*sizeof(char)); // re allocate less space for new path
            strcpy(current_path, dummy2);         // dummy2 happens after this, so will have 1 less token
            break;                                // break the while loop
        }

        strcat(dummy2, token);                    // same as what happens to dummy1
        strcat(dummy2, "/");

        token = strtok(NULL, looking_for);        // get the next token
    }

    printf("\x1b[32mnew current_path: %s\x1b[0m\n", current_path);
    printf("\x1b[32mdummy1: %s\x1b[0m\n", dummy1);
    printf("\x1b[32mdummy2: %s\x1b[0m\n", dummy2);
}


// This function is a bit messy, it creates 2 pointers,  which both point to the same directory, then iterates over each of those,
// first finding out what size the array needs to be, then allocates an array in memory accordingly
void get_all_in_dir(char dir_to_show[]) {                // Prints all files in a given directory
    consoleSelect(&bottomScreen);                        // Bottom screen is for debug info. Anything printed in this func will print to bottom screen
    DIR *d;                                              // Set d to a pointer which will point to a DIR
    DIR *nd;                                             // For the 2nd iteration of file names

    d = opendir(dir_to_show);                            // Open the dir
    nd = opendir(dir_to_show);                           // Open the dir for 2nd iteration
    printf("Opened dirs\n");

    if (d) {                                             // If it returned something and is a directory
        if (array_set == 1){                             // If memory has been set for the arrray
            for (int i=0; i<size_of_file_array; i++) {
                free(file_arr[i]);                       // Free all memory allocated inside array
            }
            free(file_arr);                              // Free memory allocated for array
            printf("\x1b[31mmem for array freed\x1b[0m\n");
        } else {
            array_set = 1;
            printf("array set = 1\n");
        }

        struct dirent *dir;                              // Set dir to a pointer which will point to a struct called dirent
        struct dirent *ndir;                             // For the 2nd iteration of file names
        int num = 0;                                     // Used in the while loop for finding out how many files are in dir, then as an index for file array
        arrowpos = 0;                                    // Reset the position of on screen arrow
        offset = 0;                                      // Line 17

        while ((dir = readdir(d)) != NULL) {             // While readdir returns something other than NULL. The variable dir will change each loop
            num++;                                       // Count how many files there are
        }

        size_of_file_array = num;                        // Set global size_of_file_array to the amount of elements in the array
        num = 0;                                         // reset num so it can be used again
        printf("size_of_file_array = %i\n", size_of_file_array);

        // Create a 2D string array using malloc
        file_arr = malloc((size_of_file_array+1) * sizeof(char*)); // Create an array of pointers the size of the amount of files in the chosen directory
        for (int i = 0; i < size_of_file_array; i++) {             // Set each pointer inside the array to point to a char
            file_arr[i] = malloc((max_dir_name_size+1) * sizeof(char));
        }

        printf("\x1b[31mfile_arr allocated mem\x1b[0m\n");
        printf("\x1b[31mfile_arr loc: %p\x1b[0m\n", file_arr);     // Escape codes used to colour text

        while ((ndir = readdir(nd)) != NULL) {           // Iterate over dir again, this time adding filenames to created 2D array
            strcpy(file_arr[num], ndir->d_name);         // Get d_name from the dir struct and copy into array
            num++;
        }
        printf("file_arr filled\n");
        closedir(d);                                     // close access to the dir variables
        closedir(nd);
        printf("dirs closed\n");
    }
}


void print_all_values_in_filear(void) {           // Print all 'things' in the file name array
    consoleSelect(&bottomScreen);                 // print debug stuff to bottom screen
    int i;                                        // Init i variable
    int max_files_to_print;                       // Maximum files that will be printed

    if (size_of_file_array < max_files_on_screen) {
        max_files_to_print = size_of_file_array;
    } else {
        max_files_to_print = max_files_on_screen;
    }

    printf("max_files_to_print: %i\n", max_files_to_print);
    consoleSelect(&topScreen);                    // print this to the top screen

    if (max_files_to_print > 0) {
        for (i=0; i<max_files_to_print; i++) {
            if (i == arrowpos) {                       // If the loop number is where the on screen pointer is
                printf("\n-> %s", file_arr[i+offset]); // Print it with the pointer
            } else{
                printf("\n   %s", file_arr[i+offset]); // Else, just print it without arrow
            }
        }
    } else {
        printf("\n\n\t\x1b[47;30m- Folder is empty -\x1b[0m"); // https://smealum.github.io/ctrulib/graphics_2printing_2colored-text_2source_2main_8c-example.html#a1
    }
}


void dpadup(void) {
    consoleSelect(&bottomScreen);
    if (size_of_file_array == 0){
        ;                                                    // do nothing
    }
    else if (arrowpos+offset == 0) {                         // if arrowpos is 0 and there is not offset, skip to the bottom
        if (size_of_file_array > max_files_on_screen) {      // If there is a need for offset
            arrowpos = max_files_on_screen-1;                // arrays are 0 indexed, so max files -1 = highest index of array shown
            offset = size_of_file_array-max_files_on_screen; // offset will be max size it can be
        } else {
            arrowpos = size_of_file_array-1;
        }
    }
    else if (offset > 0) {
        offset--;
    }
    else {
        arrowpos--;
    }
    printf("\x1b[34mDPADUP: arrowpos: %i, offset: %i\x1b[0m\n", arrowpos, offset);
}


void dpaddown(void) {
    consoleSelect(&bottomScreen);
    if (size_of_file_array == 0){
        ;                                               // do nothing
    }
    else if (arrowpos+offset == size_of_file_array-1) { // If arrowpos+offset are at the largest index of the file array (arrays are 0 indexed!)
        arrowpos = 0;                                   // go back to start
        offset = 0;
    }
    else if ((arrowpos == max_files_on_screen-1) && (arrowpos+offset < size_of_file_array-1)) { // If arrowpos = highest index and arrowpos+offset < the max index for the array
        offset++;
    }
    else {
        arrowpos++;
    }
    printf("\x1b[34mDPADDOWN: arrowpos: %i, offset: %i\x1b[0m\n", arrowpos, offset);
}


void a_pressed(void) {
    consoleSelect(&bottomScreen);
    if (size_of_file_array == 0){
        ;                                               // do nothing
    }
    else {
        // http://stackoverflow.com/a/4553076
        int newlen = strlen(current_path)+strlen(file_arr[arrowpos+offset])+strlen("/")+1; // +1 for end char
        char *for_testing_path = malloc(newlen*sizeof(char)); // For testing if new path is directory

        strcpy(for_testing_path, current_path);
        strcat(for_testing_path, file_arr[arrowpos+offset]);  // Create new path
        strcat(for_testing_path, "/");

        printf("\x1b[32mtest path: %s\x1b[0m\n", for_testing_path);

        struct stat path_stat;                        // Needed for stat()
        stat(for_testing_path, &path_stat);           // Gives info about the dir

        if (S_ISDIR(path_stat.st_mode)) {             // If it is actually a directory
            printf("\x1b[32mpath is dir\x1b[0m\n");
            current_path = realloc(current_path, newlen*sizeof(char));
            strcat(current_path, file_arr[arrowpos+offset]); // Change current_path to new dir
            strcat(current_path, "/");
            get_all_in_dir(current_path);
        } else {
            printf("\x1b[32mpath is not dir\x1b[0m\n");
        }
    }
}


void b_pressed(void) {
    consoleSelect(&bottomScreen);
    if (!strcmp(current_path, "sdmc:/")) {
        printf("\x1b[32mCurrently in sdmc:/\x1b[0m\n");
    }
    else {
        get_ud(); // move up a directory
        printf("\x1b[32mnew path: %s\x1b[0m\n", current_path);
    }

}


int main(int argc, char **argv) {      // function: Main - run first

	gfxInitDefault();

    // Initialize console for both screens using the two different PrintConsole
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    consoleSelect(&bottomScreen);
    printf("Started...\n");
    printf("\x1b[32mdefault dir: %s\x1b[0m\n", default_dir);

    arrowpos = 0;                      // On screen pointer set to 1st position

    current_path = malloc((strlen(default_dir)+1)*sizeof(char)); // Create memory for current path
    printf("\x1b[31mcurrent_path mem loc: %p\x1b[0m\n", current_path);
	strcpy(current_path, default_dir); // Copy sdmc:/3ds into current_path

	get_all_in_dir(current_path);      // Fill file name array with file names
	printf("get_all_in_dir() done\n");

	print_all_values_in_filear();      // Print all in /3ds

	// Main loop
	while (aptMainLoop()) {
        consoleSelect(&bottomScreen);

		gspWaitForVBlank();
		hidScanInput();

		// Buttons:
		// DPAD UP   : Move arrow up
		// DPAD DOWN : Move arrow down
		// BUTTON X  : Get files & directories again

		u32 kDown = hidKeysDown(); // Keys pressed
		// u32 kHeld = hidKeysHeld(); // Keys being held

		if (kDown & KEY_START) {
			break; // break in order to return to hbl
        }
        else if (kDown & KEY_UP) {
            dpadup();
            consoleInit(GFX_TOP, &topScreen); // re-init-ing the console clears the screen without clearing bottom screen
            print_all_values_in_filear();
        }
        else if (kDown & KEY_DOWN) {
            dpaddown();
            consoleInit(GFX_TOP, &topScreen);
            print_all_values_in_filear();
        }
        else if (kDown & KEY_X) {
            printf("\x1b[34mKEY_X pressed\x1b[0m\n");
            current_path = realloc(current_path, (strlen(default_dir)+1)*sizeof(char)); // reallocate memory for default path
            strcpy(current_path, default_dir); // Go back to /3ds
            get_all_in_dir(current_path);      // Re-init file array
            consoleInit(GFX_TOP, &topScreen);
            print_all_values_in_filear();
        }
        else if (kDown & KEY_A) {
            a_pressed();
            consoleInit(GFX_TOP, &topScreen);
            print_all_values_in_filear();
        }
        else if (kDown & KEY_B) {
            b_pressed();
            get_all_in_dir(current_path);
            consoleInit(GFX_TOP, &topScreen);
            print_all_values_in_filear();
        }

		// Flush and swap framebuffers (Not sure what this actually means)
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

    if (array_set == 1){                             // If memory has been set for the arrray
        for (int i=0; i<size_of_file_array; i++) {
            free(file_arr[i]);                       // Free all memory allocated inside array
        }
        free(file_arr);                              // Free memory allocated for array
    }
	free(current_path);
	gfxExit(); // Exit services
	return 0;  // End main
}
