#pragma once

#include "geo.h"

#include <iostream>
#include <deque>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <optional>
#include <unordered_set>
#include <set>

struct Bus {
    std::string number;
    std::vector<std::string> stops;
    bool circular_route;
};

struct Stop {
    std::string name;
    Coordinates coordinates;
};

struct RouteInfo {
    size_t stops_count;
    size_t unique_stops_count;
    double route_length;
    double curvature;
};

struct StopDistancesHasher {
    size_t operator()(const std::pair<const Stop*, const Stop*>& points) const {
        size_t hash_first = std::hash<const void*>{}(points.first);
        size_t hash_second = std::hash<const void*>{}(points.second);
        return hash_first + hash_second * 37;
    }
};

class TransportCatalogue {
public:
    void AddRoute(const Bus& bus);
    void AddStop(const Stop& stop);
    const Bus* FindRoute(const std::string_view route_number) const;
    Stop* FindStop(const std::string_view stop_name) const;
    const RouteInfo RouteInformation(const std::string_view route_number) const;
    const std::unordered_set<const Bus*> GetBusesOnStop(const std::string_view stop_name) const;
    void SetDistance(Stop* from, Stop* to, int distance);
    int GetDistance(const Stop* from, const Stop* to) const;
    
private:
    std::deque<Bus> all_buses_;
    std::deque<Stop> all_stops_;
    std::unordered_map<std::string_view, const Bus*> busname_to_bus_;
    std::unordered_map<std::string_view, Stop*> stopname_to_stop_;
    std::unordered_map<const Stop*, std::unordered_set<const Bus*>> buses_by_stop_;
    std::unordered_map<std::pair<const Stop*, const Stop*>, int, StopDistancesHasher> stop_distances_;
    
    size_t CountUniqueStops(const std::string_view route_number) const;
};
