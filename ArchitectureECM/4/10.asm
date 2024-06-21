# 5.10

.data
msgf: .asciiz " Enter float: "
msgd: .asciiz " Enter double: "

.text
main:
# print prompt
li $v0, 4
la $a0, msgf
syscall

# read float
li $v0, 6
syscall
mov.s $f2, $f0

# print prompt
li $v0, 4
la $a0, msgd
syscall

# read double
li $v0, 7
syscall
mov.d $f4, $f0

# print numbers
li $v0, 2
mov.s $f12, $f2
syscall

li $v0, 11
li $a0, ' '
syscall

li $v0, 3
mov.d $f12, $f4
syscall

# exit
li $v0, 10
syscall
