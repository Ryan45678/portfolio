##########################################################################
# Ryan Askew, CS 2318-001, Assignment 2 Part 1 Program B
##########################################################################
# Checks & reports if a user-entered 32-bit int has 2^2 & 2^7 contributors
############################ data segment ################################
		.data
legend1:	.asciiz "sans 128, sans 4 --> 0\n"
legend2:	.asciiz "sans 128, with 4 --> 1\n"
legend3:	.asciiz "with 128, sans 4 --> 32\n"
legend4:	.asciiz "with 128, with 4 --> 33\n"
inPrompt:	.asciiz "Enter an integer: "
outLab:		.asciiz "It is "
############################ code segment ################################
		.text
		.globl main
main:
		li $v0, 4
		la $a0, legend1        
		syscall                   # print legend line 1
		la $a0, legend2        
		syscall                   # print legend line 2
		la $a0, legend3        
		syscall                   # print legend line 3
		la $a0, legend4        
		syscall                   # print legend line 4
		la $a0, inPrompt        
		syscall                   # print input prompt
		
		li $v0, 5
		syscall                   # read input integer

		##########################################################
		# Write NO MORE THAN 7 lines of code that involve using 
		# ONLY the following:
		# - syscall
		# - syscall supporting instructions (e.g.: li to load $v0)
		# - bit manipulating instructions (ANDing, ORing, XORing,
		#   NORing and shifting - only whatever that are needed)
		# so that the program will work just like the sample runs 
		# shown at the bottom.
		# You MUST test your completed program for AT LEAST the 
		# test cases shown (and include the result in hardcopy).
		##########################################################
		
		
		
		
		srl $t0, $v0, 2		# shift right so 128 and 4 now align with 32 and 1
		
		li $v0, 4		# print the result label
		la $a0, outLab
		syscall
		
		li $v0, 1		# print the result
		andi $a0, $t0, 33	# use and to mask the shifted number with 33 (32 + 1)
		syscall
		
		
				
                ##########################################################
		
                li $v0, 10               # exit
                syscall

########################## sample test runs ##############################
# sans 128, sans 4 --> 0
# sans 128, with 4 --> 1
# with 128, sans 4 --> 32
# with 128, with 4 --> 33
# Enter an integer: 123456
# It is 0
# -- program is finished running --
# 
# Reset: reset completed.
# 
# sans 128, sans 4 --> 0
# sans 128, with 4 --> 1
# with 128, sans 4 --> 32
# with 128, with 4 --> 33
# Enter an integer: 234567
# It is 1
# -- program is finished running --
# 
# Reset: reset completed.
# 
# sans 128, sans 4 --> 0
# sans 128, with 4 --> 1
# with 128, sans 4 --> 32
# with 128, with 4 --> 33
# Enter an integer: 345543
# It is 33
# -- program is finished running --
# 
# Reset: reset completed.
# 
# sans 128, sans 4 --> 0
# sans 128, with 4 --> 1
# with 128, sans 4 --> 32
# with 128, with 4 --> 33
# Enter an integer: 454321
# It is 32
# -- program is finished running --
# 
# Reset: reset completed.
# 
# sans 128, sans 4 --> 0
# sans 128, with 4 --> 1
# with 128, sans 4 --> 32
# with 128, with 4 --> 33
# Enter an integer: -1
# It is 33
# -- program is finished running --
# 
# Reset: reset completed.
# 
# sans 128, sans 4 --> 0
# sans 128, with 4 --> 1
# with 128, sans 4 --> 32
# with 128, with 4 --> 33
# Enter an integer: -987654321
# It is 1
# -- program is finished running --
######################## end sample test runs ############################
