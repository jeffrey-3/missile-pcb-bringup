# GDB

.gdbinit file automatically loads firmware so you don't have to stop openocd

gdb-multiarch firmware.elf

list source code around current execution line: l

b line_number or break function_name

s to step (execute next line, if its a function then go into function call), n for next (don't step into it)

c to continue

ctrl+c to stop

quit to exit gdb

p SPI1 or p SPI1->CR1 to see registers or x/xw &SPI1->CR1 for hex

p variable_name to print value of variable

i b or info breakpoints to list all breakpoints

# Serial monitor

cu -l /dev/ttyACM0 -s 115200

enter ~ . to exit

# Vim

Type same thing on multiple lines Ctrl+v, shif+i, type what you want, then esc twice

:E file tree explorer

:vsplit filename

Copy/cut and paste multiple lines: v, select text, y for copy or d for cut, ctrl+w+w, p

Copy single line: yy

Undo: u

Redo: Ctrl+r

Save: :w

Quit: :q

Quit without saving: :q!

Ctrl+u and Ctrl+d to move up and down slower

Resize split: Ctrl+w > or < or + or -

Delete multiple lines: Ctrl+v, use arrows to select text, d

Shift+4 to go to end of line, Shift+6 to go to start of line

Shift+3 is highlighting search the current word under cursor, :noh to exit

Copy multiple lines to clipboard: go to visual, select lines, "+y

:term to open horizontal split terminal

:e filename to switch files

:set list and :set nolist to view whitespace

:retab to fix tabs

Search: press / and type what you want to search
