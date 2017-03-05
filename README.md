# 3dsFB
A (simple) homebrew 3ds File Browser written in C

Currently all debug info outputs to the lower screen, printing all the debug
info slows down everything else, so I will be adding either another version that
does not have that, adding an option to turn it off, or most likely just making
it more readable and more of a notification area

# Buttons
- A - Change to selected directory
- B - go "up" a directory
- X - CD to `/` and reallocate memory
- L - Create a new directory (brings a keyboard up to type the name)
- DPAD/Circle pad up and down control the selection arrow
- START to close app and go back to HB menu

# TODO
- Check memory allocated correctly after `malloc`
- In `get_all_in_dir()`, realloc memory instead of freeing and allocating again
- Set R to delete file/directory
- If you press A on a file, show it's binary (maybe show text)
- Get a nice icon for .SMDH file

# Credits

Thanks to [Pirater12](https://github.com/Pirater12) and [LiquidFenrir](https://github.com/LiquidFenrir) for helping me
with the code :)

# Notes (mainly for myself)
Useful:
[Directory stuff](https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html)
[Using escape codes](https://smealum.github.io/ctrulib/graphics_2printing_2colored-text_2source_2main_8c-example.html#a1)
[Explanation of `strtok()`](http://stackoverflow.com/a/3890186)
