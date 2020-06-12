#include "catch.hpp"
#include "ParkingLot.h"
#include "ParkingLotPrinter.h"
#include "ParkingLotTypes.h"
#include "ParkingSpot.h"
#include "Time.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

using namespace MtmParkingLot;
using namespace ParkingLotUtils;
using std::ostringstream;
using std::cout;
using std::streambuf;
using std::vector;
using std::map;
using std::pair;
using std::sort;

namespace MtmParkingLotTest {

    typedef pair<VehicleType, LicensePlate> VehicleInfo;
    typedef pair<VehicleInfo, ParkingSpot> FullVehicleInfo;

}

using namespace MtmParkingLotTest;

#define GET_COUT(strCout, expr) {               \
    streambuf* oldCoutStreamBuf = cout.rdbuf(); \
    cout.rdbuf(strCout.rdbuf());                \
    (expr);                                     \
    cout.rdbuf(oldCoutStreamBuf);               \
}

static bool operator== (const ParkingSpot& ps1, const ParkingSpot& ps2) {
    return (!(ps1 < ps2) && !(ps2 < ps1));
}

static unsigned int calculatePayment(Time parkingTime, VehicleType vehicleType, bool isFined) {
    static const map<VehicleType, unsigned int> pricePer1stHour = { {VehicleType::MOTORBIKE, 10}, {VehicleType::HANDICAPPED, 15}, {VehicleType::CAR, 20} };
    static const map<VehicleType, unsigned int> pricePerHour    = { {VehicleType::MOTORBIKE, 5},  {VehicleType::HANDICAPPED, 0},  {VehicleType::CAR, 10} };
    Time::Hour numHours = parkingTime.toHours();
    if (numHours > 6) {
        numHours = 6;
    }
    return (pricePer1stHour.at(vehicleType) + (numHours-1)*pricePerHour.at(vehicleType) + (isFined ? 250 : 0));
}

static vector<FullVehicleInfo> getSortedParkingVec(const map<VehicleInfo, ParkingSpot>& parkingSpots) {
    vector<FullVehicleInfo> parkingSpotsVec(parkingSpots.begin(), parkingSpots.end());
    struct CompareByParkingSpot {
        bool operator()(const FullVehicleInfo& fvi1, const FullVehicleInfo& fvi2) const {
            return (fvi1.second < fvi2.second);
        }
    };
    sort(parkingSpotsVec.begin(), parkingSpotsVec.end(), CompareByParkingSpot());
    return parkingSpotsVec;
}

static string printParkingLotReq(const vector<FullVehicleInfo>& parkingSpotsVec, Time time) {
    ostringstream printOutputReq;
    ParkingLotPrinter::printParkingLotTitle(printOutputReq);
    for (FullVehicleInfo vfullinfo : parkingSpotsVec) {
        ParkingLotPrinter::printVehicle(printOutputReq, vfullinfo.first.first, vfullinfo.first.second, time);
        ParkingLotPrinter::printParkingSpot(printOutputReq, vfullinfo.second);
    }
    return printOutputReq.str();
}

