#pragma once

#include "Enum/ScalingMode.h"
#include "Enum/UpscalingTechnique.h"
#include <string>

namespace Engine {
    
    struct ViewportInfo {
        int x, y;           // Viewport position
        int width, height;  // Viewport dimensions
        float scaleX, scaleY; // Scaling factors
        float letterboxOffsetX, letterboxOffsetY; // Offset for centering
    };
    
    struct ResolutionSettings {
        int width = 1280;
        int height = 720;
        float scale = 1.0f;
        ScalingMode mode = ScalingMode::FIT;
        UpscalingTechnique technique = UpscalingTechnique::LINEAR;
    };
    
    class ViewportManager {
    public:
        ViewportManager();
        ~ViewportManager() = default;
        
        // Configuration
        void SetReferenceResolution(int width, int height);
        void SetWindowDimensions(int width, int height);
        void SetScalingMode(ScalingMode mode);
        void SetUpscalingTechnique(UpscalingTechnique technique);
        void SetScale(float scale);
        
        // Getters
        ViewportInfo CalculateViewport() const;
        ScalingMode GetScalingMode() const { return scalingMode; }
        UpscalingTechnique GetUpscalingTechnique() const { return upscalingTechnique; }
        
        // Reference resolution
        int GetReferenceWidth() const { return referenceWidth; }
        int GetReferenceHeight() const { return referenceHeight; }
        
        // Window dimensions
        int GetWindowWidth() const { return windowWidth; }
        int GetWindowHeight() const { return windowHeight; }
        
        // Scale factors
        float GetScaleX() const;
        float GetScaleY() const;
        float GetUniformScale() const;
        
        // Coordinate transformations
        float TransformX(float referenceX) const;
        float TransformY(float referenceY) const;
        float TransformWidth(float referenceWidth) const;
        float TransformHeight(float referenceHeight) const;
        
        // Inverse transformations (screen to reference coordinates)
        float InverseTransformX(float screenX) const;
        float InverseTransformY(float screenY) const;
        
        // Utility methods - now using generic enum system
        // Use Engine::ToString(mode) and Engine::FromString<ScalingMode>(str) instead
        
        // Load from config (call manually when needed)
        void LoadFromConfig();
        
    private:
        int referenceWidth = 1280;
        int referenceHeight = 720;
        int windowWidth = 1280;
        int windowHeight = 720;
        float globalScale = 1.0f;
        
        ScalingMode scalingMode = ScalingMode::FIT;
        UpscalingTechnique upscalingTechnique = UpscalingTechnique::LINEAR;
        
        // Helper methods
        ViewportInfo CalculateStretchViewport() const;
        ViewportInfo CalculateLetterboxViewport() const;
        ViewportInfo CalculatePillarboxViewport() const;
        ViewportInfo CalculateFitViewport() const;
        ViewportInfo CalculateCropViewport() const;
        ViewportInfo CalculateIntegerViewport() const;
        
        float GetAspectRatio() const;
        float GetWindowAspectRatio() const;
    };
}