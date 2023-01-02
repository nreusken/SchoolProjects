##########################################################################
# Created by:  Reusken, Nout
#              nreusken
#              24 May 2020
#
# Assignment:  Lab 4: Sorting Integers
#              CSE 12, Computer Systems and Assembly Language
#              UC Santa Cruz, Winter 2020
# 
# Description: This program prints the hexdecimals that were inputed through the program argument. 
#		It also prints the hexidecimals in decimal format, the unosrted and sorted order.
# 
# Notes:       This program is intended to be run from the MARS IDE.
##########################################################################
#Psuedocode
# hex_num = input("Program arguments:\n").split()
# l_dec = []
# sorted_list = []
# #hex to decimal conversion
# for i in hex_num:
#     x = 0
#     length = len(i) - 2
#     j = 0 
#     for z in i:
#         if j == 1:
#             length -= 1
#             if z == 'A':
#                 x += 10 * (16 ** length)
#             elif z == 'B':
#                 x += 11 * (16 ** length)
#             elif z == "C":
#                 x += 12 * (16 ** length)
#             elif z == "D":
#                 x += 13 * (16 ** length)
#             elif z == "E":
#                 x += 14 * (16 ** length)
#             elif z == "F":
#                 x += 15 * (16 ** length)
#             else:
#                 x += int(z) * (16 ** length) 
#         if z == "x":
#             j += 1
#     l_dec.append(x)
# #print integer values
# print("\nInteger values:")
# for i in l_dec:
#     print(i, end=" ")
# print()
# #sorts numbers using bubble sort
# for i in range(len(l_dec)):
#     x = 0
#     t = l_dec[x]
#     y=1
#     while y < len(l_dec):
#         if l_dec[x] > l_dec[y]:
#             t = l_dec[x]
#             l_dec[x] = l_dec[y]
#             l_dec[y] = t      
#	  y += 1
#         x += 1            
# print("\nSorted values:")
# #prints sorted list
# for i in l_dec:
#     print(i, end=" ")
# print()    
##########################################################################
            
.data 
decimal_array: 
	.space 32
.text
main:	
	move $s0, $a0					#moves the pointer of the program arugments to s0 so a0 is opened for syscall
	move $s1, $a1					#moves hex numbers from the program arguments to s1 so a1 is opened for syscalls
	addi $t0, $t0, 0				#sets t0 to 0
	addi $t5, $zero, 0				#sets t5 to 0
	la $a0, program_argument			#loads program _argument in to a0 to be printed
	jal print					#jumps to print function to be printed
prints_hex_numbers:					#function to print individual hex number that inputed at program arguemnts
	beq $t0, $s0, print_unsorted			#if all program arguments are printed goes to print_unsorted function
	lw $t1, ($s1)					#loads hex_ stored at a0 value to t1
	la $a0, ($t1)					#sets t1 to a0 to be printed
	jal print					#jumps to print function to be printed
	addi $s1, $s1, 4				#increases address pointer by 4 to go to next hec number
	addi $t0, $t0, 1				#increase t0 by  1 for printed hex nuumbers
	jal print_space					# jumps to print fucntion to print space 
hex_to_dec:						#function to convert hex to decimal
	lb $t3, 2($t1)					#loads printed hex number without 0x in t3
	beq $t3, 0, store_int				#if t3 equals nul meaning a space it jumps to store_int
	addi $t1, $t1, 1				#increases t1 by one 
	ble $t3, 57, hex_number				#if t3 equal or less than 57 it is a number in ascii and jumps to hex_number
	num_back:					#returning point after function hex_number
	bge $t3, 65, hex_letter				#if t3 equal or greater thann 57 it is a letter in ascii and jumps to hex_letter
	letter_back:					#returning point after function hex_letter
	mul $t4, $t4, 16				#multiplies t4 by 15
	add $t4, $t4, $t3				#add t3 to t4
	j hex_to_dec					#jumps back to hex_to_dec
print_unsorted:						#fucntion to print unsorted decimal numbers
	jal print_enter					#jumps to print_enter fuction to print an enter
	la $a0, integer_statement			#sets a0 to integer_statement
	jal print					#jumps to print function to print a0
	addi $t5, $zero, 0				#sets t5 to 0 which is the pointer for the array
	individual:					#loop to print individual number in decimal_array
		beq $t7, $s0, bubble_sort		#if t7 equal s0 go to bubble sort meaning all the numbers in array are printed
		lw $t6, decimal_array($t5)		#loads the number into t6 from decimal array
		move $a0, $t6				#mvoe t6 to a0
		addi $v0, $0, 1				#set v0 to 1 to print integer
		syscall					#syscall
		jal print_space				#jump to print_space function to print a space
		addi $t5, $t5, 4			#add 4 to t5 to go to next numerb in decimal_array
		addi $t7, $t7, 1			#increase t7 of printer numbers
		j individual				#jump back to individual loop
