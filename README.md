openocd -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/target/stm32g0x.cfg

gdb-multiarch ./firmware.elf

target extended-remote localhost:3333

cu -l /dev/ttyACM0 -s 115200
enter ~ . to exit

# Vim

:vsplit filename
Copy/cut and paste multiple lines: v, select text, y for copy or d for cut, ctrl+w+w, p
Copy single line: yy
Undo: u
Redo: Ctrl+r
Save: :w
Quit: :q
Quit without saving: :q!
Resize split: Ctrl+w > or < or + or -
Delete multiple lines: Ctrl+v, use arrows to select text, d
Ctrl+4 to go to front, Ctrl+6 to go to back
Shift+3 is highlighting search the current word under cursor, :noh to exit
