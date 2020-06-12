#ifndef MTMPARKINGLOT_PARKINGLOT_H
#define MTMPARKINGLOT_PARKINGLOT_H

#include "ParkingLotTypes.h"
#include "Time.h"
#include "ParkingSpot.h"
#include "Car.h"
#include "UniqueArray.h"

namespace MtmParkingLot {

    using namespace ParkingLotUtils;
    using std::ostream;

    class ParkingLot {
    private:
        UniqueArray<Car,CarCompare> motorbike_slots;
        UniqueArray<Car,CarCompare> handicap_slots;
        UniqueArray<Car,CarCompare> car_slots;

    public:
        ParkingLot(unsigned int parkingBlockSizes[]);
        ~ParkingLot() = default;
        ParkingResult enterParking(VehicleType vehicleType, LicensePlate licensePlate, Time entranceTime);
        ParkingResult exitParking(LicensePlate licensePlate, Time exitTime);
        ParkingResult getParkingSpot(LicensePlate licensePlate, ParkingSpot& parkingSpot) const;
        void inspectParkingLot(Time inspectionTime);
        friend ostream& operator<<(ostream& os, const ParkingLot& parkingLot);

        //help methods:
        //inserts the car in the argument to the this.block that mentioned in
        //the method name
        ParkingResult insertMotorbike(const Car new_car);
        ParkingResult insertHandicap(const Car new_car);
        ParkingResult insertCar(const Car new_car);
    };



}

#endif //MTMPARKINGLOT_PARKINGLOT_H
