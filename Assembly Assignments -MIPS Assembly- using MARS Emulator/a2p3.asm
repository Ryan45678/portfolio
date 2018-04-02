###############################################################################
# Title: Assign02P3                   Author: Ryan Askew
# Class: CS 2318-00?, Fall 2016       Submitted: 11/01/16
###############################################################################
# Program: MIPS tranlation of a given C++ program
###############################################################################
# Pseudocode description: supplied a2p2_SampSoln.cpp
###############################################################################

			.data
#int a1[12],
a1:			.space 48
#    a2[12],
a2:			.space 48
#    a3[12];
a3:			.space 48
#char einStr[]    = "Enter integer #";
einStr:			.asciiz "\nEnter integer #"
#char moStr[]     = "Max of ";
moStr:			.asciiz "Max of "
#char ieStr[]     = " ints entered...";
ieStr:			.asciiz " ints entered..."
#char emiStr[]    = "\n\nEnter more ints? (n or N = no, others = yes) ";
emiStr:			.asciiz "Enter more ints? (n or N = no, others = yes) "
#char begA1Str[]  = "beginning a1: ";
begA1Str:		.asciiz "beginning a1: "
#char procA1Str[] = "processed a1: ";
procA1Str:		.asciiz "processed a1: "
#char commA2Str[] = "          a2: ";
commA2Str:		.asciiz "          a2: "
#char commA3Str[] = "          a3: ";
commA3Str:		.asciiz "          a3: "
#char dacStr[]    = "Do another case? (n or N = no, others = yes) ";
dacStr:			.asciiz "Do another case? (n or N = no, others = yes) "
#char dlStr[]     = "================================";
dlStr:			.asciiz "\n================================"
#char byeStr[]    = "bye...";
byeStr:			.asciiz "\nbye..."
#int main()
#{
			.text
			.globl main
main:
               #char reply;
               #int used1,
                   #used2,
                   #used3,
                   #target,
                   #total,
                   #mean,
                   #*hopPtr1,
                   #*hopPtr2,
                   #*hopPtr21,
                   #*hopPtr3,
                   #*endPtr1,
                   #*endPtr2,
                   #*endPtr3;

               #cout << endl;
               li $v0, '\n'
               #reply = 'y';
               li $v1, 'y'
               
               #//while (reply != 'n' && reply != 'N')
               #goto WTest1;
               j WTest1
begW1:       #{
                  #used1 = 0;
                  add $t1, $0, $0
                  #used2 = 0;
                  add $t2, $0, $0
                  #used3 = 0;
                  add $t3, $0, $0
                  #hopPtr1 = a1;
                  la $t5, a1
                  #//while (reply != 'n' && reply != 'N')
                  #goto WTest2;
                  j WTest2
begW2:          #{
                     #cout << einStr;
                     li $v0, 4
                     la $a0, einStr
                     syscall
                     #cout << (used1 + 1);
                     li $v0, 1
                     addi $t0, $t1, 1
                     move $a0, $t0
                     syscall
                     #cout << ':' << ' ';
                     li $v0, 11
                     li $a0, ':'
                     syscall
                     li $a0, ' '
                     syscall
                     #cin >> *hopPtr1;
                     li $v0, 5
                     syscall
                     lw $t0, 0($t5)
                     sw $v0, 0($t5)
                     #++used1;
                     addi $t1, $t1, 1
                     #++hopPtr1;
                     addi $t5, $t5, 4
                     #//if (used1 < 12)
                     #if (used1 >= 12) goto else1;
                     addi $t0, $0, 12
                     bge $t1, $t0, else1
begI1:             #{
                        #cout << emiStr;
                        li $v0, 4
                        la $a0, emiStr
                        syscall
                        #cin >> reply;
                        li $v0, 12
                        syscall
                        move $v1, $v0
                     #goto endI1;
                     j endI1
                   #}
else1:             #else
                   #{
                        #cout << moStr << 12 << ieStr << endl;
                        li $v0, 4
                        la $a0, moStr
                        syscall
                        li $v0, 1
                        addi $a0, $0, 12
                        syscall
                        li $v0, 4
                        la $a0, ieStr
                        syscall
                        li $v0, 11
                        li $a0, '\n'
                        syscall
                        #reply = 'n';
                        li $t0, 'n'
                        move $v1, $t0
