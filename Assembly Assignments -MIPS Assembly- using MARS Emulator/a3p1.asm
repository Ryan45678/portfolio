			.text
			.globl main
main:
###################################
# Register Usage:
###################################
# $v1: used
# $t0: numEven
# $t3: reply
# $t5: oneInt
# $t7: leftInt
# $t8: rightInt
# $t1: hopPtr1
# $t2: hopPtr2
# $a1: endPtr1
# $a2: endPtr2
# $t9: temp
###################################

# Allocating space for local variables on the stack
			addiu $sp, $sp, -143
			
# storing the 4 strings on the stack
			
			# inStr: "\nInt #"
                        li $t4, '\n'
                        sb $t4, 0($sp)
                        li $t4, 'I'
                        sb $t4, 1($sp)
                        li $t4, 'n'
                        sb $t4, 2($sp)
                        li $t4, 't'
                        sb $t4, 3($sp)
                        li $t4, ' '
                        sb $t4, 4($sp)
                        li $t4, '#'
                        sb $t4, 5($sp)
                        li $t4, '\0'
                        sb $t4, 6($sp)

			# slrStr: "Allowed hi of 10 reached..."
			li $t4, 'A'
                        sb $t4, 7($sp)
                        li $t4, 'l'
                        sb $t4, 8($sp)
                        li $t4, 'l'
                        sb $t4, 9($sp)
                        li $t4, 'o'
                        sb $t4, 10($sp)
                        li $t4, 'w'
                        sb $t4, 11($sp)
                        li $t4, 'e'
                        sb $t4, 12($sp)
                        li $t4, 'd'
                        sb $t4, 13($sp)
                        li $t4, ' '
                        sb $t4, 14($sp)
                        li $t4, 'h'
                        sb $t4, 15($sp)
                        li $t4, 'i'
                        sb $t4, 16($sp)
                        li $t4, ' '
                        sb $t4, 17($sp)
                        li $t4, 'o'
                        sb $t4, 18($sp)
                        li $t4, 'f'
                        sb $t4, 19($sp)
                        li $t4, ' '
                        sb $t4, 20($sp)
                        li $t4, '1'
                        sb $t4, 21($sp)
                        li $t4, '0'
                        sb $t4, 22($sp)
                        li $t4, ' '
                        sb $t4, 23($sp)
                        li $t4, 'r'
                        sb $t4, 24($sp)
                        li $t4, 'e'
                        sb $t4, 25($sp)
                        li $t4, 'a'
                        sb $t4, 26($sp)
                        li $t4, 'c'
                        sb $t4, 27($sp)
                        li $t4, 'h'
                        sb $t4, 28($sp)
                        li $t4, 'e'
                        sb $t4, 29($sp)
                        li $t4, 'd'
                        sb $t4, 30($sp)
                        li $t4, '.'
                        sb $t4, 31($sp)
                        li $t4, '.'
                        sb $t4, 32($sp)
                        li $t4, '.'
                        sb $t4, 33($sp)
                        li $t4, '\0'
                        sb $t4, 34($sp)

			
			# mStr: "More? (n or N = no, other = yes) "
                        li $t4, 'M'
                        sb $t4, 35($sp)
                        li $t4, 'o'
                        sb $t4, 36($sp)
                        li $t4, 'r'
                        sb $t4, 37($sp)
                        li $t4, 'e'
                        sb $t4, 38($sp)
                        li $t4, '?'
                        sb $t4, 39($sp)
                        li $t4, ' '
                        sb $t4, 40($sp)
                        li $t4, '('
                        sb $t4, 41($sp)
                        li $t4, 'n'
                        sb $t4, 42($sp)
                        li $t4, ' '
                        sb $t4, 43($sp)
                        li $t4, 'o'
                        sb $t4, 44($sp)
                        li $t4, 'r'
                        sb $t4, 45($sp)
                        li $t4, ' '
                        sb $t4, 46($sp)
                        li $t4, 'N'
                        sb $t4, 47($sp)
                        li $t4, ' '
                        sb $t4, 48($sp)
                        li $t4, '='
                        sb $t4, 49($sp)
                        li $t4, ' '
                        sb $t4, 50($sp)
                        li $t4, 'n'
                        sb $t4, 51($sp)
                        li $t4, 'o'
                        sb $t4, 52($sp)
                        li $t4, ','
                        sb $t4, 53($sp)
                        li $t4, ' '
                        sb $t4, 54($sp)
                        li $t4, 'o'
                        sb $t4, 55($sp)
                        li $t4, 't'
                        sb $t4, 56($sp)
                        li $t4, 'h'
                        sb $t4, 57($sp)
                        li $t4, 'e'
                        sb $t4, 58($sp)
                        li $t4, 'r'
                        sb $t4, 59($sp)
                        li $t4, ' '
                        sb $t4, 60($sp)
                        li $t4, '='
                        sb $t4, 61($sp)
                        li $t4, ' '
                        sb $t4, 62($sp)
                        li $t4, 'y'
                        sb $t4, 63($sp)
                        li $t4, 'e'
                        sb $t4, 64($sp)
                        li $t4, 's'
                        sb $t4, 65($sp)
                        li $t4, ')'
                        sb $t4, 66($sp)
                        li $t4, ' '
                        sb $t4, 67($sp)
                        li $t4, '\0'
                        sb $t4, 68($sp)

			# ieStr: " ints entered, odd-even grouped: "
                        li $t4, ' '
                        sb $t4, 69($sp)
                        li $t4, 'i'
                        sb $t4, 70($sp)
                        li $t4, 'n'
                        sb $t4, 71($sp)
                        li $t4, 't'
                        sb $t4, 72($sp)
                        li $t4, 's'
                        sb $t4, 73($sp)
                        li $t4, ' '
                        sb $t4, 74($sp)
                        li $t4, 'e'
                        sb $t4, 75($sp)
                        li $t4, 'n'
                        sb $t4, 76($sp)
                        li $t4, 't'
                        sb $t4, 77($sp)
                        li $t4, 'e'
                        sb $t4, 78($sp)
                        li $t4, 'r'
                        sb $t4, 79($sp)
                        li $t4, 'e'
                        sb $t4, 80($sp)
                        li $t4, 'd'
                        sb $t4, 81($sp)
                        li $t4, ','
                        sb $t4, 82($sp)
                        li $t4, ' '
                        sb $t4, 83($sp)
                        li $t4, 'o'
                        sb $t4, 84($sp)
                        li $t4, 'd'
                        sb $t4, 85($sp)
                        li $t4, 'd'
                        sb $t4, 86($sp)
                        li $t4, '-'
                        sb $t4, 87($sp)
                        li $t4, 'e'
                        sb $t4, 88($sp)
                        li $t4, 'v'
                        sb $t4, 89($sp)
                        li $t4, 'e'
                        sb $t4, 90($sp)
                        li $t4, 'n'
                        sb $t4, 91($sp)
                        li $t4, ' '
                        sb $t4, 92($sp)
                        li $t4, 'g'
                        sb $t4, 93($sp)
                        li $t4, 'r'
                        sb $t4, 94($sp)
                        li $t4, 'o'
                        sb $t4, 95($sp)
                        li $t4, 'u'
                        sb $t4, 96($sp)
                        li $t4, 'p'
                        sb $t4, 97($sp)
                        li $t4, 'e'
                        sb $t4, 98($sp)
                        li $t4, 'd'
                        sb $t4, 99($sp)
                        li $t4, ':'
                        sb $t4, 100($sp)
                        li $t4, ' '
                        sb $t4, 101($sp)
                        li $t4, '\0'
                        sb $t4, 102($sp)

