	add $sp, $zero, $imm, 1		# 0: set $sp = 1
	sll $sp, $sp, $imm, 11		# 2: set $sp = 1 << 11 = 2048
	add $sp, $sp, $imm, -2		# 4: adjust stack for 3 items
	sw $s2, $sp, $imm, 2		# 6: save $s2
	sw $s1, $sp, $imm, 1		# 8: save $s1
	sw $s0, $sp, $imm, 0		# a: save $s0
	add $s0, $zero, $zero, 0	# b: $s0 = 0, the first index
	add $s2, $zero, $imm, 15	# d: $s2 = n-1 = 15 (length of array)

FirstLoop:
	add $s1, $zero, $zero, 0	# f: $s1 = 0, the second index
	
SecondLoop:
	lw $t0, $imm, $s1, 1024		# 10: $t0 = Memory[1024 + $s1]
	add $t1, $s1, $imm, 1		# 12: $t1 = $s1 + 1
	lw $t2, $imm, $t1, 1024		# 14: $t2 = Memory[1024 + ($s1 + 1)]
	bge $imm, $t0, $t2, Continue # 16: if ($t0 >= $t2) goto Continue
	
	# Swapping
	sw $t2, $imm, $s1, 1024		# 18: Memory[1024 + $s1] = $t2
	sw $t0, $imm, $t1, 1024		# 1a: Memory[1024 + ($s1 + 1)] = $t0
	
Continue:
	add $s1, $s1, $imm, 1		# 1c: $s1++
	sub $t3, $s2, $s0, 0		# 1e: $t3 = n-1-$s0 (second loop max condition)
	blt $imm, $s1, $t3, SecondLoop # 1f: if ($s1 < $t3) goto SecondLoop
	add $s0, $s0, $imm, 1		# 21: $s0++
	blt $imm, $s0, $s2, FirstLoop # 23: if ($s0 < $s2) goto FirstLoop
	halt $zero, $zero, $zero, 0	# 25: halt
	
#array element
.word 1024 246	# F6
.word 1025 352	# 160
.word 1026 27	# 1B
.word 1027 33	# 21
.word 1028 87	# 57
.word 1029 2	# 2
.word 1030 543	# 21F
.word 1031 4325	# 10E5
.word 1032 22	# 16
.word 1033 35	# 23
.word 1034 87	# 57
.word 1035 176	# B0
.word 1036 21	# 15
.word 1037 34	# 22
.word 1038 56	# 38
.word 1039 44	# 2C