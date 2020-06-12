#include "ParkingLot.h"
#include "UniqueArray.h"
#include "ParkingLotPrinter.h"
#include <vector>
#include <algorithm>


namespace MtmParkingLot {
    using namespace ParkingLotUtils;

    const unsigned int HOURS_TO_FINE = 24;

    ParkingLot::ParkingLot(unsigned int *parkingBlockSizes):
    motorbike_slots(parkingBlockSizes[MOTORBIKE]),
    handicap_slots(parkingBlockSizes[HANDICAPPED]),
    car_slots(parkingBlockSizes[CAR]){}

    static bool CheckIfAlreadyParked(Car car,
            UniqueArray<Car,CarCompare> block, VehicleType block_type){
        //this help function gets car and parking block (as UA), if the car is
        //already parked in this block it prints the corresponding message and
        //returns true
        unsigned int index;
        if(block.getIndex(car,index)){
            Car* parked_car = block.getByIndex(index);
            ParkingSpot parking_spot(block_type, index);
            //we have to print the entrence time for the car that is already in
            ParkingLotPrinter::printVehicle(std::cout,
                                        parked_car->getVehicleType(),
                                        parked_car->getLicensePlate(),
                                        parked_car->getEntranceTime());
            ParkingLotPrinter::printEntryFailureAlreadyParked(std::cout,
                                                               parking_spot);
            return true;
        }
        return false;
    }

    ParkingResult ParkingLot::insertHandicap(const Car new_car) {
        //can park neither in handicap or car slots
        unsigned int index;
        VehicleType located_in = HANDICAPPED;
        //will hold the block given to the handicapped
        if(CheckIfAlreadyParked(new_car, handicap_slots, HANDICAPPED) ||
           CheckIfAlreadyParked(new_car, car_slots, CAR)||
                CheckIfAlreadyParked(new_car, motorbike_slots, MOTORBIKE)){
            //if already parked everything is printed in the check function
            return VEHICLE_ALREADY_PARKED;
        }else if(handicap_slots.getCount() != handicap_slots.getSize()){
            index = handicap_slots.insert(new_car);
            located_in = HANDICAPPED;

        }else if(car_slots.getCount() != car_slots.getSize()){
            //there is no room in handicapped slot so we put in a car slot
            index = car_slots.insert(new_car);
            located_in = CAR;
        }else{
            ParkingLotPrinter::printVehicle(std::cout, new_car.getVehicleType(),
                    new_car.getLicensePlate() , new_car.getEntranceTime());
            ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
            return NO_EMPTY_SPOT;
        }
        //if we are here, succesfuly inserted
        ParkingLotPrinter::printVehicle(std::cout, new_car.getVehicleType(),
                new_car.getLicensePlate() , new_car.getEntranceTime());
        ParkingLotPrinter::printEntrySuccess(std::cout,
                ParkingSpot(located_in,index));
        return SUCCESS;
    }

    ParkingResult ParkingLot::insertMotorbike(const Car new_car) {
        unsigned int index;

        if(CheckIfAlreadyParked(new_car, handicap_slots, HANDICAPPED) ||
           CheckIfAlreadyParked(new_car, car_slots, CAR)||
           CheckIfAlreadyParked(new_car, motorbike_slots, MOTORBIKE)) {
            //if already parked everything is printed in the check function
            return VEHICLE_ALREADY_PARKED;

        }else if(motorbike_slots.getCount()==motorbike_slots.getSize()){
            //parking block for motorbikes is full
            ParkingLotPrinter::printVehicle(std::cout, new_car.getVehicleType(),
                    new_car.getLicensePlate() , new_car.getEntranceTime());
            ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
            return NO_EMPTY_SPOT;
        }

        //if we are here we can insert the motor to the block
        index = motorbike_slots.insert(new_car);
        ParkingLotPrinter::printVehicle(std::cout, new_car.getVehicleType(),
                new_car.getLicensePlate() , new_car.getEntranceTime());
        ParkingLotPrinter::printEntrySuccess(std::cout,
                ParkingSpot(new_car.getVehicleType(),index));
        return SUCCESS;
    }

