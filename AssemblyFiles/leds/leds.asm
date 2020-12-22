	add $t0, $zero, $imm, 6		# 4: set $t0 = 6
	out $imm, $t0, $zero, TIMER	# 6: set irqhandler as L3
	out $imm, $zero, $zero, 1	# 8: enable irq0
	add $t0, $zero, $imm, 1024	# a: set $t0 = 1024
	out $t0, $zero, $imm, 13	# c: set timermax = 1024
	add $t0, $zero, $imm, 1		# e: set $t0 = 1
	add $t3, $zero, $imm, 1		# 10: set $t3 = 1
	add $t1, $zero, $imm, 9		# 14: set $t1 = 9 (leds index)
	out $t0, $zero, $imm, 11	# 12: set timerenable = 1

LOOP:
	in $t2, $zero, $imm, 9		# 44: read leds register into $t2
	add $t0, $zero, $imm, -1 	#     set $t0 to 0xFFFFFFFF
	bne $imm, $t0, $t2, LOOP	#	  if leds not equal to max value continue loop	
	halt $zero, $zero, $zero, 0	# 1e: halt	
	
TIMER:
	in $t2, $zero, $imm, 9		# 44: read leds register into $t2
	sll $t2, $t2, $imm, 1		# 46: left shift led pattern to the left
	or $t2, $t2, $imm, 1		# 48: lit up the rightmost led
	out $t2, $zero, $imm, 9		# 4a: write the new led pattern
	out $zero, $zero, $imm, 3	# 64: clear irq0 status
	reti $zero, $zero, $zero, 0	# 66: return from interrupt