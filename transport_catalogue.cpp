#include "transport_catalogue.h"

namespace transport {

void Catalogue::AddStop(std::string_view stop_name, const geo::Coordinates coordinates) {
    all_stops_.push_back({ std::string(stop_name), coordinates });
    stopname_to_stop_[all_stops_.back().name] = &all_stops_.back();
}

void Catalogue::AddRoute(std::string_view bus_number, const std::vector<const Stop*> stops, bool is_circle) {
    all_buses_.push_back({ std::string(bus_number), stops, is_circle });
    busname_to_bus_[all_buses_.back().number] = &all_buses_.back();
    for (const auto& route_stop : stops) {
        for (auto& stop_ : all_stops_) {
            if (stop_.name == route_stop->name) {
                const Stop* current_stop = FindStop(stop_.name);
                const Bus* current_bus = FindRoute(bus_number);
                buses_by_stop_[current_stop].emplace(current_bus);
            }
        }
    }
}

const Bus* Catalogue::FindRoute(std::string_view bus_number) const {
    return busname_to_bus_.count(bus_number) ? busname_to_bus_.at(bus_number) : nullptr;
}

const Stop* Catalogue::FindStop(std::string_view stop_name) const {
    return stopname_to_stop_.count(stop_name) ? stopname_to_stop_.at(stop_name) : nullptr;
}

const std::unordered_set<const Bus*> transport::Catalogue::GetBusesOnStop(const std::string_view stop_name) const {
    static const std::unordered_set<const Bus*> dummy;
    const auto& current_stop = FindStop(stop_name);
    auto iter = buses_by_stop_.find(current_stop);
    return iter == buses_by_stop_.end() ? dummy : iter->second;
}

void Catalogue::SetDistance(const Stop* from, const Stop* to, const int distance) {
    stop_distances_[{from, to}] = distance;
}

int Catalogue::GetDistance(const Stop* from, const Stop* to) const {
    if (stop_distances_.count({ from, to })) return stop_distances_.at({ from, to });
    else if (stop_distances_.count({ to, from })) return stop_distances_.at({ to, from });
    else return 0;
}

size_t Catalogue::GetUniqueStops(const std::string_view route_number) const {
    return CountUniqueStops(route_number);
}

const std::map<std::string_view, const Bus*> Catalogue::GetSortedAllBuses() const {
    std::map<std::string_view, const Bus*> result;
    for (const auto& bus : busname_to_bus_) {
        result.emplace(bus);
    }
    return result;
}

size_t transport::Catalogue::CountUniqueStops(const std::string_view route_number) const {
    std::unordered_set<const Stop*> unique_stops;
    for (const auto& stop : busname_to_bus_.at(route_number)->stops) {
        unique_stops.insert(stop);
    }
    return unique_stops.size();
}

} // namespace transport