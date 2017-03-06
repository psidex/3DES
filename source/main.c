#include <stdio.h>     // Basic functions
#include <stdlib.h>    // malloc and free
#include <string.h>    // String manipulation
#include <dirent.h>    // For opendir() and readdir()
#include <3ds.h>       // Main 3ds lib (libctru)


/*
Notes

According to Wikipedia, the max FAT32 path length is 255 UTF-16 characters, so 255 * 2 = 510 (because the 16 in UTF-16 means 16 bits = 2 bytes)

all malloc / realloc statements create the length needed +1, because there needs to be room for the end character

Colours { red = error related, blue = button related, green = path related, white = other }
*/


// Array of chars which will contain the current path being used
char current_path[510];
// Int of which item in the file array is selected
int selected = 0;
// A pointer to an array which is full of pointers which will point to strings (only way to create a dynamic/resizable array)
char **file_arr;
// Same as file_arr, will be used "alongside" file_arr to check whether something is a file or directory
char *isfile_arr;
// Size of file name array
int size_of_file_array;
// Used to offset what is printed from the file array, to allow "scrolling"
int scroll = 0;
// One PrintConsole for each screen
PrintConsole topScreen, instructionscreen, debugscreen;
// Max len of file/directory name
int MAX_DIR_NAME_SIZE = 260;
// What it says on the tin
int MAX_FILES_ON_SCREEN = 26;



// Gets the upper directory of the current_path
void get_ud(void) {
    consoleSelect(&debugscreen);

    char path_to_iterate[510];
    strcpy(path_to_iterate, current_path);

    char looking_for[] = "/";
    char *token;
    char dummy1[510];
    char dummy2[510];

    // Sometimes there is random stuff in here for some reason, so clean both before starting
    strcpy(dummy1, "");
    strcpy(dummy2, "");

    token = strtok(path_to_iterate, looking_for);

    while( token != NULL ) {
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

        // get the next token288342890277634069
        token = strtok(NULL, looking_for);
    }

    printf("\x1b[32mnew path: %s\x1b[0m\n", current_path);
}



// Fills file array with all files in a given directory
void get_all_in_dir(char dir_to_show[]) {
    // Bottom screen is for debug info. Anything printed in this func will print to bottom screen
    consoleSelect(&debugscreen);

    // 2 of each for 2 iterations
    DIR *d;
    DIR *nd;
    d = opendir(dir_to_show);
    nd = opendir(dir_to_show);

    if (d) {
        // Get rid of strings in file name array
        for (int i = 0; i < size_of_file_array; i++) { free(file_arr[i]); }

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

        // Create a 2D string array using malloc
        // Create an array of pointers the size of the amount of files in the chosen directory
        file_arr = realloc(file_arr, (size_of_file_array+1) * sizeof(char*));
        // Set each pointer inside the array to point to a char
        for (int i = 0; i < size_of_file_array; i++) { file_arr[i] = malloc((MAX_DIR_NAME_SIZE+1) * sizeof(char)); }
        isfile_arr = realloc(isfile_arr, (size_of_file_array+1) * sizeof(bool));

        // Iterate over dir again, this time adding filenames to created 2D array
        while ((ndir = readdir(nd)) != NULL) {
            // Get d_name from the dir struct and copy into array
            strcpy(file_arr[count], ndir->d_name);
            // If d_type is a file
            isfile_arr[count] = (ndir->d_type == 8);
            count++;
        }
        closedir(d);
        closedir(nd);
    }
}



// Print all 'things' in the file name array
void print_all_values_in_filear(void) {
    consoleSelect(&debugscreen);

    int max_files_to_print;

    if (size_of_file_array < MAX_FILES_ON_SCREEN) { max_files_to_print = size_of_file_array; }
    else { max_files_to_print = MAX_FILES_ON_SCREEN; }

    consoleSelect(&topScreen);

    if (max_files_to_print > 0) {
        if (scroll > 0) { printf("/\\\n"); } // Print up arrow showing you can scroll up
        else { printf("\n"); }               // Keep all the files in the same place on screen

        int i;
        for (i=0; i<max_files_to_print; i++) {

            // If it is a dir
            if (!isfile_arr[i+scroll]) {
                // print as white text on black background
                if (i == selected) { printf("\n <dir>  | \x1b[47;30m%s\x1b[0m", file_arr[i+scroll]); }
                // Else, just print it without arrow
                else { printf("\n <dir>  | %s", file_arr[i+scroll]); }
            }

            else {
                // print as white text on black background
                if (i == selected) { printf("\n <file> | \x1b[47;30m%s\x1b[0m", file_arr[i+scroll]); }
                // Else, just print it without arrow
                else { printf("\n <file> | %s", file_arr[i+scroll]); }

            }
        }
        // If there are files below on screen
        if ( (size_of_file_array > MAX_FILES_ON_SCREEN) && (scroll == 0) ) { printf("\n\n\\/ "); }
    }
    else { printf("\n\n\t\t\x1b[47;30m- Folder is empty -\x1b[0m"); }
}



