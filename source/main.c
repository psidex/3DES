#include <string.h> // String manipulation
#include <stdio.h>  // Basic functions
#include <dirent.h> // For opendir() and readdir()
#include <3ds.h>    // Main 3ds lib (libctru)

// Define global variables
char current_path[100]; // Array of chars which will contain the current path being used, max len of 100 chars
int on_screen_pointer;  // Int of which item in the file array is selected, so a "->" will be printed next to the selected item
char file_arr[24][256]; // 2D array of filenames. 24 filenames each can be 256 characters long. Only 24 for now

void get_all_in_dir(char dir_to_show[]) {  // Prints all files in a given directory
    DIR           *d;                        // Set d to a pointer which will point to a DIR
    struct dirent *dir;                      // Set dir to a pointer which will point to a struct
    int num = 0;                             // Used in the while loop for adding to the array

    d = opendir(dir_to_show);                // Open the dir

    if (d != NULL) {                         // If it returned something
        while ((dir = readdir(d)) != NULL) {    // While readdir returns something other than NULL. The variable dir will change each loop
            if (dir->d_type == DT_REG || dir->d_type == DT_DIR) { // If the type of file is a normal file or a directory, carry on
                strcpy(file_arr[num], dir->d_name); // Get d_name from the dir struct and copy into array
                num++;
            }
        }
        closedir(d);                            // close access to the d variable
    }
}

void print_all_values_in_filear(void) { // Print all 'things' in the file name array
    int i;                              // Init i variable
    for (i=0; i<24; i++) {              // For 24 loops (arays are 0 indexed!)
            if (i == on_screen_pointer) {         // If the loop number is where the on screen pointer is
                printf("\n-> %s" , file_arr[i]);  // Print it with the pointer
            } else{
                printf("\n%s" , file_arr[i]);     // Else, just print it
            }
    }
}

int main(int argc, char **argv) { // function: Main - run first

	gfxInitDefault();
	consoleInit(GFX_TOP, NULL); // Init a console on the top screen - this is where printf() will output

	printf("Starting...\n");

    on_screen_pointer = 0;         // On screen pointer set to 1st position
    printf("pointer set...\n");

	strcpy(current_path, "sdmc:/3ds"); // Copy /3ds/ into current_path
	printf("current path strcpyed...\n");

	get_all_in_dir(current_path);  // Fill file name array with file names
	printf("get_all_in_dir done...\n");

	print_all_values_in_filear();  // Print all in /3ds
    printf("all values printed...\n");

	printf("Should have printed files by now\n");

	// Main loop
	while (aptMainLoop()) {

		gspWaitForVBlank();
		hidScanInput();

		// Code here!

		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// Flush and swap framebuffers (Not sure what this actually means)
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	gfxExit(); // Exit services
	return 0;  // End main
}
