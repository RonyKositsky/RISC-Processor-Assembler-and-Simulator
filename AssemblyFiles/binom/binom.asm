.word 0x100 5
.word 0x101 3

	lw $a0, $zero, $imm, 0x100 	   #$a0 = memory[0x100]
	lw $a0, $zero, $imm, 0x100 	   #$a1 = memory[0x101]
	jal $imm, $zero, $zero, Binom  #start the run
	sw $v0, $imm, $zero, 0x101	   #memory[0x102] = $v0
Binom:
	add $sp, $sp, $imm, -16        #allocate space in stack.
	sw  $ra, $sp, $imm, 0	       #save return address. 
	sw  $a0, $sp, $imm, 4	       #save first argument. 
	sw  $a1, $sp, $imm, 8	       #save second argument. 
	sw  $s0, $sp, $imm, 12	       #save $s0. 
	bne $imm, $a0, $zero ,Else     #if n != 0 go to else
	bne $imm, $a0, $zero, Else     #if n != K go to else
	add $vo, $ao, $imm, 1	       #return value equals 1.
	jal $imm, $zero, $zero Return  #Return
Else:                              
	add $a0, $a0, $imm, -1  	   #$a0 = n-1
	jal $imm, $zero, $zero Binom   #recrusive call to Binom(n-1,k)
	add $s0, $v0, $zero, 0 		   #save Binom(n-1,k) to $s0
	add $a1, $a1, $imm, -1		   #$a1 = k-1
	jal $imm, $zero, $zero, Binome #recrusive call to Binom(n-1,k-1)
	add $vo, $s0, $v0, 0		   #$vo = Binom(n-1,k) + Binom(n-1,k-1)
Return:
	lw  $s0, $sp, $imm, 12		   #restore $s0
	lw  $a1  $sp, $imm, 8		   #restore $a1
	lw  $a0, $sp, $imm, 4		   #restore $a0
	lw  $ra, $sp, $imm, 0		   #restore $ra
	add $sp, $sp, $imm, 12		   #restore stack
	jal $ra, $zero, $zero, 0       #return to caller.
