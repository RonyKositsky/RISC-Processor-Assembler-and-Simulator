.word 0x100 5
.word 0x101 4
.word 0x102 5
.word 0x103 0
.word 0x104 1
.word 0x105 6
.word 0x106 4
.word 0x107 -2
.word 0x108 2
.word 0x109 4
.word 0x10A 5
.word 0x10B 2
.word 0x10C 5
.word 0x10D 5
.word 0x10E 4
.word 0x10F 4
.word 0x110 6
.word 0x111 1
.word 0x112 7
.word 0x113 2
.word 0x114 -1
.word 0x115 4
.word 0x116 0
.word 0x117 -5
.word 0x118 2
.word 0x119 1
.word 0x11A 5
.word 0x11B 1
.word 0x11C 4
.word 0x11D 2
.word 0x11E 2
.word 0x11F 3

	add  $sp, $sp, $imm, -3       # 0: allocate space in stack.
	sw   $s0, $sp, $imm, 1	      # 2: save $s0. 
	sw   $s1, $sp, $imm, 2	      # 4: save $s1.  
	sw   $s2, $sp, $imm, 3	      # 6: save $s2.  
	
	add  $s0, $zero, $imm, -1     # 8: row_index = -1 for the outer loop 1.
	add  $t3, $zero, $imm, 3	  # A: $t3 = 3 stop condition
	
For1:                         
	add  $s0, $s0, $imm, 1		  # C: row_index++
	bgt  $imm, $s0, $t3, Return	  # E: if row_index > 3 Return.
	add  $s1, $zero, $imm, -1     # 10: col_index = -1 for the outer loop 2.

For2:
	add  $s1, $s1, $imm, 1		  # 12: col_index++
	bgt  $imm, $s1, $t3, For1	  # 14: if col_index > 3 Return.
	add  $s2, $zero, $imm, -1     # 16: index = -1 for the outer loop 3.

For3:
	add  $s2, $s2, $imm, 1		  # 18: index++
	bgt  $imm, $s2, $t3, For2	  # 1A: if index > 3 Return.
	
	# multiply logic
	sll $t0, $s0, $imm, 2		  # 1C: $t0 = row_index * 4
	
	# A matrix
	add $t1, $s2, $t0, 0		  # 1E: $t1 = index + $t0
	lw $t1, $imm, $t1, 0x100	  # 1F: $t1 = A[row_index][index]
	
	# B matrix
	sll $t2, $s2, $imm, 2		  # 21: $t2 = index * 4
	add $t2, $t2, $s1, 0		  # 23: $t2 += col_index
	lw $t2, $imm, $t2, 0x110	  # 24: $t2 = B[index][col_index]
	
	mul $t1, $t1, $t2, 0		  # 26: $t1 = A[row_index][index] * B[index][col_index]
	
	# C matrix
	add $t2, $s1, $t0, 0		  # 27: $t2 = row_index*4 + col_index
	lw $t0, $imm, $t2, 0x120	  # 28: $t0 = C[row_index][col_index]
	
	add $t0, $t0, $t1, 0		  # 2A: $t0 += multiply
	sw $t0, $t2, $imm, 0x120	  # 2B: C[row_index][col_index] = new value
		
	beq $imm, $zero, $zero, For3  # 2D: goto For3
		
Return:
	lw  $s2, $sp, $imm, 2		  # 2F: restore $s2
	lw  $s1, $sp, $imm, 1		  # 31: restore $s1
	lw  $s0, $sp, $imm, 0		  # 33: restore $s1
	add $sp, $sp, $imm, 3		  # 35: restore stack
	halt $zero, $zero, $zero, 0	  # 37: halt