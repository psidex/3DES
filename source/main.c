#include <string.h> // String manipulation
#include <stdio.h>  // Basic functions
#include <dirent.h> // For opendir() and readdir()
#include <3ds.h>    // Main 3ds lib (libctru)

// Define global variables
char current_path[100];  // Array of chars which will contain the current path being used, max len of 100 chars
int arrowpos;            // Int of which item in the file array is selected, so a "->" will be printed next to the selected item
char file_arr[100][256]; // 2D array of filenames. 100 filenames each can be 256 characters long.
int size_of_file_array;  // Size of file name array: size_of_file_array = sizeof(file_arr) / sizeof(file_arr[0]);

void get_all_in_dir(char dir_to_show[]) {                // Prints all files in a given directory
    memset(file_arr, 0, sizeof(file_arr[0][0])*100*256); // Clear file_arr so new values can be set
    DIR           *d;                                    // Set d to a pointer which will point to a DIR
    struct dirent *dir;                                  // Set dir to a pointer which will point to a struct
    int num = 0;                                         // Used in the while loop for adding to the array

    d = opendir(dir_to_show);                            // Open the dir

    if (d != NULL) {                                     // If it returned something
        while ((dir = readdir(d)) != NULL) {             // While readdir returns something other than NULL. The variable dir will change each loop
            strcpy(file_arr[num], dir->d_name);          // Get d_name from the dir struct and copy into array
            num++;
        }
        closedir(d);                                     // close access to the d variable
    }
}

void print_all_values_in_filear(void) {           // Print all 'things' in the file name array
    int i;                                        // Init i variable

    for (i=0; i<30; i++) {                        // For 29 loops because that is max amount of elements that will fit on 3ds top screen
        if (i == arrowpos) {                      // If the loop number is where the on screen pointer is
            printf("\n-> %s" , file_arr[i]);      // Print it with the pointer
        } else{
            printf("\n   %s" , file_arr[i]);      // Else, just print it
        }
    }
}

void dpadup(void) {
    if (arrowpos == 0) {
        arrowpos = 29;
    } else {
        arrowpos--;
    }
}

void dpaddown(void) {
    if (arrowpos == 29) {
        arrowpos = 0;
    } else {
        arrowpos++;
    }
}

int main(int argc, char **argv) {      // function: Main - run first

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);        // Init a console on the top screen - this is where printf() will output

	printf("Starting...\n");

    arrowpos = 0;                      // On screen pointer set to 1st position
    printf("pointer set...\n");

	strcpy(current_path, "sdmc:/3ds"); // Copy /3ds/ into current_path
	printf("current path strcpyed...\n");

	get_all_in_dir(current_path);      // Fill file name array with file names
	printf("get_all_in_dir done...\n");

	print_all_values_in_filear();      // Print all in /3ds

	// Main loop
	while (aptMainLoop()) {

		gspWaitForVBlank();
		hidScanInput();

		// Code here!

		u32 kDown = hidKeysDown(); // Keys pressed
		u32 kHeld = hidKeysHeld(); // Keys being held

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
