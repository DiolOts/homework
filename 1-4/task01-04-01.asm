
#make_boot#

org 7c00h 
   
    
start:

	N EQU 4
    CLD


	lea	di,Welc1
	push di
	call OutputText
    call CR 
    call CR

	lea	di,Text1
	push di
	call OutputText
	
	lea	di,Slug1
	push di
	call InputText
	
	lea	di, Slug1
	push di
	call str2dw   
	push ax
	
	lea	di,Text2
	push di
	call OutputText
		
	lea	di,Slug2
	push di
	call InputText
        
    lea	di, Slug2
	push di
	call str2dw 
	pop bx
	add ax, bx   
    
    lea	di,Summ
	call printdec 
	
	lea	di,Text3
	push di
	call OutputText 
	
	lea	di,Summ
	push di
	call OutputText
    
    call CR
    lea	di,Text4
	push di
	call OutputText
    
    mov ah, 0
    int 16h   

    exit:
	INT 19h 
	

InputText:
    push bp
    mov bp, sp
    mov di, [bp + 4]
    mov cx, 100
    mov dx, N
InputRepeat:
    mov ah, 0
    int 16h
    cmp al, 0Dh
    je PutEnter
    cmp al,'9'
    jnbe InputRepeat
    cmp al,'0'
    jb InputRepeat
    mov ah, 0x0e
    int 10h
    STOSB 
 
    sub dx, 1
    jz End
    loop InputRepeat
    ret 2
PutEnter:
    mov al, '$'
    mov cx, dx
    REPE STOSB 
End:    
    call CR
    pop bp
    ret 2 


OutputText:
    push bp
    mov bp, sp
    mov di, [bp + 4]
    mov cx, 255
OutputRepeat:
    mov al, es:di
    cmp al, '$'
    je EndOfStroke
    mov ah, 0x0e
    int 10h 
    add di, 1
    loop OutputRepeat
    pop bp
    ret 2
EndOfStroke:
    pop bp
    ret 2     


 
CR:
    mov al, 0dh
    mov ah, 0x0e
    int 10h 
    mov al, 0ah
    mov ah, 0x0e
    int 10h 
ret 



str2dw:
    push bp
    mov bp, sp
	mov si, [bp + 4]
	xor dx,dx	
	xor bx,bx	
lp1:	xor ax,ax
	lodsb		
	test al,al	
	jz ex
	cmp al, '$'
	je ex
	cmp al,'9'	
	jnbe lp1
	cmp al,'0'  
	jb lp1
	sub ax,'0'	
	shl dx, 1	
	add ax, dx
	shl dx, 2
	add dx, ax	
	jmp lp1
ex:	mov ax,dx 
    pop bp
	ret 2         
  	

printdec proc
    push cx; 
    push dx
    push bx
    mov bx, 10
    xor cx, cx
m1: xor dx, dx
    div bx; 
    push dx
    inc cx
    test ax, ax
    jnz m1
m2: pop ax
    add al, '0'
    stosB
    loop m2
    pop bx
    Pop dx
    Pop cx
    RET
printdec endp        
    
Welc1 db 'Welcome to Sum calculator Otus Reverse Engineering, Task 1-4','$'

Text1 db 'slug1 = ','$'
Text2 db 'slug2 = ','$'
Text3 db 'summ = ','$'
Text4 db 'Press any key ...','$'
Slug1 db N dup('$')
Slug2 db N dup('$')
Summ db N+1 dup('$')  