###################################
#                    int used = 0,
#                        numEven = 0;
			li $v1, 0
			li $t0, 0
#                    hopPtr1 = iArr;
			# iArr replaced with stack base address #
			addi $t1, $sp, 103	
#                    endPtr1 = iArr + 9;
			addi $a1, $t1, 36
begDW1:
#                       cout << inStr << (used + 1);
			li $v0, 4
			# inSstr replaced by string stored on stack #
			move $a0, $sp	
			syscall
			li $v0, 1
			addi $a0, $v1, 1
			syscall
#                       cout << ':' << ' ';
			li $v0, 11
			li $a0, ':'
			syscall
			li $a0, ' '
			syscall
#                       cin >> oneInt;
			li $v0, 5
			syscall
			move $t5, $v0
#                       if ((oneInt & 1) == 0) goto elseI1;
			andi $t9, $t5, 1
			beqz $t9, elseI1
begI1:
			sw $t5, 0($t1)
			addi $t1, $t1, 4
			j endI1
elseI1:
			addi $t0, $t0, 1
			sw $t5, 0($a1)
			addi $a1, $a1, -4
endI1:
			addi $v1, $v1, 1
#                       if (used != 10) goto elseI2;
			li $t9, 10
			bne $v1, $t9, elseI2
begI2:
#                          cout << slrStr << endl;
			li $v0, 4
			# slrSstr replaced by string stored on stack #
			addi $a0, $sp, 7	
			syscall
			li $v0, 11
			li $a0, '\n'
			syscall
