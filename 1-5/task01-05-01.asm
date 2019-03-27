.text
    .globl main

main:
    sub         $sp , $sp , 4           # push stack
    sw          $ra , 0 ( $sp )     # save return address

    addi        $v0 , $0 , 4   
    la          $a0 , Welc1 
    syscall     #printing Welc1
    
    addi        $v0 , $0 , 4   
    la          $a0 , Text1 
    syscall     #printing Text1

    addi        $v0 , $0 , 5   
    syscall     #get input

    move        $t0 , $v0               # save input in $t0

        
    addi        $v0 , $0 , 4   
    la          $a0 , Text2 
    syscall     #printing Text2

    addi        $v0 , $0 , 5   
    syscall     #get input

    move        $t1 , $v0               # save input in $t1
    add 	$t1 , $t1 , $t0
    
    addi        $v0 , $0 , 4   
    la          $a0 , Text3 
    syscall     #printing Text3

    move        $v0 , $t1               # save summ in $v0
    move        $a0 , $v0
    addi        $v0 , $0 , 1
    syscall     #print summ 
    
    addi        $v0 , $0 , 4   
    la          $a0 , Text4 
    syscall     #printing Text4

    addi        $v0 , $0 , 12   
    syscall     #get input
    
     .data

Welc1 :.asciiz "\“Welcome to Summ calculator Otus Reverse Engineering, Task 1-5\n\n"
Text1:.asciiz "\“slug1 = "
Text2:.asciiz "\“slug2 = "
Text3:.asciiz "\“summ = "
Text4:.asciiz "\nPress any key ..."