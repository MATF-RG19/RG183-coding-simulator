# RG183-coding-simulator

## How the game works
The game is composed of several levels that work on the same principle:
	- There are white and blue tiles on the floor
	- The figure is spawned on one of the tiles
	- Your goal is to "activate" all the blue tiles by entering the sequence of movements and then running them when you're done with the input
	Possible moves (relative to the direction the figure is facing at the beginning of the level) and input:
		- W: figure moves forward one tile  
		- A: figure moves left one tile
		- S: figure moves backwards one tile
		- D: figure moves right one tile
		- E: activate the current tile the figure is on
		- R: resets the level if it was failed
		- C: clear the sequence of movements 
		- ENTER (key): figure performs the sequence of movements (WASDE) given
	- The figure must stay on the path (either white or blue tiles), if the figure wanders off, the level is failed
	
In the lower left corner you will be able to see the current sequence of moves you have entered, if you want to clear the input, press "C" and start again.	

## Adding new levels
Adding new levels is fairly simple, in the "levels.txt" file you can find several groups of numbers. Each group begins with three integers in one line, followed by a certain number of lines. The line with three intergers has the format "<level_number> <number_of_white_tiles> <number_of_blue_tiles>", and after that line there are n + m (number of white and blue tiles respectively) lines denoting the coordinates of the tiles. 
So in order to add a new level, you need to insert the data somewhere before the final level, and change the level numbers appropriately. For example if you wante to insert a level in between levels 4 and 5, you'd need to increment all the <level_number> integers in the rest of the file. 
The other thing you need to do is to take a look at the `set_arena_for_level()` method, you will see a `switch` statemement there with the number of `case` statements equal to the number of levels. Insert a new `case` statement for your new level in the appropriate position (make sure to increment the value for the following levels) and tweak the camera settings as needed. 
That's it - a new level has been added.
