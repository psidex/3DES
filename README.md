# 3DES
A (simple) homebrew 3ds File Browser written in C

 - Top screen - main file Browser
 - Upper area of bottom screen - instructions
 - Lower area of bottom screen - debug output (look here if, for example, you try to create a directory and nothing happens)

Compiled with [devkitPro](https://devkitpro.org/)

# Notes/Warnings
- Closing the lid (only tested on n3ds) whilst on the delete screen causes the 3ds to not "wake up", and you have to press and hold the power button to turn it off
- Deleting directories uses a recursive function, so if you have a large (very large) amount of directories/files inside the directory you want to delete, it may end up breaking

# Buttons

- A - Change to selected directory (opening files not supported)
- B - go "up" a directory
- X - CD to `/`
- L - Create a new directory (brings a keyboard up to type the name)
- R - Delete file/directory
- DPAD/Circle pad up and down control the selection of files/directories
- DPAD/Circle pad left and right go to the top/bottom of the file/directory list
- START to close app and go back to HB menu

# TODO
- Fix closing lid on delete screen causing 3ds to not wake up in certain situations
- Show a message when file/dir successfully deleted

# Credits
Thanks to [Pirater12](https://github.com/Pirater12) and [LiquidFenrir](https://github.com/LiquidFenrir) for helping me
with the code :)

Icon made by [Madebyoliver](http://www.flaticon.com/authors/madebyoliver) from [www.flaticon.com](http://www.flaticon.com) is licensed by [CC 3.0 BY](http://creativecommons.org/licenses/by/3.0/)

# Notes (mainly for myself)
[Directory stuff](https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html) | [Using escape codes](https://smealum.github.io/ctrulib/graphics_2printing_2colored-text_2source_2main_8c-example.html#a1) | [Explanation of `strtok()`](http://stackoverflow.com/a/3890186)

# Build
`make`
`makerom -f cia -o 3DES.cia -rsf meta\cia.rsf -target t -exefslogo -elf 3DES.elf -icon 3DES.smdh -banner meta\banner.bin`