#                          reply = 'n';
			li $t3, 'n'
			j endI2
elseI2:
#                          cout << mStr;
			li $v0, 4
			# mSstr replaced by string stored on stack #
			addi $a0, $sp, 35	
			syscall
#                          cin >> reply;
			li $v0, 12
			syscall
			move $t3, $v0
endI2:
endDW1:
DWTest1:
#                    if (reply == 'n') goto xitDW1;
			li $t9, 'n'
			beq $t3, $t9, xitDW1
#                    if (reply != 'N') goto begDW1;
			li $t9, 'N'
			bne $t3, $t9, begDW1
xitDW1:

#                    if (numEven <= 0) goto endI3;
			bltz $t0, endI3
begI3:
			addi $t2, $a1, 4
#                       endPtr2 = iArr + 9;
			# iArr replaced with stack base address #
			addi $a2, $sp, 103	
			addi $a2, $a2, 36
#                       //while (hopPtr2 < endPtr2)
			j WTest1
begW1:
			lw $t7, 0($t2)
			lw $t8, 0($a2)
			sw $t8, 0($t2)
			sw $t7, 0($a2)
			addi $t2, $t2, 4
			addi $a2, $a2, -4
endW1:
WTest1:
#                       if (hopPtr2 < endPtr2) goto begW1;
			blt $t2, $a2, begW1

#                       hopPtr2 = endPtr1 + 1;
			addi $t2, $a1, 4
#                       endPtr2 = iArr + 9;
			# iArr replaced with stack base address #
			addi $a2, $sp, 103	
			addi $a2, $a2, 36
#                       //while (hopPtr2 <= endPtr2)
			j WTest2
begW2:
			lw $t9, 0($t2)
			sw $t9, 0($t1)
			addi $t1, $t1, 4
			addi $t2, $t2, 4
endW2:
WTest2:
#                       if (hopPtr2 <= endPtr2) goto begW2;
			ble $t2, $a2, begW2
endI3:

#                    cout << used << ieStr;
			# extra '\n' to cover syscall #12 drawback #
			li $v0, 11			
			li $a0, '\n'
			syscall
			li $v0, 1
			move $a0, $v1
			syscall
			li $v0, 4
			# ieSstr replaced by string stored on stack #
			addi $a0, $sp, 69	
			syscall
#                    hopPtr1 = iArr;
			# iArr replaced with stack base address #
			addi $t1, $sp, 103	
#                    endPtr1 = hopPtr1 + used;
			sll $a1, $v1, 2
			add $a1, $a1, $t1
#                    //while (hopPtr1 < endPtr1)
			j WTest3
begW3:
#                       if (hopPtr1 >= endPtr1 - 1) goto elseI4;
			addi $t9, $a1, -4
			bge $t1, $t9, elseI4
begI4:
#                          cout << *hopPtr1 << ' ' << ' ';
			li $v0, 1
			lw $a0, 0($t1)
			syscall
			li $v0, 11
			li $a0, ' '
			syscall
			syscall

			j endI4
elseI4:
#                          cout << *hopPtr1 << endl;
			li $v0, 1
			lw $a0, 0($t1)
			syscall
			li $v0, 11
			li $a0, '\n'
			syscall
endI4:
			addi $t1, $t1, 4
endW3:
WTest3:
#                    if (hopPtr1 < endPtr1) goto begW3;
			blt $t1, $a1, begW3

# deallocating memory from the stack
			addiu $sp, $sp, 143

			li $v0, 10
			syscall
			
