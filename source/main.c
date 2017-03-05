#include <stdio.h>     // Basic functions
#include <stdlib.h>    // malloc and free
#include <string.h>    // String manipulation
#include <dirent.h>    // For opendir() and readdir()
#include <3ds.h>       // Main 3ds lib (libctru)
#include <sys/types.h> // for comparing what is in stat()
#include <sys/stat.h>  // stat() for directory identification
#include <libgen.h>    // for dirname()

/*
Notes

According to Wikipedia, the max FAT32 path length is 255 UTF-16 characters, so 255 * 2 = 510 (because the 16 in UTF-16 means 16 bits = 2 bytes)

all malloc / realloc statements create the length needed +1, because there needs to be room for the end character

Colours { red = mem related output, blue = button related output, green = path related output, white = other }
*/


// Array of chars which will contain the current path being used
char current_path[510];
// Int of which item in the file array is selected
int selected = 0;
// A pointer to an array which is full of pointers which will point to strings (only way to create a dynamic/resizable array)
char **file_arr;
// Size of file name array
int size_of_file_array;
// Changed to 1 after it is malloc(ed)
int array_set = 0;
// Used to offset what is printed from the file array, to allow "scrolling"
int scroll = 0;
// One PrintConsole for each screen
PrintConsole topScreen, bottomScreen;
// Max len of file/directory name
int MAX_DIR_NAME_SIZE = 260;
// What it says on the tin
int MAX_FILES_ON_SCREEN = 27;



// Gets the upper directory of the current_path
void get_ud(void) {
    consoleSelect(&bottomScreen);

    char path_to_iterate[510];
    strcpy(path_to_iterate, current_path);

    char looking_for[] = "/";
    char *token;
    char dummy1[510];
    char dummy2[510];

    // Sometimes there is random stuff in here for some reason, so clean both before starting
    strcpy(dummy1, "");
    strcpy(dummy2, "");

    // Great explanation here http://stackoverflow.com/a/3890186
    token = strtok(path_to_iterate, looking_for);

    while( token != NULL ) {
        printf("\x1b[32mToken: %s\x1b[0m\n", token);

        // add token (for example "sdmc:" into dummy1
        strcat(dummy1, token);
        strcat(dummy1, "/");

        if(strstr(dummy1, current_path) != NULL) {
            // dummy2 happens after this, so will have 1 less token
            strcpy(current_path, dummy2);
            break;
        }

        strcat(dummy2, token);
        strcat(dummy2, "/");

        // get the next token
        token = strtok(NULL, looking_for);
    }

    printf("\x1b[32mnew current_path: %s\x1b[0m\n", current_path);
    printf("\x1b[32mdummy1: %s\x1b[0m\n", dummy1);
    printf("\x1b[32mdummy2: %s\x1b[0m\n", dummy2);
}



// Fills file array with all files in a given directory
void get_all_in_dir(char dir_to_show[]) {
    // Bottom screen is for debug info. Anything printed in this func will print to bottom screen
    consoleSelect(&bottomScreen);

    // 2 of each for 2 iterations
    DIR *d;
    DIR *nd;
    d = opendir(dir_to_show);
    nd = opendir(dir_to_show);
    printf("Opened dirs\n");

    if (d) {
        // If memory has been set for the array
        if (array_set == 1) {
            // Free all memory allocated inside array
            for (int i=0; i<size_of_file_array; i++) { free(file_arr[i]); }
            // Free memory allocated for array
            free(file_arr);
            printf("\x1b[31mmem for array freed\x1b[0m\n");
        }

        else {
            array_set = 1;
            printf("array set = 1\n");
        }

        struct dirent *dir;
        struct dirent *ndir;
        int count = 0;
        selected  = 0;
        scroll    = 0;

        // While readdir returns something other than NULL. The variable dir will change each loop
        while ((dir = readdir(d)) != NULL) {
            // Count how many files there are
            count++;
        }

        size_of_file_array = count;
        count = 0;
        printf("size_of_file_array = %i\n", size_of_file_array);

        // Create a 2D string array using malloc
        // Create an array of pointers the size of the amount of files in the chosen directory
        file_arr = malloc((size_of_file_array+1) * sizeof(char*));
        for (int i = 0; i < size_of_file_array; i++) {
            // Set each pointer inside the array to point to a char
            file_arr[i] = malloc((MAX_DIR_NAME_SIZE+1) * sizeof(char));
        }

        // Escape codes used to colour text
        printf("\x1b[31mfile_arr loc: %p\x1b[0m\n", file_arr);

        // Iterate over dir again, this time adding filenames to created 2D array
        while ((ndir = readdir(nd)) != NULL) {
            // Get d_name from the dir struct and copy into array
            strcpy(file_arr[count], ndir->d_name);
            count++;
        }
        printf("file_arr filled\n");
        closedir(d);
        closedir(nd);
        printf("dirs closed\n");
    }
}



// Print all 'things' in the file name array
void print_all_values_in_filear(void) {
    consoleSelect(&bottomScreen);

    int max_files_to_print;

    if (size_of_file_array < MAX_FILES_ON_SCREEN) { max_files_to_print = size_of_file_array; }
    else { max_files_to_print = MAX_FILES_ON_SCREEN; }

    printf("max_files_to_print: %i\n", max_files_to_print);

    consoleSelect(&topScreen);

    if (max_files_to_print > 0) {
        int i;
        for (i=0; i<max_files_to_print; i++) {
            if (i == selected) {
                // print as white text on black background
                printf("\n\t \x1b[47;30m%s\x1b[0m", file_arr[i+scroll]);
            } else{
                printf("\n\t %s", file_arr[i+scroll]); // Else, just print it without arrow
            }
        }
    } else {
        // https://smealum.github.io/ctrulib/graphics_2printing_2colored-text_2source_2main_8c-example.html#a1
        printf("\n\n\t\x1b[47;30m- Folder is empty -\x1b[0m");
    }
}



