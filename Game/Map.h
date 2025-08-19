#pragma once
#include "../Engine/Core/World.h"
#include "../Engine/Core/Settings/Config.h"
#include <functional>
#include <string>

class Map : public Engine::World {
public:
    using OnLoadedCallback = std::function<void()>;
    using OnLoadingCallback = std::function<void(const std::string& message, int actual, int total, float percentage)>;
    using OnMapStartCallback = std::function<void(const std::string& name, const ::Engine::Config& mapConfig)>;

    Map();
    ~Map();

    void OnLoaded(OnLoadedCallback callback);
    void OnLoading(OnLoadingCallback callback);
    void OnMapStart(OnMapStartCallback callback);
    void Load(const std::string& name, OnMapStartCallback callback);

    // Getter for map configuration
    const ::Engine::Config& GetConfig() const { return mapConfig; }
    bool ConfigLoaded() const { return mapConfigLoaded; }

private:
    OnLoadedCallback onLoadedCallback;
    OnLoadingCallback onLoadingCallback;
    OnMapStartCallback onMapStartCallback;
    ::Engine::Config mapConfig;
    bool mapConfigLoaded = false;
};
