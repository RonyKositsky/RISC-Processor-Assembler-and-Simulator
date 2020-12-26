	add $t0, $zero, $imm, 6		# 0: set $t0 = 6
	out $imm, $t0, $zero, TIMER	# 2: set irqhandler as L3
	out $imm, $zero, $zero, 1	# 4: enable irq0
	add $t0, $zero, $imm, 1021	# 6: set $t0 = 1021 (1024 - operation before turn led on)
	out $t0, $zero, $imm, 13	# 8: set timermax = 1024
	add $t0, $zero, $imm, 1		# A: set $t0 = 1
	add $t3, $zero, $imm, 1		# C: set $t3 = 1
	out $t0, $zero, $imm, 11	# E: set timerenable = 1

LOOP:
	in $t2, $zero, $imm, 9		# 10: read leds register into $t2
	add $t0, $zero, $imm, -1 	# 12: set $t0 to 0xFFFFFFFF
	bne $imm, $t0, $t2, LOOP	# 14: if leds not equal to max value continue loop	
	halt $zero, $zero, $zero, 0	# 16: halt	
	
TIMER:
	in $t2, $zero, $imm, 9		# 17: read leds register into $t2
	sll $t2, $t2, $imm, 1		# 19: left shift led pattern to the left
	or $t2, $t2, $imm, 1		# 1B: lit up the rightmost led
	out $t2, $zero, $imm, 9		# 1D: write the new led pattern
	out $zero, $zero, $imm, 3	# 1F: clear irq0 status
	add $t2, $zero, $zero, 0 	# 21: dummy command
	reti $zero, $zero, $zero, 0	# 22: return from interrupt