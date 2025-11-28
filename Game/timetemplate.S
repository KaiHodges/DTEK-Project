# timetemplate.S
# Written 2015 by F Lundevall
# Ported 2024/06 by W Szczerek (from MIPS to RISC-V)
# Copyright abandonded - this file is in the public domain.

#############################################################
# Choose the macro syntax for RARS or DTEK-V board.         #
# The syntax for RARS is probably due to its MIPS heritage. #
#############################################################
###################
# Macros for RARS #
###################
#.macro	PUSH (%reg)
#	addi	sp,sp,-4
#	sw	%reg,0(sp) 
#.end_macro
#
#.macro	POP (%reg)
#	lw	%reg,0(sp)
#	addi	sp,sp,4
#.end_macro
###################
# Macros for DTEK #
###################
.macro	PUSH reg
	addi sp,sp,-4
	sw \reg,0(sp) 
.endm

.macro	POP reg
	lw	\reg,0(sp)
	addi	sp,sp,4
.endm
#############################################################

	.data
	.align 2
mytime:	.word 	0x5957
timstr:	.asciz 	"text more text lots of text\0"
	.text
	.globl timetemplate, tick, time2string

# Function for displaying a string with a newline at the end	
display_string:	
	li a7,4
	ecall
	li a0, 10
	li a7,11
	ecall
	jr ra
	
timetemplate:
	la	a0, timstr
	jal     display_string
	
	# wait a little
	li	a0, 1000	# ms
	jal	delay
	
	# call tick
	la 	a0, mytime
	jal	tick
	
	# call your function time2string
	la	a0, timstr
	la      t0, mytime
	lw	a1, 0(t0)
	jal	time2string
	
	# go back and do it all again
	j	timetemplate

	
# tick: update time pointed to by $a0
tick:	lw	t0, 0(a0)	# get time
	addi	t0, t0, 1	# increase
	andi	t1, t0, 0xf	# check lowest digit
	sltiu	t2, t1, 0xa	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x6	# adjust lowest digit
	
	
	andi	t1, t0, 0xf0	# check next digit
	sltiu	t2, t1, 0x60	# if digit < 6, okay
	bnez	t2, tiend
	addi	t0, t0, 0xa0	# adjust digit
	
	
	li	t3, 0xF
	slli	t3, t3, 0x8
	and	t1, t0, t3	# check minute digit
	addi	t3, x0, 0xA
	slli	t3, t3, 0x8
	slt	t2, t1, t3	# if digit < a, okay
	bnez	t2, tiend
	addi	t0, t0, 0x600	# adjust digit - this one's okay, it's lower than 0x7FF 
	
	
	li	t3, 0xF
	slli	t3, t3, 0xC
	and	t1, t0, t3	# check last digit
	addi	t3, x0, 0x6
	slli	t3, t3, 0xC
	slt	t2, t1, t3	# if digit < 6, okay
	bnez	t2, tiend
	
	li	t3, 0xA
	slli	t3, t3, 0xC
	add	t0, t0, t3	# adjust last digit
tiend:	sw	t0,0(a0)	# save updated result
	jr	ra		# return

#########################################################
# Place for your functions: time2string, hex2asc, delay.#
#########################################################

hex2asc:
	li	a0, 12		# test number (from 0 to 15)
	
	addi    sp,sp,-4
	sw      ra,0(sp)
	
	jal	hexasc		# call hexasc
	
	li	a7, 11	# write a0 to stdout
	ecall

	lw      ra,0(sp)
	addi    sp,sp,4
	jr      ra	

  # You can write your own code for hexasc here
  #
  
hexasc: 
	andi a0, a0, 0xF
	
	li  t0, 9
	ble a0, t0, nummerasc #0-9 -> gå till nummerasc
	
	#ascii ska vara bokstav
	
	addi a0, a0, 0x37 #Lägger till 0x37 eftersom 0x37 + 10 = 0x41 = A, 0x37 + 11 = 0x42, etc.
	jr ra  
	
nummerasc: 

	addi a0, a0, 0x30 #Lägg till 0x30 eftersom ascii till x mellan 0-9 är 0x3x 
	
	jr ra
	
delay: 
	#since we do not use any other values than a0, t0,t1 we have no obligations of utalizing the stack

    while:

        ble     a0,zero,notwhile    #equivalent to the while loop
        add    t0,zero,zero    #initialising t0 with value zero
        li    t1,471    #initialising t1 with value 4711

        

        forloop:        #the forloop
            beq    t0,t1,ms    #we check
            addi     t0,t0,1    #same as the i++

        j forloop

        ms:

        addi a0,a0,-1

    j while

    notwhile: 
	
	jr ra
	
time2string: 
	
	# call your function time2string
	#la	a0, timstr
	#la     t0, mytime
	#lw	a1, 0(t0)
	
	PUSH ra 
	PUSH s0
	PUSH s1
	PUSH s2  
	
	add s0, a0, zero
	add s1, a1, zero
	
	
	srli s2, s1, 12   #t2 blir minuten längst till vänster i MM:SS format
	andi s2, s2, 0xF  #Behåll bara relevanta bits
	add  a0, s2, zero 
	jal hexasc        #gör talet till ascii
	sb   a0, 0(s0)
	addi s0, s0, 1 
	
	srli s2, s1, 8   #t2 blir minuten längst till höger i MM:SS format
	andi s2, s2, 0xF  #Behåll bara relevanta bits
	add  a0, s2, zero 
	jal hexasc        #gör talet till ascii
	sb   a0, 0(s0)
	addi s0, s0, 1 
	
	

    	li   s2, 0x3A  # Lägg till ":" kolon tecken 
   	sb   s2, 0(s0)
   	addi s0, s0, 1
	
	srli s2, s1, 4   #t2 blir sekunden längst till höger i MM:SS format
	andi s2, s2, 0xF  #Behåll bara relevanta bits
	add  a0, s2, zero 
	jal hexasc        #gör talet till ascii
	sb   a0, 0(s0)
	addi s0, s0, 1 
	
		  #t2 blir minuten längst till höger i MM:SS format
	andi s2, s1, 0xF  #Behåll bara relevanta bits
	add  a0, s2, zero 
	jal hexasc        #gör talet till ascii
	sb   a0, 0(s0)
	addi s0, s0, 1 
	
	
	# Sista NUL karaktären
	sb x0, 0(s0)
	POP s2
	POP s1
	POP s0
	POP ra 
	jr ra
	
	
	
	
	
	
	
	
	
	 
	 
	
	



