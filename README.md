# 3dsFB
A (simple) homebrew 3ds File Browser written in C

 - Top screen - main file Browser
 - Upper area of bottom screen - instructions
 - Lower area of bottom screen - debug output (look here if, for example, you try
to create a directory and nothing happens)

# Buttons
##### Things in the buttons list below that are labelled as `not supported` have the possibility of being added at a future date but are not definitely going to be implemented.

- A - Change to selected directory (opening files not supported)
- B - go "up" a directory
- X - CD to `/` and reallocate memory
- L - Create a new directory (brings a keyboard up to type the name)
- R - Delete file (deleting non-empty directories not supported)
- DPAD/Circle pad up and down control the selection of files/directories
- START to close app and go back to HB menu

# TODO
- Check if memory has been allocated correctly after `malloc` & `realloc`
- Clean up on screen instructions
- Quick-skip to top/bottom of list using left/right
- Fast scroll if you hold up/down
- Be able to select a file with Y then copy/paste to another path
- Get a nice icon for .SMDH file

# Credits
Thanks to [Pirater12](https://github.com/Pirater12) and [LiquidFenrir](https://github.com/LiquidFenrir) for helping me
with the code :)

# Notes (mainly for myself)
[Directory stuff](https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html) | [Using escape codes](https://smealum.github.io/ctrulib/graphics_2printing_2colored-text_2source_2main_8c-example.html#a1) | [Explanation of `strtok()`](http://stackoverflow.com/a/3890186)