    ParkingResult ParkingLot::insertCar(const Car new_car) {

        unsigned int index;

        if(CheckIfAlreadyParked(new_car, handicap_slots, HANDICAPPED) ||
           CheckIfAlreadyParked(new_car, car_slots, CAR)||
           CheckIfAlreadyParked(new_car, motorbike_slots, MOTORBIKE)){
            //if already parked everything is printed in the check function
            return VEHICLE_ALREADY_PARKED;

        }else if(car_slots.getCount() == car_slots.getSize()){
            ParkingLotPrinter::printVehicle(std::cout, new_car.getVehicleType(),
                    new_car.getLicensePlate() , new_car.getEntranceTime());
            ParkingLotPrinter::printEntryFailureNoSpot(std::cout);
            return NO_EMPTY_SPOT;
        }

        //if we are here we can insert the car to the block
        index = car_slots.insert(new_car);
        ParkingLotPrinter::printVehicle(std::cout, new_car.getVehicleType(),
                new_car.getLicensePlate() , new_car.getEntranceTime());
        ParkingLotPrinter::printEntrySuccess(std::cout,
                ParkingSpot(new_car.getVehicleType(),index));
        return SUCCESS;
    }

    ParkingResult ParkingLot::enterParking(
            ParkingLotUtils::VehicleType vehicleType,
            ParkingLotUtils::LicensePlate licensePlate,
            ParkingLotUtils::Time entranceTime) {
        Car new_car(licensePlate,entranceTime,vehicleType);
        ParkingResult res;
        switch (vehicleType){
            case MOTORBIKE :
                res = insertMotorbike(new_car);
                break;
            case HANDICAPPED:
                res = insertHandicap(new_car);
                break;
            case CAR:
                res = insertCar(new_car);
                break;
        }

        return res;
    }

    static void printRemoveMessages(const Car *to_remove,
            const unsigned int& index, const Time exit_time,
            VehicleType block_type){
        ParkingSpot parking_spot(block_type, index);
        ParkingLotPrinter::printVehicle(std::cout,
                to_remove->getVehicleType(), to_remove->getLicensePlate(),
                to_remove->getEntranceTime());

        ParkingLotPrinter::printExitSuccess(std::cout,parking_spot,exit_time,
                to_remove->calculatePrice(exit_time));
    }

    ParkingResult ParkingLot::exitParking(
            ParkingLotUtils::LicensePlate licensePlate,
            ParkingLotUtils::Time exitTime) {
        Car parking_car(licensePlate);
        //used to be sent to the UA's to find car with the same license plate
        unsigned int index;
        const Car *to_remove = NULL; //pointer to the car to be deleted;
        if(motorbike_slots.getIndex(parking_car,index)){
            to_remove = motorbike_slots[parking_car];
            printRemoveMessages(to_remove,index,exitTime,MOTORBIKE);
            motorbike_slots.remove(*to_remove);
            return SUCCESS;
        }else if(car_slots.getIndex(parking_car,index)) {
            to_remove = car_slots[parking_car];
            printRemoveMessages(to_remove, index, exitTime,CAR);
            car_slots.remove(*to_remove);
            return SUCCESS;
        }else if(handicap_slots.getIndex(parking_car,index)){
            to_remove = handicap_slots[parking_car];
            printRemoveMessages(to_remove,index,exitTime,HANDICAPPED);
            handicap_slots.remove(*to_remove);
            return SUCCESS;
        }
        //if we are here didn't find vehicle with given license plate in any of
        //the 3 blocks
        ParkingLotPrinter::printExitFailure(std::cout,licensePlate);
        return VEHICLE_NOT_FOUND;
    }

