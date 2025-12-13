// Startup code
__attribute__((naked, noreturn)) void _reset(void) {
	// memset .bss to zero, and copy .data section to RAM region
	extern long _sbss, _ebss, _sdata, _edata, _sidata;
	for (long *dst = &_sbss; dst < &_ebss; dst++) *dst = 0;
	for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) *dst++ = *src++;

        // Call main()
	extern void main(void);
        main();
	for (;;) (void) 0; // Infinite loop in the case if main() returns
}

extern void systick_handler(void); // Defined in main.c
extern void _estack(void); // Defined in link.ld

__attribute__((section(".vectors"))) void (*const tab[16 + 32])(void) = {
	_estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, systick_handler};
