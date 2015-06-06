# Tap Lights
LED game implemented and designed from cs120b class. 

### TECHNOLOGY/ COMPONENT:
* AVR Studio 6.1
* Hitachi LCD
* ATMega1284
* TI shift registers
* And miscellaneous parts provided in the UCR-IEEE 120B kit.


### USER GUIDE: 
The game title and version shows up right after you power up the board. 
I’ve given player 1 the privileges to control and move around the menu (player 2 buttons are ignored in menu).
There are 3 buttons each player.
Leftmost button is to move left, middle to select, and rightmost to move right.
Menu List: 1Player, 2Player and High Score. Player one must press the middle key in 1/2Player mode to enter the game. 
1Player: After entering 1 player mode, the user can choose between 2 to 6 (2 being fastest) for the LED speed. 
Right and Left buttons are used to navigate and middle to start the game. 
2Player: The Game Starts right away running in default speed (3). 
EEPROM saves high score for 1 player mode only.
General Rule: Pressing all 3 buttons at the same time quits the game, and score is determined.
Every time an LED hits the last column and eventually falls off and the user doesn’t press anything, the player’s available life is decremented (Each has 5 to start with).
Every correct presses increments the player’s score.
When the life available (regardless of which player) runs out the game ends and the points are compared to highest score.
This is saved to EEPROM if in 1 player mode and congratulates the winning player who had a higher score when in 2 player mode.


SOURCE FILES: 
* taplights.c
* io.c
* io.h
* timer.h.  
NOTE: “final_project_1.5.c” holds all the code used for the final demonstration. 
The rest of the files are just supporting libraries(that I do not own).


REFERENCES: 
* CS120B Lab Materials
* IEEE
* and http://www.societyofrobots.com/member_tutorials/book/export/html/309 for EEPROM.

