# 5.7

.text
main:
# midi out sync
li $v0, 33
li $a0, 60   	# pitch      (middle C)
li $a1, 1000	# duration   (ms)
li $a2, 4	# instrument (piano 0-7)
li $a3, 64	# volume
syscall

# midi out async
li $v0, 31
li $a0, 60   	# pitch      (middle C)
li $a1, 1000	# duration   (ms)
li $a2, 28	# instrument (guitar 24-31)
li $a3, 64	# volume
syscall

# exit
li $v0, 10
syscall