#pragma once

#include "transport_catalogue.h"

#include <iostream>

void ProcessRequests(std::ostream& out, TransportCatalogue& catalogue);

void PrintRoute(std::string& line, TransportCatalogue& catalogue, std::ostream& out);
void PrinttStop(std::string& line, TransportCatalogue& catalogue, std::ostream& out);