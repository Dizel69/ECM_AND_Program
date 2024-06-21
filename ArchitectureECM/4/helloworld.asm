.text 					# Define the program instructions .
main: 					# Label to define the main program .
# print message
li $v0, 4
la $a0, greeting
syscall

# exit
li $v0, 10
syscall

.data
greeting: .asciiz " Hello Wold! "
