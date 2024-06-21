# 10.2

# Реализуйте программу для представления, введенного пользователем числа в дополнительном коде.
# Программа должна использовать только операторы XOR и ADD. 
# Ваша программа должна включать правильное и полезное приглашение для ввода 
# и выводить результаты осмысленным образом

.data
prompt: .asciiz " Enter number: "
output1: .asciiz " Your number in binary: "
output2: .asciiz "\n Negative number in binary: "
output3: .asciiz "\n Negative number:  "

.text

# print prompt
addi $v0, $zero, 4
la $a0, prompt
syscall

# read integer
addi $v0, $zero, 5
syscall
move $s0, $v0

# print number in binary
addi $v0, $zero, 4
la $a0, output1
syscall

addi $v0, $zero, 35
move $a0, $s0
syscall

# calculate negative number
xori $t0, $s0, 0xffffffff
addi $s1, $t0, 1

# print negative number in binray 
addi $v0, $zero, 4
la $a0, output2
syscall

addi $v0, $zero, 35
move $a0, $s1
syscall

# print negative number in decimal
addi $v0, $zero, 4
la $a0, output3
syscall

addi $v0, $zero, 1
move $a0, $s1
syscall

# exit
li $v0, 10
syscall 