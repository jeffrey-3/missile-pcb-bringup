__attribute__((naked, noreturn)) void _reset(void) {
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    
    // Set .bss to zero 
    for (long *dst = &_sbss; dst < &_ebss; dst++) {
        *dst = 0;
    }
        
    // Copy .data section to RAM
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata;) {
        *dst++ = *src++;
    }

    // Call main()
    extern void main(void);
    main();
    
    for (;;) (void) 0; // Infinite loop in the case if main() returns
}

extern void _systick_handler(void); // Defined in hal.c
extern void _estack(void); // Defined in link.ld

__attribute__((section(".vectors"))) void (*const tab[16 + 32])(void) = {
    _estack,
    _reset,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    _systick_handler
};