endI1:             #}
WTest2:         #}
                  #//if (reply != 'n' && reply != 'N') goto begW2;
                  #if (reply == 'n') goto xitW2;
                  li $t0, 'n'
                  beq $t0, $v1, xitW2
                  #if (reply != 'N') goto begW2;
                  li $t0, 'N'
                  bne $t0, $v1, begW2
xitW2:
                  #cout << endl;
		  li $v0, 11
		  li $a0, '\n'
		  syscall

                  #//if (used1 > 0)
                  #if (used1 <= 0) goto endI2;
                  blez $t1, endI2
begI2:           #{
                     #total = 0;
                     add $t9, $0, $0
                     #//for (hopPtr1 = a1, endPtr1 = a1 + used1; hopPtr1 < endPtr1; ++hopPtr1)
                     #hopPtr1 = a1;
                     la $t5, a1
                     #endPtr1 = a1 + used1;
                     sll $t0, $t1, 2
                     add $a1, $t5, $t0
                     #goto FTest1;
                     j FTest1
begF1:              #{
                        #target = *hopPtr1;
                        lw $t4, 0($t5)
                        #total += target;
                        add $t9, $t9, $t4
                        #//if (target % 2 == 1)
                        #////if (target % 2 != 1) goto else3;
                        #if ( (target & 1) != 1) goto else3;
                        andi $t0, $t4, 1
                        addi $v0, $0, 1
                        bne $t0, $v0, else3
begI3:                 #{
                           #hopPtr3 = a3 + used3 - 1;
                           la $t0, a3
                           add $t7, $t0, $0
                           add $t0, $t3, $0
                           sll, $t0, $t0, 2
                           add $t7, $t7, $t0
                           addi $t7, $t7, -4
                           #endPtr3 = a3;
                           add $a3, $t0, $0
                           #//while (hopPtr3 >= endPtr3)
                           #goto WTest3;
                           j WTest3
begW3:                    #{
                              #//if (*hopPtr3 > target)
                              #if (*hopPtr3 <= target) goto else4;
                              lw $t0, 0($t7)
                              ble $t0, $t4, else4
begI4:                       #{
                                 #*(hopPtr3 + 1) = *hopPtr3;
                                 lw $t0, 0($t7)
                                 sw $t0, 4($t7)
                                 #--hopPtr3;
                                 addi $t7, $t7, -4
                              #goto endI4;
                              j endI4
                             #}
else4:                       #else
                             #{
                                 #//break;
                                 #goto xitW3;
                                 j xitW3
endI4:                       #}
WTest3:                   #}
                           #if (hopPtr3 >= endPtr3) goto begW3;
                           bge $t7, $a3, begW3
xitW3:
                           #*(hopPtr3 + 1) = target;
                           sw $t4, 4($t7)
                           
                           #++used3;
                           addi $t3, $t3, 1
                        #goto endI3;
                        j endI3
#//                     }
else3:                  #else
#//                     {
                           #hopPtr2 = a2;
                           la $t6, a2
                           #endPtr2 = a2 + used2;
                           sll $t0, $t2, 2
                           add $a2, $t6, $t0
                           #//while (hopPtr2 < endPtr2)
                           #goto WTest4;
                           j WTest4
begW4:                    #{
                              #//if (*hopPtr2 >= target)
                              #if (*hopPtr2 < target) goto else5;
                              lw $t0, 0($t6)
                              blt $t0, $t4, else5
begI5:                       #{
                                 #hopPtr21 = endPtr2;
                                 add $t8, $a2, $0
                                 #///while (hopPtr21 > hopPtr2)
                                 #goto WTest5;
                                 j WTest5
begW5:                          #{
                                    #*hopPtr21 = *(hopPtr21 - 1);
                                    lw $t0, -4($t8)
                                    sw $t0, 0($t8)
                                    #--hopPtr21;
                                    addi $t8, $t8, -4
WTest5:                         #}
                                 #if (hopPtr21 > hopPtr2) goto begW5;
                                 bgt $t8, $t6, begW5

                                 #///break;
                                 #goto xitW4;
                                 j xitW4
                           #///goto endI5; // unreacheable
                             #}
