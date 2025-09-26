// kernel.c - простое ядро
#include <stddef.h>
#include <stdint.h>

// Цвета для VGA
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// Создание цвета VGA
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

// Создание символа VGA
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

// Размеры экрана
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// Буфер VGA
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;

// Позиция курсора
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;

// Инициализация терминала
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    // Очистка экрана
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            vga_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

// Установка цвета
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Вывод символа
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT) {
            terminal_row = 0;
        }
        return;
    }
    
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    vga_buffer[index] = vga_entry(c, terminal_color);
    
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

// Вывод строки
void terminal_writestring(const char* data) {
    while (*data != '\0') {
        terminal_putchar(*data++);
    }
}

// Функция задержки
void delay(void) {
    for (volatile long i = 0; i < 1000000; i++);
}

// Точка входа ядра
void kernel_main(void) {
    terminal_initialize();
    
    terminal_writestring("Welcome to MyOS!\n");
    terminal_writestring("Simple kernel written in C\n\n");
    
    int counter = 0;
    while (1) {
        terminal_setcolor(vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
        terminal_writestring("System is running... ");
        
        terminal_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
        terminal_writestring("OK ");
        
        // Вывод счетчика
        char counter_str[10];
        char *ptr = counter_str;
        int n = counter++;
        if (n == 0) {
            *ptr++ = '0';
        } else {
            char temp[10];
            char *t = temp;
            while (n > 0) {
                *t++ = '0' + (n % 10);
                n /= 10;
            }
            while (t > temp) {
                *ptr++ = *--t;
            }
        }
        *ptr = '\0';
        terminal_writestring(counter_str);
        terminal_writestring("\n");
        
        delay();
        
        // Очистка последней строки
        terminal_row = 3;
        terminal_column = 0;
        for (size_t i = 0; i < VGA_WIDTH; i++) {
            terminal_putchar(' ');
        }
        terminal_row = 3;
        terminal_column = 0;
        
        delay();
    }
}