SCENARIO("ParkingLot enterParking, getParkingSpot", "[ParkingLot0]") {

    GIVEN("An empty ParkingLot") {
        unsigned int parkingSizes[VehicleType::LAST-VehicleType::FIRST+1] = {1, 1, 2};
        ParkingLot pl(parkingSizes);
        Time time;

        WHEN("Motorbike attempts to enter") {
            VehicleType vehicleType = VehicleType::MOTORBIKE;
            LicensePlate licensePlate = "ABCD";

            ostringstream enterOutput;
            ParkingResult res1;
            GET_COUT(enterOutput, (res1 = pl.enterParking(vehicleType, licensePlate, time)));

            ParkingSpot parkingSpot;
            ParkingResult res2 = pl.getParkingSpot(licensePlate, parkingSpot);
            ostringstream enterOutputReq;
            ParkingLotPrinter::printVehicle(enterOutputReq, vehicleType, licensePlate, time);
            ParkingLotPrinter::printEntrySuccess(enterOutputReq, parkingSpot);

            THEN("enterParking is successful, getParkingSpot is successful and output as expected") {
                REQUIRE(res1 == ParkingResult::SUCCESS);
                REQUIRE(res2 == ParkingResult::SUCCESS);
                REQUIRE(enterOutput.str() == enterOutputReq.str());
            }
        }
    }

    GIVEN("A full Car ParkingLot") {
        unsigned int parkingSizes[VehicleType::LAST-VehicleType::FIRST+1] = {1, 1, 1};
        ParkingLot pl(parkingSizes);
        Time time;

        VehicleType vehicleType = VehicleType::CAR;
        LicensePlate licensePlate = "1234";
        pl.enterParking(vehicleType, licensePlate, time);
        ParkingSpot oldParkingSpot;
        pl.getParkingSpot(licensePlate, oldParkingSpot);

        WHEN("Existing vehicle attempts to enter") {
            ostringstream enterOutput;
            ParkingResult res;
            GET_COUT(enterOutput, (res = pl.enterParking(VehicleType::MOTORBIKE, licensePlate, time)));

            ParkingSpot parkingSpot;
            pl.getParkingSpot(licensePlate, parkingSpot);
            ostringstream enterOutputReq;
            ParkingLotPrinter::printVehicle(enterOutputReq, VehicleType::CAR, licensePlate, time);
            ParkingLotPrinter::printEntryFailureAlreadyParked(enterOutputReq, parkingSpot);

            THEN("enterParking fails with ALREADY_PARKED, output as expected and ParkingSpot as expected") {
                REQUIRE(res == ParkingResult::VEHICLE_ALREADY_PARKED);
                REQUIRE(enterOutput.str() == enterOutputReq.str());
                REQUIRE(oldParkingSpot == parkingSpot);
            }
        }
        WHEN("Different vehicle attempts to enter") {
            LicensePlate licensePlate = "ABCD";
            ostringstream enterOutput;
            ParkingResult res;
            GET_COUT(enterOutput, (res = pl.enterParking(VehicleType::CAR, licensePlate, time)));

            ostringstream enterOutputReq;
            ParkingLotPrinter::printVehicle(enterOutputReq, VehicleType::CAR, licensePlate, time);
            ParkingLotPrinter::printEntryFailureNoSpot(enterOutputReq);

            THEN("enterParking fails with NO_EMPTY_SPOT and output as expected") {
                REQUIRE(res == ParkingResult::NO_EMPTY_SPOT);
                REQUIRE(enterOutput.str() == enterOutputReq.str());
            }
        }
    }

    GIVEN("A full Handicapped ParkingLot with an available Car spot") {
        unsigned int parkingSizes[VehicleType::LAST-VehicleType::FIRST+1] = {1, 1, 1};
        ParkingLot pl(parkingSizes);
        Time time;

        VehicleType vehicleType = VehicleType::HANDICAPPED;
        LicensePlate licensePlate = "1234";
        pl.enterParking(vehicleType, licensePlate, time);
        ParkingSpot oldParkingSpot;
        pl.getParkingSpot(licensePlate, oldParkingSpot);

        WHEN("A new Handicapped attempts to enter") {
            VehicleType vehicleType = VehicleType::HANDICAPPED;
            LicensePlate licensePlate = "ABCD";

            ostringstream enterOutput;
            ParkingResult res;
            GET_COUT(enterOutput, (res = pl.enterParking(vehicleType, licensePlate, time)));

            ParkingSpot parkingSpot;
            pl.getParkingSpot(licensePlate, parkingSpot);
            ostringstream enterOutputReq;
            ParkingLotPrinter::printVehicle(enterOutputReq, vehicleType, licensePlate, time);
            ParkingLotPrinter::printEntrySuccess(enterOutputReq, parkingSpot);

            THEN("enterParking is successful and output as expected") {
                REQUIRE(res == ParkingResult::SUCCESS);
                REQUIRE(enterOutput.str() == enterOutputReq.str());
            }
        }
    }
}

SCENARIO("ParkingLot exitParking, getParkingSpot", "[ParkingLot1]") {
    vector<VehicleType> vehicleTypes = { MOTORBIKE, HANDICAPPED, CAR };
    for (VehicleType vehicleType : vehicleTypes) {
        GIVEN("A ParkingLot with " + vehicleTypeToString(vehicleType)) {
            unsigned int parkingSizes[VehicleType::LAST-VehicleType::FIRST+1] = {1, 1, 1};
            ParkingLot pl(parkingSizes);
            Time entranceTime;
            LicensePlate licensePlate = "1234";
            pl.enterParking(vehicleType, licensePlate, entranceTime);
            vector<Time> exitTimes = { Time(0, 1, 0), Time(0, 2, 0), Time(0, 7, 0) };

            for (Time exitTime : exitTimes) {
                WHEN(vehicleTypeToString(vehicleType) + " attempts to exit after parking for " + std::to_string(exitTime.toHours()) + " hours") {
                    ostringstream exitOutput;
                    ParkingResult res1;
                    ParkingSpot parkingSpot;
                    pl.getParkingSpot(licensePlate, parkingSpot);
                    GET_COUT(exitOutput, (res1 = pl.exitParking(licensePlate, exitTime)));

                    ParkingSpot dummyParkingSpot;
                    ParkingResult res2 = pl.getParkingSpot(licensePlate, dummyParkingSpot);
                    ostringstream exitOutputReq;
                    ParkingLotPrinter::printVehicle(exitOutputReq, vehicleType, licensePlate, entranceTime);
                    ParkingLotPrinter::printExitSuccess(exitOutputReq, parkingSpot, exitTime, calculatePayment(exitTime, vehicleType, false));

                    THEN("exitParking is successful, getParkingSpot on vehicle fails and output as expected") {
                        REQUIRE(res1 == ParkingResult::SUCCESS);
                        REQUIRE(res2 == ParkingResult::VEHICLE_NOT_FOUND);
                        REQUIRE(exitOutput.str() == exitOutputReq.str());
                    }
                }
            }
        }
    }
}

