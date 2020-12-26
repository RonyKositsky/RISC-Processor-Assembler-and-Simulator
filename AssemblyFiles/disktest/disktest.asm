# Load all 4 first sectors from disk
	# Read sector 0
	out $zero, $imm, $zero, 16		# set disk_buffer = 0
	out $zero, $imm, $zero, 15		# set disk_sector to 0
	add $t0, $zero, $imm, 1			# $t0=1
	out $t0, $zero, $imm, 14		# set disc_cmd = 1 (read)
	jal $imm, $zero, $zero, Wait	# Wait for reading all sector
	
	# Read sector 1
	add $t0, $zero, $imm, 0x100		# $t0= 0x100
	out $t0, $imm, $zero, 16		# set disk_buffer = 0x100
	add $t0, $zero, $imm, 1			# $t0=1
	out $t0, $imm, $zero, 15		# set disk_sector to 1
	out $t0, $zero, $imm, 14		# set disc_cmd = 1 (read)
	jal $imm, $zero, $zero, Wait	# Wait for reading all sector
	
	# Read sector 2
	add $t0, $zero, $imm, 0x200		# $t0= 0x200
	out $t0, $imm, $zero, 16		# set disk_buffer = 0x200
	add $t0, $zero, $imm, 2			# $t0=2
	out $t0, $imm, $zero, 15		# set disk_sector to 2
	add $t0, $zero, $imm, 1			# $t0=1
	out $t0, $zero, $imm, 14		# set disc_cmd = 1 (read)
	jal $imm, $zero, $zero, Wait	# Wait for reading all sector

	# Read sector 3
	add $t0, $zero, $imm, 0x300		# $t0= 0x300
	out $t0, $imm, $zero, 16		# set disk_buffer = 0x300
	add $t0, $zero, $imm, 3			# $t0=3
	out $t0, $imm, $zero, 15		# set disk_sector to 3
	add $t0, $zero, $imm, 1			# $t0=1
	out $t0, $zero, $imm, 14		# set disc_cmd = 1 (read)
	jal $imm, $zero, $zero, Wait	# Wait for reading all sector
	
	add $sp, $sp, $imm, -2			# allocate space in stack.
	sw  $s0, $sp, $imm, 0			# save $s0. 
	sw  $s1, $sp, $imm, 1			# save $s0. 
	
	add $s0, $zero, $zero, 0		# $s0 = 0 (counter)
	add $s1, $zero, $imm, 127		# $s1 = 127 (sector_size)
	
# iterate on sector length and calculate xor
Loop:
	bgt $imm, $s0, $s1, End			# if (counter > sector_size) goto End
	lw $t0, $imm, $s0, 0			# get sector 0 data in counter location
	
	# Sector 1
	lw $t1, $imm, $s0, 0x100		# get sector 1 data in counter location
	xor $t0, $t0, $t1, 0			# $t0 = xor($t0, $t1)
	
	# Sector 2
	lw $t1, $imm, $s0, 0x200		# get sector 2 data in counter location
	xor $t0, $t0, $t1, 0			# $t0 = xor($t0, $t1)

	# Sector 3
	lw $t1, $imm, $s0, 0x300		# get sector 3 data in counter location
	xor $t0, $t0, $t1, 0			# $t0 = xor($t0, $t1)

	sw $t0, $imm, $s0, 0x400		# store all xor
	add $s0, $s0, $imm, 1			# $s0++
	beq $imm, $zero, $zero, Loop	# goto Loop

End:
	# Write sector 4
	add $t0, $zero, $imm, 0x400		# $t0= 0x400
	out $t0, $imm, $zero, 16		# set disk_buffer = 0x400
	add $t0, $zero, $imm, 4			# $t0=4
	out $t0, $imm, $zero, 15		# set disk_sector to 4
	add $t0, $zero, $imm, 2			# $t0=2
	out $t0, $zero, $imm, 14		# set disc_cmd = 2 (write)
	jal $imm, $zero, $zero, Wait	# Wait for writing all sector
	
	lw  $s1, $sp, $imm, 1			# restore $s1
	lw  $s0, $sp, $imm, 0			# restore $s0
	add $sp, $sp, $imm, 2			# restore stack
	halt $zero, $zero, $zero, 0		# halt

# wait for disk status
Wait:
	in $t0, $imm, $zero, 17			# get disk_status to $t0
	beq $ra, $zero, $t0, 0			# if (disk_status == 0) go back
	beq $imm, $zero, $zero, Wait	# goto Wait