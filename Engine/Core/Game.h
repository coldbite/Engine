#pragma once

#include "Engine.h"
#include "ViewManager.h"
#include "NativeWindow.h"
#include "Settings/Options.h"
#include <memory>
#include <atomic>

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }

    class Game : public Engine {
    public:
        Game();
        virtual ~Game();

        bool Initialize();
        void Run();
        void Shutdown();

        virtual void OnInit() {}
        virtual void OnShutdown() {}

        template<typename EnumType>
        void AddOptions() {
            Settings::OptionsManager::GetInstance().AddOptions<EnumType>();
        }

        template<typename EnumType, typename T>
        void SetOption(EnumType option, const T& value) {
            Settings::OptionsManager::GetInstance().SetOption(option, value);
        }

        template<typename EnumType>
        bool HasOption(EnumType option) const {
            return Settings::OptionsManager::GetInstance().HasOption(option);
        }

        template<typename EnumType, typename T>
        T GetOption(EnumType option, const T& defaultValue = T{}) const {
            return Settings::OptionsManager::GetInstance().GetOption<EnumType, T>(option, defaultValue);
        }

        template<typename EnumType>
        void PrintOptions() const {
            Settings::OptionsManager::GetInstance().PrintOptions<EnumType>();
        }

        // View Management
        ViewManager& GetViewManager() { return *viewManager; }

        // Window Management
        std::shared_ptr<NativeWindow> GetMainWindow() { return mainWindow; }

        // Threading
        void RequestStop() { shouldStop = true; }


    private:
        void SetupEventHandlers();

        std::unique_ptr<ViewManager> viewManager;
        std::shared_ptr<NativeWindow> mainWindow;
        std::shared_ptr<Graphics::IRenderingAPI> renderingAPI;
        bool isInitialized;
        std::atomic<bool> shouldStop{false};
    };
}
