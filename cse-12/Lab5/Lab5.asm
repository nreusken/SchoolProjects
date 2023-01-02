##########################################################################
# Created by: 	Reusken, Nout
# 		nreusken
# 		5 June 2020
#
# Assignment:	Lab 5: Functions and Graphics
# 		CSE 012, Computer Systems and Assembly Language
# 		UC Santa Cruz, Spring 2020
#
# Description: 	This program includes the fucntions to print circles, solid and normal ones.
#		It also has a function to clear the bitmap with a certain color.
#		Also has a function that can find the color at a certain coordinate.
#
# Notes: 	This program is intended to be run from the MARS IDE.
##########################################################################
#Spring20 Lab5 Template File

# Macro that stores the value in %reg on the stack 
#  and moves the stack pointer.
.macro push(%reg)							#macro to store a register value for later use if the register will be used in another function
	subi $sp $sp 4							#substitute 4 from sp
	sw %reg 0($sp)							#save %reg value in 0(%sp)
.end_macro 								#end macro

# Macro takes the value on the top of the stack and 
#  loads it into %reg then moves the stack pointer.
.macro pop(%reg)							#macro to restore the original value of register stored
	lw %reg 0($sp)							#set %reg to 0(%sp)
	addi $sp $sp 4							#add 4 to sp
.end_macro								#end macro

# Macro that takes as input coordinates in the format
# (0x00XX00YY) and returns 0x000000XX in %x and 
# returns 0x000000YY in %y
.macro getCoordinates(%input %x %y)					#macro to get seperate coordinates from coordinate
	srl %x, %input, 16						#x equals shift right logical 16 of input
	sll %y, %input, 16						#y equals shift left logical 16 of input
	srl %y, %y, 16							#y equals shift right logical 16 of y
.end_macro								#end macro

# Macro that takes Coordinates in (%x,%y) where
# %x = 0x000000XX and %y= 0x000000YY and
# returns %output = (0x00XX00YY)
.macro formatCoordinates(%output %x %y)					#macro to create on coordinate from two registers
	sll, %x, %x, 16							#x equals shift left logical 16 of x
	add %output, %y, %x						#add x and y equal to input
	srl, %x, %x, 16							#x equals shift right logical 16 of x
.end_macro 								#end macro


.data
originAddress: .word 0xFFFF0000

.text
j done
    done: nop
    li $v0 10 
    syscall

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#  Subroutines defined below
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#*****************************************************
#Clear_bitmap: Given a color, will fill the bitmap display with that color.
#   Inputs:
#    $a0 = Color in format (0x00RRGGBB) 
#   Outputs:
#    No register outputs
#    Side-Effects: 
#    Colors the Bitmap display all the same color
#*****************************************************
#pseudocode
#x = 0xFFFF0000
#while x <= 0xFFFFFFFC:
#	bitmap[x] = a0 
#	x += 4
#*****************************************************
clear_bitmap: nop							#clears the bit map to the color set to a0
	la $t0, 0xffff0000						#set register $t0 to 0xffff0000 which is the first pixel on the Bitmap
	individual_pixel:						#loop that goes over all the pixels on the Bitmap
		sw $a0, ($t0)						#sets addres equaling $t0 to what $a0 equals
		add $t0, $t0, 4						#increasse $t0 by 4
		ble $t0, 0xFFFFFFFC, individual_pixel			#if $t0 is less or equal to 0xFFFFFFFC it will continues looping to indvidual_pixel
	jr $ra								#jumps to where the function was called
	
#*****************************************************
# draw_pixel:
#  Given a coordinate in $a0, sets corresponding value
#  in memory to the color given by $a1	
#-----------------------------------------------------
#   Inputs:
#    $a0 = coordinates of pixel in format (0x00XX00YY)
#    $a1 = color of pixel in format (0x00RRGGBB)
#   Outputs:
#    No register outputs
#*****************************************************
#pseudocode
#getCoordinates(a0, x, y)
#z = x * 128
#z = z + y
#z = z + 4
#z = z + 0xFFFF0000
#bitmap[z] = a1
#*****************************************************
draw_pixel: nop								#function that changes a single pixel given by the coordinate in a0 to the color in a1
	getCoordinates($a0, $t0, $t1)					#changes the coordinate in a0 to seperate in two different registers
	mul $t2, $t0, 128						#multiplies $t0 by 128 and sets it to $t2
	add $t2, $t2, $t1						#add $t1 to $t2
	mul $t2, $t2, 4							#mulitply $t2 by 4
	add $t2, $t2, 0xFFFF0000					#add 0xFFFF0000 to $t2 
	sw $a1, ($t2)							#store the appropriate color to the address in $t2
	jr $ra								#jumps to where the functiont was called
	
