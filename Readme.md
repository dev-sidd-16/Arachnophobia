**README FILE**
Arachnophobia - A 2D game with OpenGL

Name : Siddhant Prakash


================================================================================================================================================

Source Code : The source code of the game named source.cpp is included in the folder.
Makefile : The makefile of the code is also included which takes in source.cpp as input and outputs the compiled executable exeCode.exe


Execution:
----------
To execute the code, first compile it using "make" command on the command prompt.
The code compiles an executable exeCode is generated.

Run the executable by using the command "./exeCode <specs file>" .
A Screen pops up on which the game can be played, using the specifications given in file.

For example, if the specification file is named, world1.txt,
Run " ./exeCode world1.txt", on the shell to start the game.

------------------------------------------------------------------------------------------------------------------------------------------------

Completed aspects of the game:

Required :
----------
1. Create the World with simple representation of objects
	- Baskets : colored cylinders
	- Spiders : colored rectangles
	- Cannon 
	- Lasers

2. Spider boxes are randomly generated with different color and move towards the base with constant speed.

3. The baskets are selectable with the required key format, i.e,
	- 'B' : cannon (default)
	- 'R' : Red Basket
	- 'G' : Green Basket

4. The baskets and cannon move using the left and right key on the keyboard.

5. On hitting 'spacebar' a laser comes out of cannon .

6. Rotation feature specified to the cannon. It can rotate in both direction upto a min-max of (-80,+80) degrees using the required keys, i.e,
	- 'Up key' : Counter Clockwise Rotation
	- 'Down key' : Clockwise Rotation

7. The cannon also behaves in the following way on the click of mouse buttons, as
	- 'Left button' : SHOOTS a laser <DIFFERENT from as specified>
	- 'Right button' : Click and Drag to rotate the Cannon accordingly

8. The laser when hits the world boundary reflects from the boundary and continue its path in accordance with its physics.

9. Points are scored/lost as specified:
    - Hitting a Black Spider : 1 point
    - Scoring a Red Spider in Red basket : 1 point
    - Scoring a Green Spider in Green basket : 1 point
    - Hitting a Green or Red Spider : 0 point
    - Scoring any Spider in a wrong basket( red in green/black in red etc.) : -1 point
    - Any Color Spider reaches base : -3 points
    - Spider falls on Cannon : Game Over !!

9. The spiders on reaching the base stays on the base, and blocks the baskets as well as cannon.

10. The specifications of the world are read using a specification file. The specification file should be of the given format :
    <BOF>
    < The World width >
	< The World height >
    < The Difficulty Level of the Game (1/2/3/4) >
    < The World in which you want to play (1/2/3) >
    <EOF>

	The world feature is extra implementation.

    Minimum World width : 4.0f
    Maximum World width : 10.0f
    Minimum World height : 2.0f
    Maximum World height : 6.0f
    Difficulty Level : 1-4
    World : 1-3

11. To exit the game at any point, press 'Q' or 'Escape key' to finish the game, and again to exit the window.

Extra :
-------

1. Display of running scoreboard.
2. Multiple difficulty levels, specified in the specification file, as well as increases as the game progresses.
3. Multiple worlds to play in, specified in the specification file.
4. Sound plays on firing cannon.
5. Pause/Resume implemented, using 'P' key.
6. On Left Click through mouse, A LASER is fired, giving the complete control of laser to the Mouse, instead of selection of objects and movement.

---------------------------------------------------------------------------------------------------------------------------------------------------