else5:                       #else
                             #{
                                 #++hopPtr2;
                                 addi $t6, $t6, 4
endI5:                       #}
WTest4:                   #}
                           #if (hopPtr2 < endPtr2) goto begW4;
                           blt $t6, $a2, begW4
xitW4:
                           #*hopPtr2 = target;
                           sw $t4, 0($t6)
                           #++used2;
                           addi $t2, $t2, 1
endI3:                 #}
                        #mean = total/used1;
                        div $t8, $t9, $t1
                     #++hopPtr1;
                     addi $t5, $t5, 4
FTest1:             #}
                     #if (hopPtr1 < endPtr1) goto begF1;
                     blt $t5, $a1, begF1
                     #cout << begA1Str;
                     li $v0, 4
                     la $a0, begA1Str
                     syscall
                     #//if (used1 > 0)
                     #if (used1 <= 0) goto endI6;
                     blez $t1, endI6
begI6:              #{
                        #hopPtr1 = a1;
                        la $t5, a1
                        #endPtr1 = a1 + used1;
                        sll $t0, $t1, 2
                        add $a1, $t5, $t0
                       #do
begDW1:                #{
                           #cout << *hopPtr1 << ' ' << ' ';
                           li $v0, 1
                           lw $a0, 0($t5)
                           syscall
                           li $v0, 11
                           li $a0, ' '
                           syscall
                           syscall
                           #++hopPtr1;
                           addi $t5, $t5, 4
DWTest1:               #}
                        #//while (hopPtr1 < endPtr1);
                        #if (hopPtr1 < endPtr1) goto begDW1;
                        blt $t5, $a1, begDW1
endI6:              #}
                     #cout << endl;
                     li $v0, 11
                     li $a0, '\n'
                     syscall

                     #cout << commA2Str;
                     li $v0, 4
                     la $a0, commA2Str
                     syscall
                     #//if (used2 > 0)
                     #if (used2 <= 0) goto endI7;
                     blez $t2, endI7
begI7:              #{
                        #hopPtr2 = a2;
                        la $t6, a2
                        #endPtr2 = a2 + used2;
                        sll $t0, $t2, 2
                        add $a2, $t0, $t6
                       #do
begDW2:                #{
                           #cout << *hopPtr2 << ' ' << ' ';
                           li $v0, 1
                           lw $a0, 0($t6)
                           syscall
                           li $v0, 11
                           li $a0, ' '
                           syscall
                           syscall
                           #++hopPtr2;
                           addi $t6, $t6, 4
DWTest2:               #}
                        #//while (hopPtr2 < endPtr2);
                        #if (hopPtr2 < endPtr2) goto begDW2;
                        blt $t6, $a2, begDW2
endI7:              #}
                     #cout << endl;
                     li $v0, 11
                     li $a0, '\n'
                     syscall

                     #cout << commA3Str;
                     li $v0, 4
                     la $a0, commA3Str
                     syscall
                     #//if (used3 > 0)
                     #if (used3 <= 0) goto endI8;
                     blez $t3, endI8
begI8:              #{
                        #hopPtr3 = a3;
                        la $t7, a3
                        #endPtr3 = a3 + used3;
                        sll $t0, $t3, 2
                        add $a3, $t7, $t0
                       #do
begDW3:                #{
                           #cout << *hopPtr3 << ' ' << ' ';
                           li $v0, 1
                           lw $a0, 0($t7)
                           syscall
                           li $v0, 11
                           li $a0, ' '
                           syscall
                           syscall
                           #++hopPtr3;
                           addi $t7, $t7, 4
DWTest3:               #}
                        #//while (hopPtr3 < endPtr3);
                        #if (hopPtr3 < endPtr3) goto begDW3;
                        blt $t7, $a3, begDW3
endI8:              #}
                     #cout << endl;
                     li $v0, 11
                     li $a0, '\n'
                     syscall
                     #hopPtr1 = a1;
                     la $t5, a1
                     #hopPtr2 = a2;
                     la $t6, a2
                     #hopPtr3 = a3;
                     la $t7, a3
                     #endPtr2 = a2 + used2;
                     sll $t0, $t2, 2
                     add $a2, $t0, $t6
                     #endPtr3 = a3 + used3;
                     sll $t0, $t3, 2
                     add $a3, $t0, $t7
                     #//while (hopPtr2 < endPtr2 && hopPtr3 < endPtr3)
                     #goto WTest6;
                     j WTest6
