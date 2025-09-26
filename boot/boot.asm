; boot.asm - простой загрузчик
bits 16
org 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Сообщение о загрузке
    mov si, msg_loading
    call print_string

    ; Загрузка ядра (2 сектора)
    mov ah, 0x02
    mov al, 2
    mov ch, 0
    mov cl, 2
    mov dh, 0
    mov bx, 0x7E00
    int 0x13
    jc disk_error

    mov si, msg_loaded
    call print_string

    ; Прыжок к ядру
    jmp 0x0000:0x7E00

disk_error:
    mov si, msg_disk_error
    call print_string
    jmp $

print_string:
    mov ah, 0x0E
    mov bh, 0
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

msg_loading db "Loading kernel...", 0xD, 0xA, 0
msg_loaded db "Kernel loaded!", 0xD, 0xA, 0
msg_disk_error db "Disk error!", 0xD, 0xA, 0

times 510-($-$$) db 0
dw 0xAA55
