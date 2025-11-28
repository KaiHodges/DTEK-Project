  # hexmain.S
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
	.globl hex2asc

hex2asc:
	li	a0, 10		# test number (from 0 to 15)
	
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
	andi a0, a0, 0x7F #Sätter allt förutom de 7 lsb som ska användas till 0 
	jr ra  
	
nummerasc: 

	addi a0, a0, 0x30 #Lägg till 0x30 eftersom ascii till x mellan 0-9 är 0x3x 
	andi a0, a0, 0x7F #samma som ovan 
	jr ra

	
