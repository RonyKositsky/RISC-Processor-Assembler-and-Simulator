	
	add $sp, $sp, $imm, -3			# allocate space in stack.
	sw  $s0, $sp, $imm, 0			# save $s0. 
	sw  $s1, $sp, $imm, 1			# save $s1. 
	sw  $s2, $sp, $imm, 2			# save $s2. 
	
	lw $s0, $zero, $imm, 0x100		# Get the circle radius
	mul $s0, $s0, $s0, 0			# $s0 = radius^2
	add $s1, $zero, $imm, 175		# $s1 = 175 (x_axis_center)
	add $s2, $zero, $imm, 143		# $s2 = 143 (y_axis_center)
	
	
for x < 352:
    for y < 288:
		$t0 = x-x0
		$t0^2
		$t1 = y-y0
		$t1^2
		$t0 += $t1
		if ($t0 < radius^2)
			color the pixel.