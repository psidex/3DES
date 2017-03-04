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
- DPAD/Circle pad up and down control the selection arrow
- START to close app

---

# TODO
- Check memory allocation after `malloc` and `realloc`
- Open files and show binary
- Basic file actions like delete and create

---

# Notes (mainly for myself)
Useful:
https://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html

---
