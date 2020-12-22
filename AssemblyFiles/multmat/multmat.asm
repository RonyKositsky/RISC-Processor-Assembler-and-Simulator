.word 0x100 5
.word 0x101 3
.word 0x102 5
.word 0x103 1
.word 0x104 1
.word 0x105 5
.word 0x106 4
.word 0x107 5
.word 0x108 5
.word 0x109 1
.word 0x10A 5
.word 0x10B 5
.word 0x10C 5
.word 0x10D 3
.word 0x10E 4
.word 0x10F 4
.word 0x110 4
.word 0x111 1
.word 0x112 1
.word 0x113 2
.word 0x114 4
.word 0x115 4
.word 0x116 1
.word 0x117 5
.word 0x118 2
.word 0x119 1
.word 0x11A 5
.word 0x11B 1
.word 0x11C 4
.word 0x11D 2
.word 0x11E 4
.word 0x11F 3

	add  $sp, $sp, $imm, -12      # allocate space in stack.
	sw   $ra, $sp, $imm, 0	      # save return address. 
	sw   $s0, $sp, $imm, 4	      # save $s0. 
	sw   $s1, $sp, $imm, 8	      # save $s1.  
	add  $s0, $zero, $imm, 4      # stop condition for the outer loop 2.
	add  $s1, $zero, $imm, 4      # stop condition for the outer loop 3.
	add  $t0, $zero, $zero, 0	  # set $t0 = 0.
	add  $t1, $zero, $zero, 0	  # set $t1 = 0.
	add  $t2, $zero, $zero, 0	  # set $t2 = 0.
	jal	 $imm, $zero, $zero, For1 # Start run.
	halt $zero, $zero, $zero,0 	  # stop.
For1:                         
	bgt  $imm, $t0, $s0, Return	  # if $t0 > 4 Return.
	add  $t1, $a1, $zero, 0		  # $t0 = $a1
	mult $t1, $t2, $t1, 0		  # $t1 = A[i][0] * B[i][0]
	sw   $t2, $s2, $imm, 0		  # $t2 = C[i][0]
For2:                        
	lw $t1,$s0,$t0,0			  # load $t0'th cell of first matrix to $t1
	lw $t2,$s1,$t0,0			  # load $t0'th cell of second matrix to $t2
	add $t2,$t1,$t2,0			  # perform sum operation and store it in $t2
	sw $t2,$s2,$t0,0			  # save result in output matrix
	add $t0,$t0,$imm,1			  # increase loop index by one
	blt $imm,$t0,$t3,LoopStart    # go back to "LoopStart" if the $t0 index did not reach 16 yet
	halt $zero, $zero, $zero, 0	  # if the jump does not happen($t0 index did reach 16). stop the program
For3:
	lw   $t2, $s2, $imm, 0		 # $t2 = C[i][0]
	add  $t2, $t2, $t1			 # $t2 = C[i][0] + A[i][0] * B[i][0]
Check:
	bge	$imm, $t1, $s1, Return	 # if $t1 >= 4 Return.
Return:
	lw  $s1  $sp, $imm, 8		   #restore $s1
	lw  $s0, $sp, $imm, 4		   #restore $s1
	lw  $ra, $sp, $imm, 0		   #restore $ra
	add $sp, $sp, $imm, 12		   #restore stack
	jal $ra, $zero, $zero, 0       #return to caller.