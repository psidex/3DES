#include <stdio.h>  // Basic functions
#include <stdlib.h> // malloc and free
#include <string.h> // String manipulation
#include <dirent.h> // For opendir() and readdir()
#include <3ds.h>    // Main 3ds lib (libctru)

// Define global variables
char current_path[100];      // Array of chars which will contain the current path being used, max len of 100 chars
int arrowpos;                // Int of which item in the file array is selected, so a "->" will be printed next to the selected item
char **file_arr;             // A pointer to an array which is full of pointers which will point to strings (only way to create a dynamic/resizable array)
int size_of_file_array;      // Size of file name array
int max_dir_name_size = 260; // Max len of file/directory name
int array_set = 0;           // Changed to 1 after it is malloc(ed)

// This function is a bit messy, it creates 2 pointers,  which both point to the same directory, then iterates over each of those,
// first finding out what size the array needs to be, then allocates an array in memory accordingly
void get_all_in_dir(char dir_to_show[]) {                // Prints all files in a given directory
    if (array_set == 1){
        free(file_arr);                                  // Free current file array from memory
    } else {
        array_set = 1;
    }

    DIR *d;                                              // Set d to a pointer which will point to a DIR
    DIR *nd;
    struct dirent *dir;                                  // Set dir to a pointer which will point to a struct
    struct dirent *ndir;                                 // For the 2nd iteration
    int num = 0;                                         // Used in the while loop for finding out how many files are in dir, then as an index for file array

    d = opendir(dir_to_show);                            // Open the dir
    nd = opendir(dir_to_show);                           // Open the dir for 2nd iteration

    if (d != NULL) {                                     // If it returned something
        while ((dir = readdir(d)) != NULL) {             // While readdir returns something other than NULL. The variable dir will change each loop (?)
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

    for (i=0; i<size_of_file_array; i++) {
        if (i == arrowpos) {                      // If the loop number is where the on screen pointer is
            printf("\n-> %s", file_arr[i]);       // Print it with the pointer
        } else{
            printf("\n   %s", file_arr[i]);       // Else, just print it
        }
    }
}

void dpadup(void) {
    if (arrowpos == 0) {
        arrowpos = size_of_file_array-1;
    } else {
        arrowpos--;
    }
}

void dpaddown(void) {
    if (arrowpos == size_of_file_array-1) {
        arrowpos = 0;
    } else {
        arrowpos++;
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
            get_all_in_dir(current_path); // Re-init file array
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
