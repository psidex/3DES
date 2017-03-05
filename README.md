# 3dsFB
A (simple) homebrew 3ds File Browser written in C

Currently all debug info outputs to the lower screen, printing all the debug
info slows down everything else, so I will be adding either another version that
does not have that, or an option to turn it off

---

# Buttons
- A - Change to selected directory
- B - go "up" a directory
- X - CD to `/` and reset and reallocate memory
- L - Create a new directory (brings a keyboard up to type the name)
- DPAD/Circle pad up and down control the selection arrow
- START to close app

---

# TODO
- Check memory allocation after `malloc`
- Basic file actions like delete and create directories
- Open files and show binary

---

# Credits

Thanks to [Pirater12](https://github.com/Pirater12) and [LiquidFenrir](https://github.com/LiquidFenrir) for helping me
with the code :)

---

# Notes (mainly for myself)
Useful:
https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html

---