#*****************************************************
# get_pixel:
#  Given a coordinate, returns the color of that pixel	
#-----------------------------------------------------
#   Inputs:
#    $a0 = coordinates of pixel in format (0x00XX00YY)
#   Outputs:
#    Returns pixel color in $v0 in format (0x00RRGGBB)
#*****************************************************
#pseudcode
#getCoordinates($a0, x, y)
#z = x * 128
#z = z + y
#z = z + 4
#z = z + 0xFFFF0000
#v0 = bitmap[z]
#*****************************************************
get_pixel: nop								#function gets the color of the pixel at the given coordinate
	getCoordinates($a0, $t0, $t1)					#changes the coordinate in a0 to seperate in two different registers
	mul $t2, $t0, 128						#multiplies $t0 by 128 and sets it to $t2
	add $t2, $t2, $t1						#add $t1 to $t2
	mul $t2, $t2, 4							#mulitply $t2 by 4
	add $t2, $t2, 0xFFFF0000					#add 0xFFFF0000 to $t2 
	lw $v0, ($t2)							#stores what is stored at the address stored in $t2, in $v0
	jr $ra								#jumps to where the functiont was called

#***********************************************
# draw_solid_circle:
#  Considering a square arround the circle to be drawn  
#  iterate through the square points and if the point 
#  lies inside the circle (x - xc)^2 + (y - yc)^2 = r^2
#  then plot it.
#-----------------------------------------------------
# draw_solid_circle(int xc, int yc, int r) 
#    xmin = xc-r
#    xmax = xc+r
#    ymin = yc-r
#    ymax = yc+r
#    for (i = xmin; i <= xmax; i++) 
#        for (j = ymin; j <= ymax; j++) 
#            a = (i - xc)*(i - xc) + (j - yc)*(j - yc)	 
#            if (a < r*r ) 
#                draw_pixel(x,y) 	
#-----------------------------------------------------
#   Inputs:
#    $a0 = coordinates of circle center in format (0x00XX00YY)
#    $a1 = radius of the circle
#    $a2 = color in format (0x00RRGGBB)
#   Outputs:
#    No register outputs
#***************************************************
draw_solid_circle: nop							#function draws a solid circle
	push($s0)							#stores $s0 in stack
	push($s1)							#stores $s1 in stack
	push($s2)							#stores $s2 in stack
	push($a0)							#stores $a0 in stack
	getCoordinates($a0, $t0, $t1)					#changes the coordinate in a0 to seperate in two different registers of $t0 and $t1
	sub $t2, $t0, $a1						#subtracts $t0 and $a1 and sets it to $t2
	add $t3, $t0, $a1						#adds $t0 and $a1 and sets it to $t3
	sub $t4, $t1, $a1						#subtracts $t1 and $a1 and sets it to $t4
	add $t5, $t1, $a1						#adds $t1 and $a1 and sets it to $t5
	mul $s2, $a1, $a1						#squares $a1 and sets it to $s2
	la $a1, ($a2)							#sets $a1 to $a2
	addi $t6, $t2, 0						#sets $t6 to $t2
	addi $t7, $t4, 0						#sets $t7 to $t4
	x_loop:								#loops through all possible x values
		la $t7, ($t4)						#sets $t7 to $t4	
		y_loop:							#loops through all the possible y values
			sub $s0, $t6, $t0				#subtracts $t0 from $t6 and sets it to $s0
			sub $s1, $t7, $t1				#subtracts $t1 from $t7 and sets it to $s1
			mul $s0, $s0, $s0				#squares $s0 and sets it $s0
			mul $s1, $s1, $s1				#squares $s1 and sets it $s1
			add $s0, $s0, $s1				#adds $s1 and $s0 and sets it to $s0
			blt $s0, $s2, draw				#if $s0 is less than or equal to $s2 jump to draw
			addi $t7, $t7, 1				#add 1 to $t7 
			bgt $t7, $t5, end				#if $t7 is greater than $t5 jump to end
			j y_loop					#jump to y_loop
			draw:						#function draw
				formatCoordinates($a0, $t6, $t7)	#changes the coordinate in a0 to seperate in two different registers of $t6 and $t7
				push($ra)				#stores $ra in stack
				push($t0)				#stores $t0 in stack
				push($t1)				#stores $t1 in stack
				push($t2)				#stores $t2 in stack
				push($a1)				#stores $s1 in stack
				jal draw_pixel				#jumps to draw pixel
				pop($a1)				#restores $a1 from stack
				pop($t2)				#restores $t2 from stack
				pop($t1)				#restores $t1 from stack
				pop($t0)				#restores $t0 from stack
				pop($ra)				#restores $ra from stack
				addi $t7, $t7, 1			#add 1 to $t7
				bgt $t7, $t5, end			#if $t7 is greater than $t5 jump to end
				j y_loop				#jump to y_loop
		end:							#end point when the y loop is done
		addi $t6, $t6, 1					#add 1 to $t6
		bgt $t6, $t3, x_end					#if $t6 is greater than $t3 jump to x_end
		j x_loop						#jump to x_loop
	x_end:								#end point when the x loop is done
	pop($a0)	 						#restores $a0 from stack
	pop($s2)							#restores $s2 from stack
	pop($s1)							#restores $s1 from stack
	pop($s0)							#restores $s0 from stack
	jr $ra								#jumps back to where the function was called
		
