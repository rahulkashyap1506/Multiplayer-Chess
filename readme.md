# Multiplayer Chess

## About

This is a multiplayer chess which contains all the rules of an actual game.

## How to run

You do not need anything other than a C++ compiler. Compile the program and run the .exe file created by it.

## Instructions

1. Select the piece you want to move. The input must contain 2 characters. The first character denotes the type of piece and the second character denotes the specific number of that piece
    1. p - Pawn
    2. r - Rook
    3. n - Knight
    4. b - Bishop
    5. q - Queen
    6. k - King
    
    Example -> p4
    This will select the pawn which is numbered 4.
    
    To confirm the piece you want to move, enter the characters "to". To choose a different piece, simply perform the process mentioned in the beginning.
    
    Once you have selected the piece you want to move, all the possible movements will be displayed. In order to move the piece to your desired location, enter the address you want to move the piece to and the piece will move.
	
	The format of the location is first column{a-h} and then row{1-8}, eg. a4, b5
2. If you want to perform casling, enter 2 chacters. First character should be c, while the second character denotes the side you to perform it on. 0 for left and 1 for right.
3. If you want to perform en passant, enter 2 characters. First caracter sould be e, while the second character denotes the specific number of that piece.
4. If you want to undo the last step, a single character u is sufficient.
5. If you want to resign from the match, enter 'R'.
6. If both player cosider the match to be a draw, enter 'D'.