begW6:              #{
                        #//if (*hopPtr2 < *hopPtr3)
                        #if (*hopPtr2 >= *hopPtr3) goto else9;
                        lw $t0, 0($t6)
                        lw $v0, 0($t7)
                        bge $t0, $v0, else9
begI9:                 #{
                           #*hopPtr1 = *hopPtr2;
                           lw $t0, 0($t6)
                           sw $t0, 0($t5)
                           #++hopPtr2;
                           addi $t6, $t6, 4
                        #goto endI9;
                        j endI9
                       #}
else9:                 #else
                       #{
                           #*hopPtr1 = *hopPtr3;
                           lw $t0, 0($t7)
                           sw $t0, 0($t5)
                           #++hopPtr3;
                           addi $t7, $t7, 4
endI9:                 #}
                        #++hopPtr1;
                        addi $t5, $t5, 4
WTest6:             #}
                     #////if (hopPtr2 < endPtr2 && hopPtr3 < endPtr3) goto begW6;
                     #if (hopPtr2 >= endPtr2) goto xitW6;
                     bge $t6, $a2, xitW6
                     #if (hopPtr3 < endPtr3) goto begW6;
                     blt $t7, $a3, begW6
xitW6:
                     #//while (hopPtr2 < endPtr2)
                     #goto WTest7;
                     j WTest7
begW7:              #{
                        #*hopPtr1 = *hopPtr2;
                        lw $t0, 0($t6)
                        sw $t0, 0($t5)
                        #++hopPtr2;
                        addi $t6, $t6, 4
                        #++hopPtr1;
                        addi $t5, $t5, 4
WTest7:             #}
                     #if (hopPtr2 < endPtr2) goto begW7;
                     blt $t6, $a2, begW7

                     #//while (hopPtr3 < endPtr3)
                     #goto WTest8;
                     j WTest8
begW8:              #{
                        #*hopPtr1 = *hopPtr3;
                        lw $t0, 0($t7)
                        sw $t0, 0($t5)
                        #++hopPtr3;
                        addi $t7, $t7, 4
                        #++hopPtr1;
                        addi $t5, $t5, 4
WTest8:             #}
                     #if (hopPtr3 < endPtr3) goto begW8;
                     blt $t7, $a3, begW8

                     #hopPtr1 = a1;
                     la $t5, a1
                     #hopPtr2 = a2;
                     la $t6, a2
                     #hopPtr3 = a3;
                     la $t7, a3
                     #endPtr1 = a1 + used1;
                     sll $t0, $t1, 2
                     add $a1, $t0, $t5
                     #used2 = 0;
                     add $t2, $0, $0
                     #used3 = 0;
                     add $t3, $0, $0
                     #//while (hopPtr1 < endPtr1)
                     #goto WTest9;
                     j WTest9
begW9:              #{
                        #target = *hopPtr1;
                        lw $t0, 0($t5)
                        add $t4, $t0, $0
                        #//if (target < mean)
                        #if (target >= mean) goto else10;
                        bge $t4, $t8, else10
begI10:                #{
                           #*hopPtr2 = target;
                           sw $t4, 0($t6)
                           #++used2;
                           addi $t2, $t2, 1
                           #++hopPtr2;
                           addi $t6, $t6, 4
                        #goto endI10;
                        j endI10
                       #}
else10:                #else
                       #{
                           #//if (target > mean)
                           #if (target <= mean) goto endI11;
                           ble $t4, $t8, endI11
begI11:                   #{
                              #*hopPtr3 = target;
                              sw $t4, 0($t7)
                              #++used3;
                              addi $t3, $t3, 1
                              #++hopPtr3;
                              addi $t7, $t7, 4
endI11:                   #}
endI10:                #}
                        #++hopPtr1;
                        addi $t5, $t5, 4
