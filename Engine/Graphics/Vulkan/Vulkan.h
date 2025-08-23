#pragma once

#include "../IRenderingAPI.h"
#include <string>
#include <memory>
#include <vector>

#ifdef _WIN32
    #define VK_USE_PLATFORM_WIN32_KHR
#endif

// Vulkan headers - commented out for now since SDK may not be installed
// Uncomment when Vulkan SDK is available
// #include <vulkan/vulkan.h>

// Vulkan types and constants (minimal definitions for compilation)
#ifndef VK_NULL_HANDLE
    #define VK_NULL_HANDLE 0
#endif

#ifndef VK_SUCCESS  
    #define VK_SUCCESS 0
#endif

// Forward declarations of Vulkan types
typedef void* VkInstance;
typedef void* VkDevice;
typedef void* VkPhysicalDevice;
typedef void* VkSurfaceKHR;
typedef void* VkSwapchainKHR;
typedef void* VkCommandPool;
typedef void* VkRenderPass;
typedef uint32_t VkResult;

namespace Engine {
    class NativeWindow;

    namespace Graphics {
        namespace Vulkan {

            class Vulkan : public IRenderingAPI {
            public:
                virtual ~Vulkan();
                bool Init(std::shared_ptr<NativeWindow> window) override;

                // Implementations for the pure virtual functions
                bool Available() override;
                std::string GetVersion() override;
                void CreateDevice() override;
                void GetDevice() override;
                void CreateContext() override;
                void GetContext() override;

                void SetViewport(int width, int height) override;
                void SetViewport(int x, int y, int width, int height) override;
                int GetWidth() override;
                int GetHeight() override;

                // Rendering operations
                void Clear() override;
                void Clear(IColor* color = new RGBA(0, 0, 0, 100)) override;
                void SwapBuffers() override;

                // 2D rendering setup
                void Begin2D(int width, int height) override;
                void End2D() override;

                Texture LoadTexture(const std::string& path) override;

                // Texture drawing
                void DrawTexture(const Texture& texture, float x, float y, float width, float height) override;
                void DrawTextureBlurred(const Texture& texture, float x, float y, float width, float height, float blurRadius = 2.0f) override;

                // Overlay effects
                void DrawDiagonalLines(float x, float y, float width, float height, float lineSpacing = 10.0f, float lineWidth = 1.0f, IColor* color = new RGBA(255, 255, 255, 10)) override;
                void DrawRadialLines(float x, float y, float width, float height, float centerX1, float centerY1, float centerX2, float centerY2, int numLines = 20, float lineWidth = 1.0f, IColor* color = new RGBA(255, 255, 255, 10)) override;
                void DrawVerticalLines(float x, float y, float width, float height, float lineSpacing = 15.0f, float lineWidth = 1.0f, IColor* color = new RGBA(255, 255, 255, 8)) override;
                void DrawHorizontalLines(float x, float y, float width, float height, float lineSpacing = 15.0f, float lineWidth = 1.0f, IColor* color = new RGBA(255, 255, 255, 8)) override;
                void DrawFilmGrain(float x, float y, float width, float height, float intensity = 0.08f, int seed = 0) override;

                // Primitive drawing
                void DrawRect(float x, float y, float width, float height, IColor* color) override;
                void DrawRectWithShadow(float x, float y, float width, float height, IColor* color, float shadowRadius, IColor* shadowColor, float shadowOffsetX = 0.0f, float shadowOffsetY = 0.0f) override;
                void PaintText(const std::string& text, float x, float y, IColor* color) override;

                // Static methods for backwards compatibility
                static void Shutdown();
                static void MakeContextCurrent();
                static void SwapBuffersStatic();
                static void Begin2DStatic(int width, int height);
                static void End2DStatic();

            private:
                static std::shared_ptr<NativeWindow> currentWindow;
                static bool initialized;
                std::string version;

                // Vulkan specific members (would be implemented when adding Vulkan support)
                // VkInstance instance = VK_NULL_HANDLE;
                // VkDevice device = VK_NULL_HANDLE;
                // VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
                // VkSurfaceKHR surface = VK_NULL_HANDLE;
                // VkSwapchainKHR swapchain = VK_NULL_HANDLE;
                // VkCommandPool commandPool = VK_NULL_HANDLE;
                // std::vector<VkCommandBuffer> commandBuffers;
                // std::vector<VkImage> swapchainImages;
                // std::vector<VkImageView> swapchainImageViews;
                // VkRenderPass renderPass = VK_NULL_HANDLE;
                // std::vector<VkFramebuffer> framebuffers;
                
                int windowWidth = 0;
                int windowHeight = 0;
                int viewportX = 0;
                int viewportY = 0;
                int viewportWidth = 0;
                int viewportHeight = 0;
                
                // Private initialization methods
                bool CreateVulkanInstance();
                bool CreateSurface();
                bool SelectPhysicalDevice();
                bool CreateLogicalDevice();
                bool CreateSwapchain();
                bool CreateRenderPass();
                bool CreateCommandPool();
            };
        }
    }
}