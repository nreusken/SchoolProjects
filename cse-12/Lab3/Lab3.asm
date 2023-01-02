##########################################################################
# Created by: Reusken, Nout
# nreusken
# 12 May 2020
#
# Assignment: Lab 3: ASCII-risks (Asterisks)
# CSE 012, Computer Systems and Assembly Language
# UC Santa Cruz, Spring 2020
#
# Description: This program prints variable-sized ASCII diamonds and a sequence of embedded numbers
#		The size is determined by the input of the user.
#
# Notes: This program is intended to be run from the MARS IDE.
##########################################################################
#Pseudocode:
#user_input = 0
#while user_input <= 0:
#    user_input = int(input("Enter the height of the pattern of the pattern (must be grader 0):\t")) 
#    if user_input <= 0:
#        print("Invalid Entry!")
#print()
#z = (user_input - 1) * 2
#l = 1
#f = 1
# = 1
#
#
#for i in range(user_input):
#    for j in range(p):
#        print(l,end="\t") 
#        l += 1
#    for j in range(z):
#        print("*", end="\t")
#   f = l - 1
#   for k in range(p):
#        print(f,end="\t")
#        f -= 1
#    print()
#    z -= 2
#    p += 1
#print()
##########################################################################
.text
main:
	la, $a0, request_number_height		#Print the request for user numebr
	jal print				#jumps to the print function
	li, $v0, 5				#allows for the input from the user
	syscall					#executes the syscall 
	move, $t0, $v0				#store the user's input in to a register
	blez $t0, invalid			#if the input is equal or less jumps to the invalid function
	sub $t1, $t0, 1				#the amount of astrisks for the first row
	mul $t1, $t1, 2				#the amount of astrisks for the first row
	li, $t2, 1				#counter for the numebrs on the right side
	li, $t3, 1				#counter for the numbers on the left side
	li, $t4, 1				#stores the starting number for the left side
	li, $t5, 0				#coutner for the loop for the rows
	la $a0, enter				#stores enter to be printed
	jal print				#prints enter in between input and trianlge 
row:						#loop to print each row
	ble  $t0, $t5, exit			#jumps to exit if the right amount of rows are printed
	li, $t6, 0				#set the counter for the righ side number loop
	jal right_numbers			#jumps to the right_numbers function
	rback:					#returning point after all the right side numbers are printed for the current row
	li, $t6, 0				#set the counter for the left side number loop
	jal stars				#jumps to the stars function
	sback:					#returning point after all the astrisks are printed for the current row
	sub, $t3, $t2, 1 			#sets the right starting left side number of the current row
	li, $t6, 0				#sets the counter for the left side number loop
	jal left_numbers			#jumps to the left numbers function
	lback:					#returning point after all the left side numbers are printed for the current row
	la, $a0, enter				#store enter to be prrinted
	jal print				#jumps to print enter after the row 
	sub, $t1, $t1, 2			#decreases the amount of stars printed for the next row
	addi $t5, $t5, 1			#increases the amount of row printed
	addi $t4, $t4, 1			#increases the amount of numbers printed for each side
	j row					#jumps to beginning of the row loop
right_numbers:					# the loop that prints the right side numbers
	ble $t4, $t6, rback			#if all the right numbers are printed for that row it goes back to the row loop
	addi $t6, $t6, 1			#increases the right side numbers printed for that row 
	move $a0, $t2				#stores the number to be printed
	jal print_int				#jumps to print the number
	la, $a0, tab				#stores a tab to be printed
	jal print				#jumps to print the tab
	addi $t2, $t2, 1			#increases the number that is going to printed next
	j right_numbers				#jumps back to the right numbers function to print the next number
stars:						#function to print the astrisks for the current row
	ble $t1, $t6, sback			#if all the astrisks are printed for that row go back to the row loop
	addi $t6, $t6, 1			#increases the astisks printed for that row
	la, $a0, star				#stores the astrisks to be printed
	jal print				#jumps to print the astrisks
	la, $a0, tab				#stores the tab to be printed
	jal print				#jumps to print the tab
	j stars					#jumps to functions stars to print the next star
left_numbers:					# the loop that prints the left side numbers
	ble $t4, $t6, lback			#if all the left numbers are printed for that row it goes back to the row loop
	addi $t6, $t6, 1			#increases the left  side numbers printed for that row 
	move $a0, $t3				#stores the number to be printed
	jal print_int				#jumps to print the number
	beq $t4, $t6, skip			#if the whole row is printed it jumps to skip to not print a tab at the end
	la, $a0, tab				#stores the tab to be printed
	jal print				#jumps to print the tab
	skip:					#returning point if the whole row was printed
	sub $t3, $t3, 1				#decreases the number that is going to printed next
	j left_numbers				#jumps back to the left numbers function to print the next number
exit:						#exit function if everything is printed it exists the code
	addi $v0, $0, 10			#calls syscall 10 to exit system
	syscall					#executes the syscall
print:						#function if a string needs to be printed
	addi, $v0, $0, 4			#calls syscall 4 to print a string
	syscall					#executes the syscall
	jr, $ra					#returns to where the jal print was called
print_int:					#function if a integer needs to be printed
	addi $v0, $0, 1				#calls syscall 1 to print a string
	syscall					#executes the syscall
	jr, $ra					#returns to where the jal print_int was called
invalid:					#function if the input is equal or less than 0
	addi, $v0, $0, 4			#calls syscall 4 to print a string
	la, $a0, invalid_height			#loads the string stored at invalid_height into the register
	syscall					#executes the syscall
	j main					#jumps back to main to request a new input for the user
.data
request_number_height:
	.asciiz "Enter the height of the pattern of the pattern (must be grader 0):\t"
invalid_height:
	.asciiz "Invalid Entry!\n"
tab:
	.asciiz "\t"
enter:
	.asciiz "\n"
star:
	.asciiz "*"
