This program runs a game known as Left Right Center(LRC)

The game works like this:
Some number ofkplayers, 1 < k ≤ 10, sit around a table. 
Each player has in her hand $3. There are three dice, and each die has 6 faces and is labeled:  3 × •, 1 × L, 1 × R or 1 × C.  
As a result, we know that there is a 50% chance of rolling •, and 16.66% chance of rolling each of L, R, or C.

	1.  Beginning with player 1, roll the dice:  

		If the player has $3 or more then she rolls three dice; 
		if she has $2 then she rolls two dice; 
		if she has only $1 then she rolls one die; 
		if she has no money then she must pass.

		For each die:
			i.  If the player rolls L then she gives $1 to the player on herleft.
			ii.  If the player rolls R then she gives $1 to the player on herright.
			iii.  If the player rolls C then she puts $1 in the pot in thecenter.
			iv.  If the player rolls • then she ignores it.

	2.  Move to the next player in sequence: to the right.

	3.  Repeat until only one player has any money remaining (who then wins the pot).

Instructions for use of the program using Makefile:

	1. When in directory use Makefile to compile.
		
		type: make
	
	2. When program compiled, run program.
		
		type: ./lrc.c
	
	3. Program should run and prompt:
		
		Random seed: 

	4. Enter the Random seed wanted for the program.

	5. Program should ask what amount players wanted in the game;
	
		How many players?
		
		If not in range in between 0 - 10, program will come up with an error and will ask for a new input.
                This error will continue to pop up untill the number of players is with in range of 0 - 10.

	6. Enter the wanted players in the game and press enter.
		
	7. Game should have started and given a result of who won the game.