void up(void) {
    consoleSelect(&bottomScreen);
    if (size_of_file_array == 0){ ; }

    // if selected is 0 and there is no scroll, skip to the bottom
    else if (selected+scroll == 0) {
         // If there is a need for scroll
        if (size_of_file_array > MAX_FILES_ON_SCREEN) {
            // arrays are 0 indexed, so max files -1 = highest index of array shown
            selected = MAX_FILES_ON_SCREEN-1;
            // scroll will be max size it can be
            scroll = size_of_file_array-MAX_FILES_ON_SCREEN;
        }
        else { selected = size_of_file_array-1; }
    }

    else if (scroll > 0) { scroll--; }

    else { selected--; }

    printf("\x1b[34mDPADUP: selected: %i, scroll: %i\x1b[0m\n", selected, scroll);
}



void down(void) {
    consoleSelect(&bottomScreen);
    if (size_of_file_array == 0){ ; }

    // If selected+scroll are at the largest index of the file array (arrays are 0 indexed!)
    else if (selected+scroll == size_of_file_array-1) {
        selected = 0;
        scroll = 0;
    }

    // If selected = highest index and selected+scroll < the max index for the array
    else if ((selected == MAX_FILES_ON_SCREEN-1) && (selected+scroll < size_of_file_array-1)) { scroll++; }

    else { selected++; }

    printf("\x1b[34mDPADDOWN: selected: %i, scroll: %i\x1b[0m\n", selected, scroll);
}



void a_pressed(void) {
    consoleSelect(&bottomScreen);
    if (size_of_file_array == 0){ ; }

    else {
        // For testing if new path is directory
        char for_testing_path[510];
        strcpy(for_testing_path, current_path);

        // Create new path
        strcat(for_testing_path, file_arr[selected+scroll]);
        strcat(for_testing_path, "/");

        printf("\x1b[32mtest path: %s\x1b[0m\n", for_testing_path);

        // Needed for stat()
        struct stat path_stat;
        stat(for_testing_path, &path_stat);

        // If it is actually a directory
        if (S_ISDIR(path_stat.st_mode)) {
            printf("\x1b[32mpath is dir\x1b[0m\n");
            strcpy(current_path, for_testing_path);
            get_all_in_dir(current_path);
        }

        else { printf("\x1b[32mpath is not dir\x1b[0m\n"); }
    }
}



void b_pressed(void) {
    consoleSelect(&bottomScreen);

    if (!strcmp(current_path, "sdmc:/")) { printf("\x1b[32mCurrently in sdmc:/\x1b[0m\n"); }

    // move up a directory
    else { get_ud(); }
}



void l_pressed(void) {
    consoleSelect(&bottomScreen);
    printf("Bringing up keyboard...\n");

    SwkbdState swkbd;
    char newdirname[260];
    SwkbdButton button = SWKBD_BUTTON_NONE;

    swkbdInit(&swkbd, SWKBD_TYPE_WESTERN, 1, 260);
    swkbdSetHintText(&swkbd, "New directory name here");
    button = swkbdInputText(&swkbd, newdirname, sizeof(newdirname));

    printf("Dir name: %s\n", newdirname);

    // Create the path to be test/created
    char path_to_create[510];
    strcpy(path_to_create, current_path);
    strcat(path_to_create, newdirname);

    printf("path to create: %s\n", path_to_create);

    int result;

    // will fail is path already exists
    result = mkdir(path_to_create, 0700);

    if (result == 0) {
        printf("Dir created\n");
    }
    else {
        printf("Dir creation failed\n");
    }
}



int main(int argc, char **argv) {

	gfxInitDefault();

	// Initialize console for both screens using the two different PrintConsole
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &bottomScreen);

    consoleSelect(&bottomScreen);

    printf("Started...\n");

    strcpy(current_path, "sdmc:/");

	get_all_in_dir(current_path);      // Fill file name array with file names
	printf("get_all_in_dir() done\n");

	print_all_values_in_filear();      // Print all in /3ds

	// Main loop
	while (aptMainLoop()) {

		gspWaitForVBlank();
		hidScanInput();

		consoleSelect(&bottomScreen);

		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) { break; }

        else if (kDown & KEY_UP) {
            up();
            consoleInit(GFX_TOP, &topScreen); // re-init-ing the console clears the screen without clearing bottom screen
            print_all_values_in_filear();
        }

        else if (kDown & KEY_DOWN) {
            down();
            consoleInit(GFX_TOP, &topScreen);
            print_all_values_in_filear();
        }

        else if (kDown & KEY_X) {
            printf("\x1b[34mKEY_X pressed\x1b[0m\n");
            strcpy(current_path, "sdmc:/");
            get_all_in_dir(current_path);
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

        else if (kDown & KEY_L ) {
            l_pressed();
            get_all_in_dir(current_path);
            consoleInit(GFX_TOP, &topScreen);
            print_all_values_in_filear();
        }

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
	}

	for (int i=0; i<size_of_file_array; i++) { free(file_arr[i]); }
    free(file_arr);
	gfxExit();
	return 0;
}
