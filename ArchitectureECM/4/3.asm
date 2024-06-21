# 5.3

.data
n: .word 202
prompt1: .asciiz " Enter number 1: "
prompt2: .asciiz " Enter number 2: "

.text
main:
# --- getting input
# print prompt1
li $v0, 4
la $a0, prompt1
syscall

# read int1
li $v0, 5
syscall
move $s0, $v0

# print prompt2
li $v0, 4
la $a0, prompt2
syscall

# read int2
li $v0, 5
syscall
move $s1, $v0

# --- adding 202
lw $s2, n
add $s0, $s0, $s2
add $s1, $s1, $s2

# --- print integers
# print int1
li $v0, 1
move $a0, $s0
syscall

# print " , "
la $v0, 11
li $a0, ','
syscall

li $a0, ' '
syscall

# print int2
li $v0, 1
move $a0, $s1
syscall

# exit
li $v0, 10
syscall