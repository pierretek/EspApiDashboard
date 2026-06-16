//Stores the current state of the api configuration
#pragma once

#include <array>

#define MAX_APIS 5

struct ApiConfig {
	char apiLink[200]{};
	char jsonKey[50]{};
	char title[25]{};
	char suffix[20]{};
	uint32_t displayColor{};
	unsigned long refreshCooldown{};
};

inline std::array<ApiConfig, MAX_APIS> state{};


