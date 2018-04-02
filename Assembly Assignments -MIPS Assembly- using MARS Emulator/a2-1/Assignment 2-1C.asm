# Ryan Askew, CS 2318-001, Assignment 2 Part 1 Program C

			.data
			
buffer:			.word, 7, 9, 3, 8, 5
initialLabel:		.asciiz, "Original Array, Elements 1-5: "
swappedLabel:		.asciiz, "Swapped Array, Elements 5-1: "
			
			
			.text
			
			
			.globl main
main:
			# Load Array
			########################################
			li $v0, 4
			la $a0, initialLabel
			syscall
			
			la $t0, buffer
			########################################
			
			
			
			
			# Print the original array
			########################################
			li $v0, 1
			lb $a0, 0($t0)
			syscall
			
			li $v0, 11
			li $a0, ' '
			syscall
			
			li $v0, 1
			lb $a0, 4($t0)
			syscall
			
			li $v0, 11
			li $a0, ' '
			syscall
			
			li $v0, 1
			lb $a0, 8($t0)
			syscall
			
			li $v0, 11
			li $a0, ' '
			syscall
			
			li $v0, 1
			lb $a0, 12($t0)
			syscall
			
			li $v0, 11
			li $a0, ' '
			syscall
			
			li $v0, 1
			lb $a0, 16($t0)
			syscall
			
			li $v0, 11
			li $a0, '\n'
			syscall
			########################################
			
			
			
			# Swap elements of the array in memory
			########################################
			lb $t1, 0($t0)
			lb $t2, 8($t0)
			sb $t1, 8($t0)
			sb $t2, 0($t0)
			
			lb $t1, 4($t0)
			lb $t2, 16($t0)
			sb $t1, 16($t0)
			sb $t2, 4($t0)
			########################################
			
			
			
			#Print the swapped array
			########################################
			li $v0, 4
			la $a0, swappedLabel
			syscall
			
			la $t0, buffer
			
			li $v0, 1
			lb $a0, 16($t0)
			syscall
			
			li $v0, 11
			li $a0, ' '
			syscall
			
			li $v0, 1
			lb $a0, 12($t0)
			syscall
			
			li $v0, 11
			li $a0, ' '
			syscall
			
			li $v0, 1
			lb $a0, 8($t0)
			syscall
			
			li $v0, 11
			li $a0, ' '
			syscall
			
			li $v0, 1
			lb $a0, 4($t0)
			syscall
			
			li $v0, 11
			li $a0, ' '
			syscall
			
			li $v0, 1
			lb $a0, 0($t0)
			syscall
			########################################
			
			
			li $v0, 10
			syscall