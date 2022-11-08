## Brief Description
- This project visualizes the path from a source to a destination based on the OpenFlights dataset. Use flymap class to store the flights information and use emap class to visualize.

## Compile and Run
- To run the code based on current dataset, simply run "make" to compile the code. 
- After compiling, type "./main". You could see the result of traversals and information about shortest path in terminal.
- If you like to test the algorithm based on your own dataset, please put you data into "src" folder and change the file name at main.cpp line 9 to the name of your own file. 
  - To be more specific, it's the bold part you need to change: Flymap(__"src/airports.dat"__, __"src/airlines.dat"__)

## Run Test
- To run the test based on current dataset, simply run "make test" to compile the code.
- After compiling, type "./test" to see the result.

## Extra Info
- The dataset is from [this public dataset](https://openflights.org/data.html "OpenFilghts"). To specify, we construct our project based on information from aiports.dat, airlines.dat, and rountes.dat.  
# Project-Flights
