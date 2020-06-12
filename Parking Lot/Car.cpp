#include "Car.h"
#include "Time.h"

namespace MtmParkingLot{

    const unsigned int FINE_PRICE = 250;
    const unsigned int PRIVATE_INITIAL_PRICE = 20;
    const unsigned int PRIVATE_PRICE = 10;
    const unsigned int MOTORBIKE_INITIAL_PRICE = 10;
    const unsigned int MOTORBIKE_PRICE = 5;
    const unsigned int HANDICAP_PRICE = 15;
    const unsigned int MAX_PAY_HOURS = 6;

    using ParkingLotUtils::Time;

    Car::Car(const LicensePlate license_plate,const Time entrance,
        const VehicleType vehicleType): license_plate(license_plate),
        entrance_time(entrance),vehicle_type(vehicleType), fine(false) {}

    void Car::giveFine(){
        fine = true;
    }

    Time Car::getEntranceTime() const {
        return entrance_time;
    }

    unsigned int Car::calculatePrice(const Time& exit_time) const {
        Time difference = exit_time - entrance_time;
        Time::Hour parking_hours = difference.toHours();
        if(parking_hours == 0){ //if left at entrance - will not pay
            return 0;
        }
        unsigned int price = 0;
        if(fine){
            price += FINE_PRICE;
        }
        if(vehicle_type == MOTORBIKE){
            price += MOTORBIKE_INITIAL_PRICE;//first hour is always payed
            if(parking_hours >= MAX_PAY_HOURS){
                price += (MAX_PAY_HOURS - 1) * MOTORBIKE_PRICE;
            }else {
                price += (parking_hours - 1) * MOTORBIKE_PRICE;
            } //if only one hour inside price will remain the same (1-1=0)
        } else if(vehicle_type == CAR){
            price += PRIVATE_INITIAL_PRICE;
            if(parking_hours >= MAX_PAY_HOURS){
                price += (MAX_PAY_HOURS - 1) * PRIVATE_PRICE;
            }else {
                price += (parking_hours - 1) * PRIVATE_PRICE;
            }
        } else if(vehicle_type == HANDICAPPED){
            price += HANDICAP_PRICE;
        }
        return price;
    }

    LicensePlate Car::getLicensePlate() const{
        return license_plate;
    }

    VehicleType  Car::getVehicleType() const {
        return vehicle_type;
    }

    bool Car::isFined() const {
        return fine;
    }

}

