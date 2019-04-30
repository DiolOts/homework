use16
CURRENT_OFFSET EQU 0x7c00
org CURRENT_OFFSET
NEW_SEG EQU 0x8000
start:  
    mov ax, 0xffff
    mov sp, ax

    mov ax,3
    int 10h
        cld
        xor ax, ax

        mov ds, ax
        mov si, CURRENT_OFFSET
        
        mov di, ax      
        mov ax, NEW_SEG
        mov es, ax
        
        mov cx, 0x200
        mov bx, cx
        rep movsb

        mov ah, 2
        mov al, 1
        mov dx, 0
        mov cx, 2
        int 13h

        push NEW_SEG
        push NEW_OFFSET-CURRENT_OFFSET
        retf
        
NEW_OFFSET:
        in al, 0x92
        or al, 2
        out 0x92, al
                
        cli
        mov al, 0x8F
        out 0x70, al
        in al, 0x71

;xchg bx, bx
        mov eax, NEW_SEG
        shl eax, 4                      ;phisical addres = segmeng*0x10 + offset
        add eax, GDT-CURRENT_OFFSET
        mov [(GDTR-CURRENT_OFFSET) + 2], eax             ;phisical address of GDT table

        lgdt fword [GDTR-CURRENT_OFFSET]
        mov eax, cr0 
        or al, 1
        mov cr0, eax

;xchg bx, bx
        jmp fword 8: pm_Main-CURRENT_OFFSET
        
align 8
GDT:
        dw 0x0000
        dw 0x0000
        dw 0x0000
        dw 0x0000

CODE_DESC:
        dw 0x0fff
        dw 0x0000
        dw 0x9a08
        dw 0x00c0
DATA_DESC:
        dw 0xffff
        dw 0x0000
        dw 0x9210
        dw 0x00c0
STACK_DESC:
        dw 0xfff
        dw 0x0000
        dw 0x9618
        dw 0x00c0
VIDEO_DESC:
        dw 0xffff
        dw 0x8000
        dw 0x920b
        dw 0x0040

GDTR:
        Limit  dw (GDTR-CURRENT_OFFSET - GDT - 1)
        Base dd GDT   ; base of GDT

db 510-($-start) dup(0x90), 0x55, 0xaa

CODE_SEG equ CODE_DESC - GDT
DATA_SEG equ DATA_DESC - GDT
STACK_SEG equ STACK_DESC - GDT
VIDEO_SEG equ VIDEO_DESC - GDT

use32
pm_Main:
        mov ax, DATA_SEG
        mov ds, ax
        mov fs, ax

        mov ax, VIDEO_SEG
        mov gs, ax

        mov ax, STACK_SEG
        mov ss, ax
        mov esp, -1

CLI


        mov esi, Hello-CURRENT_OFFSET
        call print

        hlt
        jmp $
        
print: 
        pushad  
        mov ah, 7
        xor ebx, ebx
        xchg ebx, ebx
puts:
        mov al, byte [cs:esi+ebx]
        mov [gs:(ebx*2)], ax
        inc ebx 
        test al, al
        jnz puts
        popad
        hlt
        ret



Hello db 'pm_Main', 0
db 510-($-pm_Main) dup(0), 0x55, 0xaa