WTest9:             #}
                     #if (hopPtr1 < endPtr1) goto begW9;
                     blt $t5, $a1 begW9

                     #cout << procA1Str;
                     li $v0, 4
                     la $a0, procA1Str
                     syscall
                     #//if (used1 > 0)
                     #if (used1 <= 0) goto endI12;
                     blez $t1, endI12
begI12:             #{
                        #hopPtr1 = a1;
                        la $t5, a1
                        #endPtr1 = a1 + used1;
                        sll $t0, $t1, 2
                        add $a1, $t0, $t5
                        #//do
begDW4:                #{
                           #cout << *hopPtr1 << ' ' << ' ';
                           li $v0, 1
                           lw $a0, 0($t5)
                           syscall
                           li $v0, 11
                           li $a0, ' '
                           syscall
                           syscall
                           #++hopPtr1;
                           addi $t5, $t5, 4
DWTest4:                #}
                        #//while (hopPtr1 < endPtr1);
                        #if (hopPtr1 < endPtr1) goto begDW4;
                        blt $t5, $a1, begDW4
endI12:             #}
                     #cout << endl;
                     li $v0, 11
                     li $a0, '\n'
                     syscall

                     #cout << commA2Str;
                     li $v0, 4
                     la $a0, commA2Str
                     syscall
                     #//if (used2 > 0)
                     #if (used2 <= 0) goto endI13;
                     blez $t2, endI13
begI13:             #{
                        #hopPtr2 = a2;
                        la $t6, a2
                        #endPtr2 = a2 + used2;
                        sll $t0, $t2, 2
                        add $a2, $t0, $t6
                        #//do
begDW5:                #{
                           #cout << *hopPtr2 << ' ' << ' ';
                           li $v0, 1
                           lw $a0, 0($t6)
                           syscall
                           li $v0, 11
                           li $a0, ' '
                           syscall
                           syscall
                           #++hopPtr2;
                           addi $t6, $t6, 4
DWTest5:                #}
                        #//while (hopPtr2 < endPtr2);
                        #if (hopPtr2 < endPtr2) goto begDW5;
                        blt $t6, $a2, begDW5
endI13:             #}
                     #cout << endl;
                     li $v0, 11
                     li $a0, '\n'
                     syscall

                     #cout << commA3Str;
                     li $v0, 4
                     la $a0, commA3Str
                     syscall
                     #//if (used3 > 0)
                     #if (used3 <= 0) goto endI14;
                     blez $t3, endI14
begI14:             #{
                        #hopPtr3 = a3;
                        la $t7, a3
                        #endPtr3 = a3 + used3;
                        sll $t0, $t3, 2
                        add $a3, $t0, $t7
                        #//do
begDW6:                #{
                           #cout << *hopPtr3 << ' ' << ' ';
                           li $v0, 1
                           lw $a0, 0($t7)
                           syscall
                           li $v0, 11
                           li $a0, ' '
                           syscall
                           syscall
                           #++hopPtr3;
                           addi $t7, $t7, 4
DWTest6:                #}
                        #//while (hopPtr3 < endPtr3);
                        #if (hopPtr3 < endPtr3) goto begDW6;
                        blt $t7, $a3, begDW6
endI14:             #}
                     #cout << endl;
                     li $v0, 11
                     li $a0, '\n'
                     syscall
endI2:           #}
                  #cout << endl;
                  li $v0, 11
                  li $a0, '\n'
                  syscall
                  #cout << dacStr;
                  li $v0, 4
                  la $a0, dacStr
                  syscall
                  #cin >> reply;
                  li $v0, 12
                  syscall
                  move $v1, $v0
                  #cout << endl;
                  li $v0, 11
                  li $a0, '\n'
                  syscall
WTest1:      #}
               #//if (reply != 'n' && reply != 'N') goto begW1;
               #if (reply == 'n') goto xitW1;
               li $v0, 'n'
               beq $v0, $v1, xitW1
               #if (reply != 'N') goto begW1;
               li $v0, 'N'
               bne $v0, $v1, begW1
xitW1:
               #cout << dlStr << '\n';
               li $v0, 4
               la $a0, dlStr
               syscall
               #cout << byeStr << '\n';
               la $a0, byeStr
               syscall
               #cout << dlStr << '\n';
	       la $a0, dlStr
	       syscall
               #return 0;
               li $v0, 10
               syscall
#}

