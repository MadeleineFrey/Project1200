  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
  # you can write your code for subroutine "hexasc" below this line
  #
  
  #Erica
 .globl time2string
 .globl delay
 .globl enable_interrupt
 
 	.data
	.align 2
	.text
hexasc:	
	andi	$t0, $a0, 0xF	#Igoring the irrelevant bits			

	ble	$t0, 0x9, low
	nop	#if this isnt true it continues to the A-F part
	
	addi 	$v0, $t0, 0x37 	#This is to acces A-F and copying it to the return value
	jr	$ra
	nop
	
low:	
	addi 	$v0, $t0, 0x30	#This is to access 0-9 and copying it to the return value
	jr	$ra
	nop
	

	
time2string:	#time2string		
	PUSH	$ra	#return address
	PUSH	$s0	#save address to from $a0
	PUSH	$s1	#save clock from $a1
	PUSH	$s2	#counter
	PUSH	$s3	#Placement for colon
	PUSH	$s4	#Villkor för X, 1 = X, 0 = inte X
	

	
	move	$s0, $a0	#save value of $a0 (memory address where the converted time information should be stored)
	move	$s1, $a1	#save value of $a1 (the time information e.q. 0x1653
	addi	$s2, $0, 5	#set the counter (counting the memory address pointer)
	addi	$s3, $0, 3	#Where the colon will be (The colon in 15:30)
	add	$s4, $0, $0	#set to zero

	
	andi	$t0, $s1, 0xFF	#masking LSB
	bne	$t0, $0, loop	#If $t0 isnt 0, jump to loop
	addi	$s4, $0, 1	#set to one
	
	
		
loop:	move	$a0, $s1
	jal	hexasc		#$a0 as argument and the return is $v0
	nop
	
	sb	$v0, 4($s0)	#store the byte value to $s0 memory offset 4
	
	addi	$s2, $s2, -1	#dec counter
	addi	$s0, $s0, -1	#dec memory address pointer
	srl	$s1, $s1, 4	#srl 4 bit (one digit)
	
	beq	$s2, $s3, colon	#add the colon by jumping to colon function
	nop
	
	bne	$s2, $0, loop	#restart the loop until the counter is 0
	nop
	
	
	#Insert X
	bne	$s4, 0x1, notx
	addi	$t0, $0, 0x58	#X sign
	sb	$t0, 10($s0)
	addi	$s0, $s0, 1
	
	
notx:	#clear the memory
	move	$t0, $0
loop2:	lb	$t0, 10($s0)	#Load first byte from memory after clock information (10 offset)
	beq	$t0, $0, fin	#If the byte is 0, go to end
	sb	$0, 10($s0)	#10 offset bc 5 bytes of clock information and dec with 5 steps above
	addi	$s0, $s0, 1	#inc by 1
	j	loop2
	nop
	


fin:	POP	$s4
	POP	$s3
	POP	$s2
	POP	$s1
	POP	$s0
	POP	$ra
	jr	$ra
	nop
	
colon:	addi	$t0, $0, 0x3A	#store the colon value in a register
	sb	$t0, 4($s0)	#store the colon in the memory
	
	addi	$s2, $s2, -1	#dec counter
	addi	$s0, $s0, -1	#inc memory address pointer
	
	j	loop
	nop

delay:		
	PUSH	$ra	#return address
	PUSH	$s0	#iteration in for loop
	PUSH	$s1	#ms variable ($a0)
	PUSH	$s2	#The value in the for loop
	
	move	$s1, $a0	#store $a0 to $s1
	addi	$s2, $0, 7750	#Om i == 4710 ($s2] ska for-loopen stanna
	ble	$s1, $0, dfin	#If ms is equal or below 0
	nop
while:	
	addi	$s1, $s1, -1	#ms = ms - 1
	add	$s0, $0, $0	#set iteration to 0
	
for:	
	addi	$s0, $s0, 1	#i++
	bne	$s0, $s2, for	#go to for when i != 4710
	nop	
	bne	$s1, $0, while	#go to while when ms != 0
	nop
	
dfin:	POP	$s2
	POP	$s1
	POP	$s0
	POP	$ra
	jr	$ra	
	nop


enable_interrupt:
	ei
	jr	$ra	