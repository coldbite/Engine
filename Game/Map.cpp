#include "Map.h"
#include <iostream>
#include <thread>
#include <chrono>

Map::Map() {
    // Init
}

Map::~Map() {
    // Clean-Up)
}

void Map::OnLoaded(OnLoadedCallback callback) {
    onLoadedCallback = callback;
}

void Map::OnLoading(OnLoadingCallback callback) {
    onLoadingCallback = callback;
}

void Map::Load(const std::string& name) {
    mapName = name;
    std::cout << "[Map] Starting to load: " << name << std::endl;

    // Load asynchronously to avoid blocking the main thread
    std::thread loadingThread([this, name]() {
        // Simulate loading process with callbacks
        const int totalSteps = 5;
        const std::vector<std::string> steps = {
            "Loading terrain data...",
            "Loading textures...",
            "Loading objects...",
            "Loading entities...",
            "Finalizing map..."
        };

        for(int i = 0; i < totalSteps; ++i) {
            if(onLoadingCallback) {
                onLoadingCallback(steps[i], i + 1, totalSteps, ((float)(i + 1) / totalSteps * 100.0f));
            }

            // Simulate loading time
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        std::cout << "[Map] Finished loading: " << name << std::endl;

        // Call loaded callback
        if(onLoadedCallback) {
            onLoadedCallback();
        }
    });

    // Detach the thread so it can run independently
    loadingThread.detach();
}