bubble_sort:						#function to bubble sort decimal_array
	addi $t0, $zero, 0				#set t0 to o
	mul $t5, $s0, 4					#multiply t5 times 4
	while:						#while loop to go throught the array number of times length of the array
		addi, $t0, $t0, 1			#increase t0 by 1
		bgt $t0, $s0, print_sort		#if t0 is greater than s0 go to print sort meaning it has gone lenth of array number of times
		addi $t1, $zero, 0			#sets t1 t0 0
		addi $t2, $zero, 4			#sets t2 to 4
		sort_while:				#loops till the highest number is the last number
			beq $t2, $t5, while		#jumps to while if t2 equals t5
			lw $t3, decimal_array($t1)	#loads number stored at t1 in decimal_array to t3
			lw $t4, decimal_array($t2)	#loads number stored at t2 in decimal_array to t4
			bgt $t3, $t4, swap		#if t3 is greater than t4 it jumps to swap to swap places
			back:				#returning point after function swap
			addi $t1, $t1, 4		#increases t1 by 4
			addi $t2, $t2, 4		#increases t2 by 4
			j sort_while			#jumps to sort_while
		j while					#jumps to while
print_sort:						#function that prints sorted decimal_arrau
	jal print_enter					#jumps to print_enter to print an enter
	addi $t0, $zero, 0				#sets t0 to 0
	la $a0, sorted_statement			#sets a0 to sorted_statement
	jal print					#jupms to print funcction to print what is stored in a0
	sorted_numbers:					#loop to print indvidual sorted numbers in decimal_array
		beq $t0, $t5, exit			#if t0 equals t5 jumps to exit meaning all sorted numebrs are printed
		lw $t1, decimal_array($t0)		#loads number stored at t0 to t1
		move $a0, $t1				#moves t1 to ao to print
		addi $v0, $0, 1				#sets vo to 1 to print an interger
		syscall					#syscall
		jal print_space				#jumps to print_space to print a space
		addi $t0, $t0, 4			#increses t0 by 4
		j sorted_numbers			#jumps to sorted_numbers
exit:							#exit function if everything is printed it exists the code
	jal print_enter					#jumps to print_enter function to print an enter 
	addi $v0, $0, 10				#calls syscall 10 to exit system
	syscall						#executes the syscall
print:							#function if a string needs to be printed
	addi, $v0, $0, 4				#calls syscall 4 to print a string
	syscall						#executes the syscall
	jr, $ra						#returns to where the jal print was called
store_int: 						#function to store the inteeger 
	sw $t4, decimal_array($t5)			#stores t4 to t5 address in decimal_array
	addi $t5, $t5, 4				#increases t5 by 4
	move $t4, $zero					#sets t4 to zero
	j prints_hex_numbers				#jumps to prints_hex_numbers to print next hex number in program arguments
print_space:						#fucntion to print a space
	la $a0, space					#sets a0 to a space 
	addi, $v0, $0, 4				#sets vo to print a string
	syscall						#ayacall	
	jr, $ra						#jumps back to where the fucntion was called
print_enter:						#function to print an enter
	la $a0, enter					#sets a0 to an enter
	addi, $v0, $0, 4				#sets vo to print a string
	syscall 					#ayacall
	jr, $ra						#jumps back to where the fucntion was called
hex_number:						#function to convert a hex number to decimal
	subi $t3, $t3, 48				#converts ascii number to interger
	j num_back					#jumps to num_back
hex_letter:						#function to convert a hex letter to decimal
	subi $t3, $t3, 55				#converts ascii letter to interger
	j letter_back					#jumps to letter_back
swap:							#function to swap place in decimal array
	sw $t3, decimal_array($t2)			#stores t3 in decimal_array at t2 position
	sw $t4, decimal_array($t1)			#stores t4 in decimal_array at t3 position
	j back						#jumps to back
.data
program_argument:
	.asciiz "Program arguments: \n"
integer_statement:
	.asciiz "\nInteger values: \n"
sorted_statement:
	.asciiz "\nSorted values: \n"
space:
	.asciiz " "
enter:
	.asciiz "\n "