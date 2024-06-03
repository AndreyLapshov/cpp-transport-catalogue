#pragma once

#include "router.h"
#include "transport_catalogue.h"

#include <memory>

namespace transport {

struct RoutingSettings {
	int bus_wait_time = 0;
	double bus_velocity = 0.0;
};

class Router {
public:
	explicit Router(const RoutingSettings& settings, const Catalogue& catalogue)
		: settings_(settings), catalogue_(catalogue)
	{
		BuildGraph();
	}

	const std::optional<graph::Router<double>::RouteInfo> FindRoute(const std::string_view stop_from, const std::string_view stop_to) const;
	const graph::DirectedWeightedGraph<double>& GetGraph() const;

private:
	RoutingSettings settings_;
	const Catalogue& catalogue_;
	graph::DirectedWeightedGraph<double> graph_;
	graph::DirectedWeightedGraph<double> stops_graph_;
	std::map<std::string, graph::VertexId> stop_ids_;
	std::unique_ptr<graph::Router<double>> router_;

	void BuildGraph();
	void FillGraphStops();
	void FillGraphBuses();
};

}