    ParkingResult ParkingLot::getParkingSpot(
            ParkingLotUtils::LicensePlate licensePlate,
            ParkingLotUtils::ParkingSpot &parkingSpot) const {
        Car parking_car(licensePlate);
        //help "Car" variable only with license plate to search in UA
        unsigned int index;
        if(motorbike_slots.getIndex(parking_car, index)) {
            parkingSpot = ParkingSpot(MOTORBIKE, index);
            return SUCCESS;
        }else if(car_slots.getIndex(parking_car, index)){
            //can contain handicapped or car
            parkingSpot = ParkingSpot(CAR, index);
            return SUCCESS;
        }else if(car_slots.getIndex(parking_car, index)){
            parkingSpot = ParkingSpot(HANDICAPPED, index);
            return SUCCESS;
        }
        //if we are here vehicle is not in any of the blocks
        return VEHICLE_NOT_FOUND;
    }

    static unsigned int giveFinesToBlock(UniqueArray<Car,CarCompare>& block,
                                               const Time inspection_time){
        Time diffrence;
        Car *current_car = NULL;
        unsigned int count = 0;
        for(unsigned int i = 0; i<block.getSize(); i++) {
            current_car = block.getByIndex(i);
            if (current_car == NULL) {
                continue;//empty slot
            }
            diffrence = inspection_time - current_car->getEntranceTime();
            if (diffrence.toHours() - 1  >= HOURS_TO_FINE
                && !(current_car->isFined()) ) {
                //.toHours rounds to top so we reduce one hour
                current_car->giveFine();
                count++;
            }
        }
        return count;
    }

    void ParkingLot::inspectParkingLot(ParkingLotUtils::Time inspectionTime) {

        unsigned int fines_count = 0;
        fines_count += giveFinesToBlock(motorbike_slots, inspectionTime);
        fines_count += giveFinesToBlock(car_slots, inspectionTime);
        fines_count += giveFinesToBlock(handicap_slots, inspectionTime);

        ParkingLotPrinter::printInspectionResult(std::cout, inspectionTime,
                                                           fines_count);
    }

    static void getAllParkingSpots(UniqueArray<Car,CarCompare> block,
            std::vector<ParkingSpot>& spots_vector, VehicleType block_type){
        //this function gets vector of Parking Spots and parking block and
        //inserts all taken parkings into the vector
        const Car *car;
        for(unsigned int i = 0; i<block.getSize(); i++){
            car = block.getByIndex(i);
            if(car == NULL){
                continue;
            }
            ParkingSpot spot(block_type,i);
            spots_vector.push_back(spot);
        }
    }

    ostream& operator<<(ostream &os,
                    const MtmParkingLot::ParkingLot &parkingLot) {
        ParkingLotPrinter::printParkingLotTitle(os);
        std::vector<ParkingSpot> spots_vector;
        getAllParkingSpots(parkingLot.motorbike_slots,spots_vector,MOTORBIKE);
        getAllParkingSpots(parkingLot.handicap_slots, spots_vector,HANDICAPPED);
        getAllParkingSpots(parkingLot.car_slots, spots_vector, CAR);
        std::sort(spots_vector.begin(),spots_vector.end());
        const Car *current_car = NULL;
        for(unsigned int j =0; j<spots_vector.size(); j++){
            if(spots_vector[j].getParkingBlock() == MOTORBIKE){
                current_car = parkingLot.motorbike_slots.getByIndex
                                        (spots_vector[j].getParkingNumber());
                ParkingLotPrinter::printVehicle(os, MOTORBIKE,current_car->
                            getLicensePlate(),current_car->getEntranceTime());
            }else if(spots_vector[j].getParkingBlock() == CAR){
                current_car = parkingLot.car_slots.getByIndex
                        (spots_vector[j].getParkingNumber());
                ParkingLotPrinter::printVehicle(os,current_car->getVehicleType()
                            , current_car->getLicensePlate(),
                            current_car->getEntranceTime());
            }else if(spots_vector[j].getParkingBlock() == HANDICAPPED){
                current_car = parkingLot.handicap_slots.getByIndex
                        (spots_vector[j].getParkingNumber());
                ParkingLotPrinter::printVehicle(os, HANDICAPPED,current_car->
                        getLicensePlate(),current_car->getEntranceTime());
            }//car is printed from the correct UA, now just the ParkingSpot :
            ParkingLotPrinter::printParkingSpot(os,spots_vector[j]);
        }
        return os;
    }

}