void up(void) {
    consoleSelect(&debugscreen);
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
}

void down(void) {
    consoleSelect(&debugscreen);
    if (size_of_file_array == 0){ ; }

    // If selected+scroll are at the largest index of the file array (arrays are 0 indexed!)
    else if (selected+scroll == size_of_file_array-1) {
        selected = 0;
        scroll = 0;
    }

    // If selected = highest index and selected+scroll < the max index for the array
    else if ((selected == MAX_FILES_ON_SCREEN-1) && (selected+scroll < size_of_file_array-1)) { scroll++; }

    else { selected++; }
}



void a_pressed(void) {
    consoleSelect(&debugscreen);
    if (size_of_file_array == 0){ ; }

    else {
        // If it is actually a directory
        if (!isfile_arr[selected+scroll]) {
            printf("path is dir\n");
            strcat(current_path, file_arr[selected+scroll]);
            strcat(current_path, "/");
            printf("\x1b[32mnew path: %s\x1b[0m\n", current_path);
            get_all_in_dir(current_path);
        }

        else { printf("path is file\n"); }
    }
}

void b_pressed(void) {
    consoleSelect(&debugscreen);

    if (!strcmp(current_path, "sdmc:/")) { printf("\x1b[32mcurrently in sdmc:/\x1b[0m\n"); }

    // move up a directory
    else { get_ud(); }
}



void l_pressed(void) {
    consoleSelect(&debugscreen);
    printf("Bringing up keyboard...\n");

    SwkbdState swkbd;
    char newdirname[260];
    SwkbdButton button = SWKBD_BUTTON_NONE;

    swkbdInit(&swkbd, SWKBD_TYPE_WESTERN, 1, 260);
    swkbdSetHintText(&swkbd, "New directory name here");
    button = swkbdInputText(&swkbd, newdirname, sizeof(newdirname));

    // Create the path to be test/created
    char path_to_create[510];
    strcpy(path_to_create, current_path);
    strcat(path_to_create, newdirname);

    printf("\x1b[32mpath to create: %s\x1b[0m\n", path_to_create);

    int result;

    // will fail is path already exists
    result = mkdir(path_to_create, 0700);

    if (result == 0) {
        printf("Dir created\n");
    }
    else {
        printf("\x1b[31mDir creation failed\x1b[0m\n");
    }
}



int main(int argc, char **argv) {

	gfxInitDefault();

	// Initialize console for both screens using the two different PrintConsole
    consoleInit(GFX_TOP, &topScreen);
    consoleInit(GFX_BOTTOM, &debugscreen);
    consoleInit(GFX_BOTTOM, &instructionscreen);

    consoleSetWindow(&instructionscreen, 0, 0, 40, 5);
    consoleSetWindow(&debugscreen, 0, 6, 40, 24);

    consoleSelect(&instructionscreen);
    printf("A - CD/Open file, B - go up a directory X - CD to /, L - Create a new dir\nDPAD/Circle pad - up and down\nSTART - close app");
    printf("\n----------------------------------------");

    consoleSelect(&debugscreen);
    printf("Started...\n");

    // Initial allocation for these 2 arrays
    file_arr = malloc(1 * sizeof(char*));
    isfile_arr = malloc(1 * sizeof(bool));

    // For when it is first realloc(ed)
    size_of_file_array = 1;

    strcpy(current_path, "sdmc:/");

	get_all_in_dir(current_path);      // Fill file name array with file names

	print_all_values_in_filear();      // Print all in /3ds

	// Main loop
	while (aptMainLoop()) {

		gspWaitForVBlank();
		hidScanInput();

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
            consoleSelect(&debugscreen);
            printf("\x1b[34mKEY_X pressed\x1b[0m\n");
            strcpy(current_path, "sdmc:/");
            printf("\x1b[32mnew path: %s\x1b[0m\n", current_path);
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

    // Clear up allocated memory
	for (int i=0; i<size_of_file_array; i++) { free(file_arr[i]); }
    free(file_arr);
    free(isfile_arr);
	gfxExit();
	return 0;
}
