
CC = g++
OBJS_MTM_PARKING_LOT = MtmParkingLot.o ParkingLotPrinter.o ParkingSpot.o Time.o Car.o ParkingLot.o
EXEC_MTM_PARKING_LOT = MtmParkingLot
OBJS_UNIQUE_ARRAY_TEST = tests/UniqueArrayTest.o
EXEC_UNIQUE_ARRAY_TEST = UniqueArrayTest
COMP_FLAG_MTM_PARKING_LOT = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG
COMP_FLAG_UNIQUE_ARRAY_TEST = -std=c++11 -Wall -Werror -pedantic-errors -g

$(EXEC_MTM_PARKING_LOT) : $(OBJS_MTM_PARKING_LOT)
	$(CC) MtmParkingLot.o ParkingLotPrinter.o ParkingSpot.o Time.o Car.o ParkingLot.o -o $@

$(EXEC_UNIQUE_ARRAY_TEST) : $(OBJS_UNIQUE_ARRAY_TEST)
	$(CC) UniqueArrayTest.o -o $@
	
Car.o: Car.cpp Car.h ParkingLotTypes.h Time.h
	$(CC) $(COMP_FLAG_MTM_PARKING_LOT) -c $*.cpp

MtmParkingLot.o: MtmParkingLot.cpp ParkingLotTypes.h Time.h \
 ParkingLotPrinter.h ParkingSpot.h ParkingLot.h Car.h UniqueArray.h \
 UniqueArrayImp.h
	$(CC) $(COMP_FLAG_MTM_PARKING_LOT) -c $*.cpp
	
ParkingLot.o: ParkingLot.cpp ParkingLot.h ParkingLotTypes.h Time.h \
 ParkingSpot.h Car.h UniqueArray.h UniqueArrayImp.h ParkingLotPrinter.h
	$(CC) $(COMP_FLAG_MTM_PARKING_LOT) -c $*.cpp
	
ParkingLotPrinter.o: ParkingLotPrinter.cpp ParkingLotPrinter.h Time.h \
 ParkingSpot.h ParkingLotTypes.h
	$(CC) $(COMP_FLAG_MTM_PARKING_LOT) -c $*.cpp

ParkingSpot.o: ParkingSpot.cpp ParkingSpot.h ParkingLotTypes.h
	$(CC) $(COMP_FLAG_MTM_PARKING_LOT) -c $*.cpp

Time.o: Time.cpp Time.h
	$(CC) $(COMP_FLAG_MTM_PARKING_LOT) -c $*.cpp
	
tests/UniqueArrayTest.o: tests/UniqueArrayTest.cpp UniqueArray.h \
 UniqueArrayImp.h
	$(CC) $(COMP_FLAG_UNIQUE_ARRAY_TEST) -c $*.cpp

clean:	
	rm -f *.o


 
