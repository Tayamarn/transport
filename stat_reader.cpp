#include "stat_reader.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string>

#include "input_reader.h"
#include "transport_catalogue.h"

// #include "log_duration.h"

namespace stat_reader {
    using namespace detail;
    using namespace transport_catalogue;

    void PrintBus(std::string& request, TransportCatalogue& tc) {
        // 4 == "Bus ".size()
        std::string bus_name = request.substr(4);
        trim(bus_name);
        auto busname_to_bus = tc.GetBusnames();
        if (busname_to_bus.find(bus_name) == busname_to_bus.end()) {
            std::cout << "Bus " << bus_name << ": not found" << std::endl;
            return;
        }
        Bus* bus = busname_to_bus.at(bus_name);
        double true_dist = bus->GetTrueDistance();
        double curvature = true_dist / bus->GetGeoDistance();
        std::cout << std::setprecision(6)
                  << "Bus " << bus_name <<": "
                  << bus->stops.size() << " stops on route, "
                  << bus->unique_stops.size() << " unique stops, "
                  << true_dist <<" route length, "
                  << curvature << " curvature" << std::endl;
    }

    void PrintStop(std::string& request, transport_catalogue::TransportCatalogue& tc) {
        // 5 == "Stop ".size()
        std::string stop_name = request.substr(5);
        trim(stop_name);
        auto stopname_to_stop = tc.GetStopnames();
        if (stopname_to_stop.find(stop_name) == stopname_to_stop.end()) {
            std::cout << "Stop " << stop_name << ": not found" << std::endl;
            return;
        }
        Stop* stop = stopname_to_stop.at(stop_name);
        std::unordered_set<Bus*> buses = tc.GetBusesByStop(stop);
        if (buses.size() == 0) {
            std::cout << "Stop " << stop_name << ": no buses" << std::endl;
            return;
        }
        std::vector<std::string> bus_names;
        for (Bus* b : buses) {
            bus_names.push_back(b->name);
        }
        std::sort(bus_names.begin(), bus_names.end());
        std::cout << "Stop " << stop_name <<": buses";
        for (std::string& bn : bus_names) {
            std::cout << " " << bn;
        }
        std::cout << std::endl;
    }

    void ReadStat(transport_catalogue::TransportCatalogue& tc) {
        int num_requests = 0;
        std::cin >> num_requests;
        std::vector<std::string> requests;
        std::string request;
        for (;num_requests >= 0;--num_requests) {
            std::getline(std::cin, request);
            requests.push_back(request);
        }

        for (auto req : requests) {
            if (req.rfind("Bus", 0) == 0) {
                // LOG_DURATION("Bus");
                PrintBus(req, tc);
            } else if (req.rfind("Stop", 0) == 0) {
                PrintStop(req, tc);
            }
        }
    }
}
