#ifndef EX3_CAR_H
#define EX3_CAR_H

#include "ParkingLotTypes.h"
#include "Time.h"

namespace MtmParkingLot{

    using namespace ParkingLotUtils;

    class Car {
        LicensePlate license_plate;
        Time entrance_time;
        VehicleType vehicle_type;
        bool fine;

    public:
        /**
         * @brief Construct a new car object
         *
         * @param license_plate car's license_plate
         * @param entrance car's entrance time
         * @param vehicle_type type of the vehicle (MOTORBIKE,CAR,HANDICAPPED)
         */
        Car(const LicensePlate license_plate,const Time entrance = Time(),
            const VehicleType vehicle_type = MOTORBIKE);


        /**
         * @brief Marks (this) car to be fined
         */
        void giveFine();

        /**
         * @brief Returns car entrance time
         * @return this.entrance_time
         */
        Time getEntranceTime() const;

        /**
        * @brief Calculate the price a car has to pay if it leaves at given time
        *
        * @param exit_time time of exiting
        *@return price to pay
        */
        unsigned int calculatePrice(const Time& exit_time) const;

        /**
        * @brief returns car's leicnse plate
        *@return license plate
        */
        LicensePlate getLicensePlate() const;

        /**
        * @brief gets car vehicle type
        *@return vehicle type
        */
        VehicleType getVehicleType() const;

        /**
        * @brief checks if already fined
        *@return true if fined
        */
        bool isFined() const;


    };

    class CarCompare { //function object for comparing cars
    public:
        bool operator()(const Car car1, const Car car2) const{
            return (car1.getLicensePlate() == car2.getLicensePlate());
        }
    };

}
#endif //EX3_CAR_H
