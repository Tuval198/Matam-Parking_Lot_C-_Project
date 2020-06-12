Compilation command (after adding your files):

	g++ -std=c++11 -Wall -Werror -pedantic-errors -o ex3Test *.cpp
	
Running individual tests:

	./ex3Test [TestName]
	
For example:

    ./ex3Test [ParkingLot2]
	
Will run test [ParkingLot2] (Which can be found in the provided file ParkingLotTest.cpp)

Output redirection of the test can be done with "-o outputfilename.txt" flag
In general the test uses catch2 framework, additional usage details can be found here: https://github.com/catchorg/Catch2/blob/master/docs/command-line.md#top