#***********************************************
# draw_circle:
#  Given the coordinates of the center of the circle
#  plot the circle using the Bresenham's circle 
#  drawing algorithm 	
#-----------------------------------------------------
# draw_circle(xc, yc, r) 
#    x = 0 
#    y = r 
#    d = 3 - 2 * r 
#    draw_circle_pixels(xc, yc, x, y) 
#    while (y >= x) 
#        x=x+1 
#        if (d > 0) 
#            y=y-1  
#            d = d + 4 * (x - y) + 10 
#        else
#            d = d + 4 * x + 6 
#        draw_circle_pixels(xc, yc, x, y) 	
#-----------------------------------------------------
#   Inputs:
#    $a0 = coordinates of the circle center in format (0x00XX00YY)
#    $a1 = radius of the circle
#    $a2 = color of line in format (0x00RRGGBB)
#   Outputs:
#    No register outputs
#***************************************************
draw_circle: nop							#function tha draws a cirlce that isn't filled
	push($ra)							#stores $ra in stack
	la $a3, ($a1)							#sets $a3 to $a1
	mul $t3, $a3, 2							#mulitplies $a3 by 2 and sets it to $t3 
	li $t4, 3							#sets $t4 to 3
	sub $t3, $t4, $t3						#subtracts $t3 form $t4 and sets it $t3
	la $a1, ($a2)							#sets $a1 to $a2 
	li $a2, 0							#sets $a2 to 0
	push($t4)							#stores $t4 in stack
	jal draw_circle_pixels						#jumps to draw_cirlce_pixels
	pop($t4)							#restores $t4 from stack
	while:								#while loop
		add $a2, $a2, 1						#increases $a2 by 2
		bgt $t3, 0, d_is_greater				#if $t3 is greater than 0 than jump to d_is_greater	
		mul $t4, $a2, 4						#multiply $a2 times 4 and set it $t4
		add $t3, $t3, $t4					#add $t4 to $t3
		add $t3, $t3, 6						#add 6 to $t3
		push($t4)						#stores $t4 in stack
		jal draw_circle_pixels					#jumps to draw_cirlce_pixels
		pop($t4)						#restores $t4 from stack
		blt $a3, $a2, end_while					#if $a3 is less than $a2 than jump to end_while
		j while							#jump to while
		d_is_greater:						#if statment if d is greater than 0
			sub $a3, $a3, 1					#subtract 1 from $a3
			sub $t4, $a2, $a3				#subtract $a3 from $a2 and set it to $t4
			mul $t4, $t4, 4					#multiply $t4 by 4
			add $t3, $t3, $t4				#add $t4 to $t3
			add $t3, $t3, 10				#increase $t3 by 10
			push($t4)					#stores $t4 in stack
			jal draw_circle_pixels				#jumps to draw_cirlce_pixels
			pop($t4)					#restores $t4 from stack
			blt $a3, $a2, end_while				#if $a3 is less than $a2 than jump to end_while
			j while						#jump to while
	end_while:							#function that 
	pop($ra)							#restores $ra from stack
	jr $ra								#jumps to where the function was called
	
