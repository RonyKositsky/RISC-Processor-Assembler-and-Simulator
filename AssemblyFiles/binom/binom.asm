.word 0x100 5
.word 0x101 3

	lw $a0, $zero, $imm, 0x100 	   # 0: $a0 = memory[0x100]
	lw $a1, $zero, $imm, 0x101 	   # 2: $a1 = memory[0x101]
	jal $imm, $zero, $zero, Binom  # 4: start the run
	sw $v0, $imm, $zero, 0x102	   # 6: memory[0x102] = $v0
	halt $zero, $zero, $zero, 0	   # 8: halt
	
Binom:
	add $sp, $sp, $imm, -4		   # 9: allocate space in stack.
	sw  $ra, $sp, $imm, 0 	       # B: save return address. 
	sw  $a0, $sp, $imm, 1	       # D: save first argument. 
	sw  $a1, $sp, $imm, 2	       # F: save second argument. 
	sw  $s0, $sp, $imm, 3	       # 11: save $s0. 
	beq $imm, $a1, $zero, Return1  # 13: if k != 0 go to else
	beq $imm, $a0, $a1, Return1    # 15: if n != k go to else                            
	add $a0, $a0, $imm, -1  	   # 17: $a0 = n-1
	jal $imm, $zero, $zero, Binom  # 19: recrusive call to Binom(n-1,k)
	add $s0, $v0, $zero, 0 		   # 1B: save Binom(n-1,k) to $s0
	add $a1, $a1, $imm, -1		   # 1C: $a1 = k-1
	jal $imm, $zero, $zero, Binom # 1E: recrusive call to Binom(n-1,k-1)
	add $v0, $s0, $v0, 0		   # 20: $vo = Binom(n-1,k) + Binom(n-1,k-1)
	beq $imm, $zero, $zero, Return2 # 21: goto Return2

Return1:	
	add $v0, $zero, $imm, 1	       # 23: return value equals 1.	
	
Return2:
	lw  $s0, $sp, $imm, 3		   # 25: restore $s0
	lw  $a1, $sp, $imm, 2		   # 27: restore $a1
	lw  $a0, $sp, $imm, 1		   # 29: restore $a0
	lw  $ra, $sp, $imm, 0		   # 2B: restore $ra
	add $sp, $sp, $imm, 4		   # 2D: restore stack
	beq $ra, $zero, $zero, 0       # 2F: return to caller.