SCENARIO("ParkingLot operator<<", "[ParkingLot2]") {
    GIVEN("A full ParkingLot of various vehicles") {
        unsigned int parkingSizes[VehicleType::LAST-VehicleType::FIRST+1] = {2, 1, 2};
        ParkingLot pl(parkingSizes);
        Time time;
        map<VehicleInfo, ParkingSpot> parkingSpots = { {{VehicleType::MOTORBIKE, "11"}, ParkingSpot()},
                                                       {{VehicleType::MOTORBIKE, "12"}, ParkingSpot()}, 
                                                       {{VehicleType::HANDICAPPED, "21"}, ParkingSpot()},
                                                       {{VehicleType::CAR, "31"}, ParkingSpot()},
                                                       {{VehicleType::CAR, "32"}, ParkingSpot()}
                                                     };
        for (map<VehicleInfo, ParkingSpot>::iterator iter = parkingSpots.begin(); iter != parkingSpots.end(); iter++) {
            VehicleInfo vi = iter->first;
            pl.enterParking(vi.first, vi.second, time);
            pl.getParkingSpot(vi.second, iter->second);
        }
        vector<FullVehicleInfo> parkingSpotsVec = getSortedParkingVec(parkingSpots);

        WHEN("Printing the ParkingLot") {
            ostringstream strCout;
            strCout << pl;
            THEN("Output as expected") {
                REQUIRE(strCout.str() == printParkingLotReq(parkingSpotsVec, time));
            }
        }

    }
}

SCENARIO("ParkingLot inspect", "[ParkingLot3]") {
    vector<VehicleType> vehicleTypes = { MOTORBIKE, HANDICAPPED, CAR };
    for (VehicleType vehicleType : vehicleTypes) {
        GIVEN("A ParkingLot with fined " + vehicleTypeToString(vehicleType)) {
            unsigned int parkingSizes[VehicleType::LAST-VehicleType::FIRST+1] = {1, 1, 1};
            ParkingLot pl(parkingSizes);
            Time entranceTime;
            LicensePlate licensePlate = "1234";
            pl.enterParking(vehicleType, licensePlate, entranceTime);
            ostringstream inspectOutput;
            Time inspectionTime(1, 0, 1);
            GET_COUT(inspectOutput, pl.inspectParkingLot(inspectionTime));

            ostringstream inspectOutputReq;
            ParkingLotPrinter::printInspectionResult(inspectOutputReq, inspectionTime, 1);

            REQUIRE(inspectOutput.str() == inspectOutputReq.str());

            WHEN(vehicleTypeToString(vehicleType) + " attempts to exit") {
                Time exitTime(1, 0, 2);
                ostringstream exitOutput;
                ParkingSpot parkingSpot;
                pl.getParkingSpot(licensePlate, parkingSpot);
                GET_COUT(exitOutput, pl.exitParking(licensePlate, exitTime));

                ostringstream exitOutputReq;
                ParkingLotPrinter::printVehicle(exitOutputReq, vehicleType, licensePlate, entranceTime);
                ParkingLotPrinter::printExitSuccess(exitOutputReq, parkingSpot, exitTime, calculatePayment(exitTime, vehicleType, true));

                THEN("Output as expected") {
                    REQUIRE(exitOutput.str() == exitOutputReq.str());
                }
            }

            WHEN("Another inspection occurs") {
                ostringstream inspect2Output;
                Time inspection2Time(1, 1, 0);
                GET_COUT(inspect2Output, pl.inspectParkingLot(inspection2Time));

                ostringstream inspect2OutputReq;
                ParkingLotPrinter::printInspectionResult(inspect2OutputReq, inspection2Time, 0);

                THEN("Output as expected") {
                    REQUIRE(inspect2Output.str() == inspect2OutputReq.str());
                }
            }
        }
    }
}