#*****************************************************
# draw_circle_pixels:
#  Function to draw the circle pixels 
#  using the octans' symmetry
#-----------------------------------------------------
# draw_circle_pixels(xc, yc, x, y)  
#    draw_pixel(xc+x, yc+y) 
#    draw_pixel(xc-x, yc+y)
#    draw_pixel(xc+x, yc-y)
#    draw_pixel(xc-x, yc-y)
#    draw_pixel(xc+y, yc+x)
#    draw_pixel(xc-y, yc+x)
#    draw_pixel(xc+y, yc-x)
#    draw_pixel(xc-y, yc-x)
#-----------------------------------------------------
#   Inputs:
#    $a0 = coordinates of circle center in format (0x00XX00YY)
#    $a1 = color of pixel in format (0x00RRGGBB)
#    $a2 = current x value from the Bresenham's circle algorithm
#    $a3 = current y value from the Bresenham's circle algorithm
#   Outputs:
#    No register outputs	
#*****************************************************
draw_circle_pixels: nop						#Function draws the pixels of the circle
	push($ra)						#stores $ra in stack
	getCoordinates($a0, $t6, $t7)				#changes the coordinate in a0 to seperate in two different registers of $t6 and $t7
	add $t4, $t6, $a2					#add $t6 to $a2 and set it to $t4
	add $t5, $t7, $a3					#add $t7 to $a3 and set it to $t4
	push($a0)						#stores $a0 in stack
	formatCoordinates($a0, $t4, $t5)			#changes the coordinate in a0 to seperate in two different registers of $t4 and $t5
	jal draw_pixel						#jumps to draw_pixel to draw the individual pixel
	sub $t4, $t6, $a2					#subtracts $a2 from $t6 and set it to $t4
	add $t5, $t7, $a3					#add $t7 to $a3 and set it to $t5
	formatCoordinates($a0, $t4, $t5)			#changes the coordinate in a0 to seperate in two different registers of $t4 and $t5
	jal draw_pixel						#jumps to draw_pixel to draw the individual pixel
	add $t4, $t6, $a2					#add $t6 to $a2 and set it to $t4
	sub $t5, $t7, $a3					#subtracts $a3 from $t7 and set it to $t5
	formatCoordinates($a0, $t4, $t5)			#changes the coordinate in a0 to seperate in two different registers of $t4 and $t5
	jal draw_pixel						#jumps to draw_pixel to draw the individual pixel
	sub $t4, $t6, $a2					#subtracts $a2 from $t6 and set it to $t4
	sub $t5, $t7, $a3					#subtracts $a3 from $t7 and set it to $t5
	formatCoordinates($a0, $t4, $t5)			#changes the coordinate in a0 to seperate in two different registers of $t4 and $t5
	jal draw_pixel						#jumps to draw_pixel to draw the individual pixel
	add $t4, $t6, $a3					#add $t6 to $a3 and set it to $t4
	add $t5, $t7, $a2					#add $t7 to $a2 and set it to $t5
	formatCoordinates($a0, $t4, $t5)			#changes the coordinate in a0 to seperate in two different registers of $t4 and $t5
	jal draw_pixel						#jumps to draw_pixel to draw the individual pixel
	sub $t4, $t6, $a3					#subtracts $a3 from $t6 and set it to $t4
	add $t5, $t7, $a2					#add $t7 to $a2 and set it to $t5
	formatCoordinates($a0, $t4, $t5)			#changes the coordinate in a0 to seperate in two different registers of $t4 and $t5
	jal draw_pixel						#jumps to draw_pixel to draw the individual pixel
	add $t4, $t6, $a3					#add $t6 to $a3 and set it to $t4
	sub $t5, $t7, $a2					#subtracts $a2 from $t7 and set it to $t5
	formatCoordinates($a0, $t4, $t5)			#changes the coordinate in a0 to seperate in two different registers of $t4 and $t5
	jal draw_pixel						#jumps to draw_pixel to draw the individual pixel
	sub $t4, $t6, $a3					#subtracts $a3 from $t6 and set it to $t4
	sub $t5, $t7, $a2					#subtracts $a2 from $t7 and set it to $t5
	formatCoordinates($a0, $t4, $t5)			#changes the coordinate in a0 to seperate in two different registers of $t4 and $t5
	jal draw_pixel						#jumps to draw_pixel to draw the individual pixel
	pop($a0)						#restores $a0 from stack 
	pop($ra)						#restores $ra from stack
	jr $ra							#jumps back to where the function was called 
