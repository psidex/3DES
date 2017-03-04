#include <stdio.h>  // Basic functions
#include <stdlib.h> // malloc and free
#include <string.h> // String manipulation
#include <dirent.h> // For opendir() and readdir()
#include <3ds.h>    // Main 3ds lib (libctru)

#include <sys/types.h>
#include <sys/stat.h>

// Define global variables
char current_path[100];       // Array of chars which will contain the current path being used, max len of 100 chars
int arrowpos;                 // Int of which item in the file array is selected, so a "->" will be printed next to the selected item
char **file_arr;              // A pointer to an array which is full of pointers which will point to strings (only way to create a dynamic/resizable array)
int size_of_file_array;       // Size of file name array
int max_dir_name_size = 260;  // Max len of file/directory name
int array_set = 0;            // Changed to 1 after it is malloc(ed)
int offset = 0;               // Used to offset what is printed from the file array, to allow "scrolling"
int max_files_on_screen = 27; // What it says on the tin

// This function is a bit messy, it creates 2 pointers,  which both point to the same directory, then iterates over each of those,
// first finding out what size the array needs to be, then allocates an array in memory accordingly
void get_all_in_dir(char dir_to_show[]) {                // Prints all files in a given directory
    DIR *d;                                              // Set d to a pointer which will point to a DIR
    DIR *nd;                                             // For the 2nd iteration of file names

    d = opendir(dir_to_show);                            // Open the dir
    nd = opendir(dir_to_show);                           // Open the dir for 2nd iteration

    if (d) {                                             // If it returned something and is a directory
        if (array_set == 1){                             // If memory has been set for the arrray
            for (int i=0; i<size_of_file_array; i++) {
                free(file_arr[i]);                       // Free all memory allocated inside array
            }
            free(file_arr);                              // Free memory allocated for array
        } else {
            array_set = 1;
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

        // Create a 2D string array using malloc
        file_arr = malloc(size_of_file_array * sizeof(char*));  // Create an array of pointers the size of the amount of files in the chosen directory
        for (int i = 0; i < size_of_file_array; i++) {          // Set each pointer inside the array to point to a char
            file_arr[i] = malloc(max_dir_name_size * sizeof(char));
        }

        while ((ndir = readdir(nd)) != NULL) {           // Iterate over dir again, this time adding filenames to created 2D array
            strcpy(file_arr[num], ndir->d_name);         // Get d_name from the dir struct and copy into array
            num++;
        }
        closedir(d);                                     // close access to the dir variables
        closedir(nd);
    }
}

void print_all_values_in_filear(void) {           // Print all 'things' in the file name array
    int i;                                        // Init i variable
    int max_files_to_print;                       // Maximum files that will be printed

    if (size_of_file_array < max_files_on_screen) {
        max_files_to_print = size_of_file_array;
    } else {
        max_files_to_print = max_files_on_screen;
    }

    for (i=0; i<max_files_to_print; i++) {
        if (i == arrowpos) {                       // If the loop number is where the on screen pointer is
            printf("\n-> %s", file_arr[i+offset]); // Print it with the pointer
        } else{
            printf("\n   %s", file_arr[i+offset]); // Else, just print it without arrow
        }
    }
}

void dpadup(void) {
    if (arrowpos+offset == 0) {                          // if arrowpos is 0 and there is not offset, skip to the bottom
        if (size_of_file_array > max_files_on_screen) {  // If there is a need for offset
            arrowpos = max_files_on_screen-1;                // arrays are 0 indexed, so max files -1 = highest index of array shown
            offset = size_of_file_array-max_files_on_screen; // offset will be max size it can be
        } else {
            arrowpos = size_of_file_array-1;
        }

    } else if (offset > 0) {
        offset--;
    } else {
        arrowpos--;
    }
}

void dpaddown(void) {
    if (arrowpos+offset == size_of_file_array-1) { // If arrowpos+offset are at the largest index of the file array (arrays are 0 indexed!)
        arrowpos = 0;                              // go back to start
        offset = 0;
    } else if ((arrowpos == max_files_on_screen-1) && (arrowpos+offset < size_of_file_array-1)) { // If arrowpos = highest index and arrowpos+offset < the max index for the array
        offset++;
    } else {
        arrowpos++;
    }
}

void a_pressed(void) {
    // http://stackoverflow.com/a/4553076
    char for_testing_path[260];                   // For testing if new path is directory
    strcpy(for_testing_path, current_path);
    strcat(for_testing_path, "/");
    strcat(for_testing_path, file_arr[arrowpos+offset]); // Create new path

    struct stat path_stat;                        // Needed for stat()
    stat(for_testing_path, &path_stat);           // Gives info about the dir

    if (S_ISDIR(path_stat.st_mode)) {             // If it is actually a directory
        strcat(current_path, "/");
        strcat(current_path, file_arr[arrowpos+offset]); // Change current_path to new dir
        get_all_in_dir(current_path);
    }
}

int main(int argc, char **argv) {      // function: Main - run first

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);        // Init a console on the top screen - this is where printf() will output

    arrowpos = 0;                      // On screen pointer set to 1st position

	strcpy(current_path, "sdmc:/3ds"); // Copy sdmc:/3ds into current_path

	get_all_in_dir(current_path);      // Fill file name array with file names

	print_all_values_in_filear();      // Print all in /3ds

	// Main loop
	while (aptMainLoop()) {

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
        else if (kDown & KEY_DUP) {
            dpadup();
            consoleClear();
            print_all_values_in_filear();
        }
        else if (kDown & KEY_DDOWN) {
            dpaddown();
            consoleClear();
            print_all_values_in_filear();
        }
        else if (kDown & KEY_X) {
            strcpy(current_path, "sdmc:/3ds"); // Go back to /3ds
            get_all_in_dir(current_path); // Re-init file array
            consoleClear();
            print_all_values_in_filear();
        }
        else if (kDown & KEY_A) {
            a_pressed();
            consoleClear();
            print_all_values_in_filear();
        }

        //    else if (kHeld & KEY_DUP) {
        //        dpadup();
        //        consoleClear();
        //        print_all_values_in_filear();
        //    }
        //    else if (kHeld & KEY_DDOWN) {
        //        dpaddown();
        //        consoleClear();
        //        print_all_values_in_filear();
        //    }

		// Flush and swap framebuffers (Not sure what this actually means)
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	gfxExit(); // Exit services
	return 0;  // End main
}
