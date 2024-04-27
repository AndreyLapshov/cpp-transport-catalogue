/*
 * В этом файле вы можете разместить классы/структуры, которые являются частью предметной области (domain)
 * вашего приложения и не зависят от транспортного справочника. Например Автобусные маршруты и Остановки.
 *
 * Их можно было бы разместить и в transport_catalogue.h, однако вынесение их в отдельный
 * заголовочный файл может оказаться полезным, когда дело дойдёт до визуализации карты маршрутов:
 * визуализатор карты (map_renderer) можно будет сделать независящим от транспортного справочника.
 *
 * Если структура вашего приложения не позволяет так сделать, просто оставьте этот файл пустым.
 *
 */

#pragma once

#include "geo.h"

#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

namespace transport {

struct Stop {
    std::string name;
    geo::Coordinates coordinates;
};

struct Bus {
    std::string number;
    std::vector<const Stop*> stops;
    bool is_roundtrip;
};

struct RouteInfo {
    size_t stops_count;
    size_t unique_stops_count;
    double route_length;
    double curvature;
};

struct FillStopData {
    std::string_view stop_name;
    geo::Coordinates coordinates{};
    std::map<std::string_view, int> stop_distances;
};

struct FillRouteData {
    std::string_view bus_number;
    std::vector<const transport::Stop*> stops;
    bool is_roundtrip;
};

} // namespace transport