; boot.asm syka nasm
bits 16
org 0X7c00

start:
    cli ; no prerivania
    ;sigmo
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00 ;ebani steck

    sti ;yes prerivania
    
    mov si, msg_loading
    call print_string
    
    mov ah, 0x02   ; read disk porno
    mov al, 10     ; количество секторов для чтения
    mov ch, 0      ; chilindr
    mov cl, 2      ; sector
    mov dh, 0      ; golovka
    mov bx, 0x7E00 ; adress load kernel
    
    jc disk_error  ; error piness
    
    call switch_to_pm
    jmp $
    
disk_error:
    mov si, msg_disk_error
    call print_string
    jmp $

;printing
print_string:
    mov ah, 0x0E
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

;переключения в защещёны режим я пиздос заебался
;во всём вмноваты пендосы!!!
switch_to_pm:
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp 0x08:init_pm

bits 32

init_pm:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax 
    mov ss, ax

    mov esp, 0x90000
    call 0x7E00
    jmp $

msg_loading db "Loading kernel...", 0xD, 0xA, 0
msg_disk_error db "Disk error!", 0xD, 0xA, 0

; GDT
gdt_start:
    gdt_null:                   ; Нулевой дескриптор
        dd 0
        dd 0
    
    gdt_code:                   ; Дескриптор кода
        dw 0xFFFF               ; Лимит (0-15)
        dw 0                    ; База (0-15)
        db 0                    ; База (16-23)
        db 10011010b            ; Флаги доступа
        db 11001111b            ; Флаги + лимит (16-19)
        db 0                    ; База (24-31)
    
    gdt_data:                   ; Дескриптор данных
        dw 0xFFFF
        dw 0
        db 0
        db 10010010b
        db 11001111b
        db 0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Размер GDT
    dd gdt_start                ; Адрес GDT

; Заполнение до 510 байт
times 510-($-$$) db 0

; Сигнатура загрузочного сектора
dw 0xAA55
