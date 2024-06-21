# 10.8

.data
prompt1: .asciiz " Enter number 1: "
prompt2: .asciiz " Enter number 2: "
result: .asciiz " resutlt: "

.text
# print prompt1
addi $v0, $zero, 4
la $a0, prompt1
syscall

# read number1
addi $v0, $zero, 5
syscall
move $s0, $v0

# print prompt2
addi $v0, $zero, 4
la $a0, prompt2
syscall

# read number2
addi $v0, $zero, 5
syscall
move $s1, $v0

# multiply
multu $s0, $s1

# print result: 0 - no overflow, other - overflow
addi $v0, $zero, 4
la $a0, result
syscall

addi $v0, $zero, 1
mfhi $a0
syscall

# exit
li $v0, 10
syscall
