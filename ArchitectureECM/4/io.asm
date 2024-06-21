.text 					# Define the program instructions .
main: 					# Label to define the main program .
# print prompt
li $v0, 4
la $a0, prompt
syscall

# read string
li $v0, 8
la $a0, string
lw $a1, size
syscall

# print message
li $v0, 4
la $a0, output
syscall

# print read string
li $v0, 4
la $a0, string
syscall

# exit
li $v0, 10
syscall

.data
string: .space 81
size:   .word 80
prompt: .asciiz " Please, enter a string: "
output: .asciiz " You entered: "