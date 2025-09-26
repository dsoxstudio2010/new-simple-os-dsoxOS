// kernel.c - ядро для dsoxOS (реальный режим)
typedef unsigned short uint16_t;

void main() {
    // Видеопамять в реальном режиме (0xB8000)
    uint16_t* video = (uint16_t*)0xB8000;
    
    // Сообщение
    char *msg = "Welcome to dsoxOS! Kernel is running!";
    int i = 0;
    
    // Атрибут: белый на черном
    uint16_t attribute = 0x0700;
    
    // Вывод сообщения
    while (msg[i] != '\0') {
        video[i] = attribute | msg[i];
        i++;
    }
    
    // Бесконечный цикл
    while(1) {
        // Мигающий курсор в конце сообщения
        video[i] = attribute | '_';
        
        // Простая задержка
        for (volatile long j = 0; j < 50000; j++);
        
        video[i] = attribute | ' ';
        
        for (volatile long j = 0; j < 50000; j++);
    }
}
