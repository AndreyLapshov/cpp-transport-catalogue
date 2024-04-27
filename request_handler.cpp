#include "request_handler.h"

std::optional<transport::RouteInfo> RequestHandler::GetRouteInfo(const std::string_view bus_number) const {
    transport::RouteInfo bus_stat{};
    const transport::Bus* bus = catalogue_.FindRoute(bus_number);

    if (!bus) throw std::invalid_argument("bus not found");
    if (bus->is_roundtrip) bus_stat.stops_count = bus->stops.size();
    else bus_stat.stops_count = bus->stops.size() * 2 - 1;

    int route_length = 0;
    double geographic_length = 0.0;

    for (size_t i = 0; i < bus->stops.size() - 1; ++i) {
        auto from = catalogue_.FindStop(bus->stops[i]->name);
        auto to = catalogue_.FindStop(bus->stops[i + 1]->name);
        if (bus->is_roundtrip) {
            route_length += catalogue_.GetDistance(from, to);
            geographic_length += geo::ComputeDistance(from->coordinates,
                to->coordinates);
        }
        else {
            route_length += catalogue_.GetDistance(from, to) + catalogue_.GetDistance(to, from);
            geographic_length += geo::ComputeDistance(from->coordinates,
                to->coordinates) * 2;
        }
    }

    bus_stat.unique_stops_count = catalogue_.GetUniqueStops(bus_number);
    bus_stat.route_length = route_length;
    bus_stat.curvature = route_length / geographic_length;

    return bus_stat;
}

const std::unordered_set<const transport::Bus*> RequestHandler::GetBusesOnStop(const std::string_view stop_name) const {
    return catalogue_.GetBusesOnStop(stop_name);
}

bool RequestHandler::IsBusNumber(const std::string_view bus_number) const {
    return catalogue_.FindRoute(bus_number);
}

bool RequestHandler::IsStopName(const std::string_view stop_name) const {
    return catalogue_.FindStop(stop_name);
}

svg::Document RequestHandler::RenderMap() const {
    return renderer_.GetSVG(catalogue_.GetSortedAllBuses());
}