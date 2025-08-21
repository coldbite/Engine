#include "ViewportManager.h"
#include "Settings/Config.h"
#include <algorithm>
#include <cmath>
#include <iostream>

namespace Engine {
    
    ViewportManager::ViewportManager() {
        // Default values set in header, no auto-loading
    }
    
    void ViewportManager::SetReferenceResolution(int width, int height) {
        referenceWidth = width;
        referenceHeight = height;
    }
    
    void ViewportManager::SetWindowDimensions(int width, int height) {
        windowWidth = width;
        windowHeight = height;
    }
    
    void ViewportManager::SetScalingMode(ScalingMode mode) {
        scalingMode = mode;
    }
    
    void ViewportManager::SetUpscalingTechnique(UpscalingTechnique technique) {
        upscalingTechnique = technique;
    }
    
    void ViewportManager::SetScale(float scale) {
        globalScale = scale;
    }
    
    ViewportInfo ViewportManager::CalculateViewport() const {
        switch(scalingMode) {
            case ScalingMode::STRETCH:
                return CalculateStretchViewport();
            case ScalingMode::LETTER:
                return CalculateLetterboxViewport();
            case ScalingMode::PILLAR:
                return CalculatePillarboxViewport();
            case ScalingMode::FIT:
                return CalculateFitViewport();
            case ScalingMode::CROP:
                return CalculateCropViewport();
            case ScalingMode::INTEGER:
                return CalculateIntegerViewport();
            default:
                return CalculateFitViewport();
        }
    }
    
    float ViewportManager::GetScaleX() const {
        return (float)windowWidth / (float)referenceWidth * globalScale;
    }
    
    float ViewportManager::GetScaleY() const {
        return (float)windowHeight / (float)referenceHeight * globalScale;
    }
    
    float ViewportManager::GetUniformScale() const {
        return std::min(GetScaleX(), GetScaleY());
    }
    
    float ViewportManager::TransformX(float referenceX) const {
        ViewportInfo viewport = CalculateViewport();
        return viewport.x + referenceX * viewport.scaleX;
    }
    
    float ViewportManager::TransformY(float referenceY) const {
        ViewportInfo viewport = CalculateViewport();
        return viewport.y + referenceY * viewport.scaleY;
    }
    
    float ViewportManager::TransformWidth(float refWidth) const {
        ViewportInfo viewport = CalculateViewport();
        return refWidth * viewport.scaleX;
    }
    
    float ViewportManager::TransformHeight(float refHeight) const {
        ViewportInfo viewport = CalculateViewport();
        return refHeight * viewport.scaleY;
    }
    
    float ViewportManager::InverseTransformX(float screenX) const {
        ViewportInfo viewport = CalculateViewport();
        return (screenX - viewport.x) / viewport.scaleX;
    }
    
    float ViewportManager::InverseTransformY(float screenY) const {
        ViewportInfo viewport = CalculateViewport();
        return (screenY - viewport.y) / viewport.scaleY;
    }
    
    ViewportInfo ViewportManager::CalculateStretchViewport() const {
        ViewportInfo viewport;
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = windowWidth;
        viewport.height = windowHeight;
        viewport.scaleX = GetScaleX();
        viewport.scaleY = GetScaleY();
        viewport.letterboxOffsetX = 0;
        viewport.letterboxOffsetY = 0;
        return viewport;
    }
    
    ViewportInfo ViewportManager::CalculateLetterboxViewport() const {
        ViewportInfo viewport;
        
        float referenceAspect = GetAspectRatio();
        
        // Force letterboxing (horizontal black bars)
        viewport.width = windowWidth;
        viewport.height = (int)(windowWidth / referenceAspect);
        viewport.x = 0;
        viewport.y = (windowHeight - viewport.height) / 2;
        
        viewport.scaleX = (float)viewport.width / (float)referenceWidth * globalScale;
        viewport.scaleY = (float)viewport.height / (float)referenceHeight * globalScale;
        
        viewport.letterboxOffsetX = 0;
        viewport.letterboxOffsetY = (float)viewport.y;
        
        return viewport;
    }
    
    ViewportInfo ViewportManager::CalculatePillarboxViewport() const {
        ViewportInfo viewport;
        
        float referenceAspect = GetAspectRatio();
        
        // Force pillarboxing (vertical black bars)
        viewport.height = windowHeight;
        viewport.width = (int)(windowHeight * referenceAspect);
        viewport.y = 0;
        viewport.x = (windowWidth - viewport.width) / 2;
        
        viewport.scaleX = (float)viewport.width / (float)referenceWidth * globalScale;
        viewport.scaleY = (float)viewport.height / (float)referenceHeight * globalScale;
        
        viewport.letterboxOffsetX = (float)viewport.x;
        viewport.letterboxOffsetY = 0;
        
        return viewport;
    }
    
    ViewportInfo ViewportManager::CalculateFitViewport() const {
        float windowAspect = GetWindowAspectRatio();
        float referenceAspect = GetAspectRatio();
        
        if (windowAspect > referenceAspect) {
            // Window is wider than reference -> use pillarboxing
            return CalculatePillarboxViewport();
        } else {
            // Window is taller than reference -> use letterboxing
            return CalculateLetterboxViewport();
        }
    }
    
    ViewportInfo ViewportManager::CalculateCropViewport() const {
        ViewportInfo viewport;
        
        float windowAspect = GetWindowAspectRatio();
        float referenceAspect = GetAspectRatio();
        
        if (windowAspect > referenceAspect) {
            // Window is wider -> scale by height (crops sides)
            viewport.height = windowHeight;
            viewport.width = (int)(windowHeight * referenceAspect);
            viewport.y = 0;
            viewport.x = (windowWidth - viewport.width) / 2;
        } else {
            // Window is taller -> scale by width (crops top/bottom)
            viewport.width = windowWidth;
            viewport.height = (int)(windowWidth / referenceAspect);
            viewport.x = 0;
            viewport.y = (windowHeight - viewport.height) / 2;
        }
        
        viewport.scaleX = std::max(GetScaleX(), GetScaleY()) * globalScale;
        viewport.scaleY = std::max(GetScaleX(), GetScaleY()) * globalScale;
        
        viewport.letterboxOffsetX = (float)viewport.x;
        viewport.letterboxOffsetY = (float)viewport.y;
        
        return viewport;
    }
    
    ViewportInfo ViewportManager::CalculateIntegerViewport() const {
        ViewportInfo viewport;
        
        // Calculate integer scale factor
        int scaleFactorX = std::max(1, windowWidth / referenceWidth);
        int scaleFactorY = std::max(1, windowHeight / referenceHeight);
        int scaleFactor = std::min(scaleFactorX, scaleFactorY);
        
        // Apply global scale
        scaleFactor = std::max(1, (int)(scaleFactor * globalScale));
        
        viewport.width = referenceWidth * scaleFactor;
        viewport.height = referenceHeight * scaleFactor;
        viewport.x = (windowWidth - viewport.width) / 2;
        viewport.y = (windowHeight - viewport.height) / 2;
        
        viewport.scaleX = (float)scaleFactor;
        viewport.scaleY = (float)scaleFactor;
        
        viewport.letterboxOffsetX = (float)viewport.x;
        viewport.letterboxOffsetY = (float)viewport.y;
        
        return viewport;
    }
    
    float ViewportManager::GetAspectRatio() const {
        return (float)referenceWidth / (float)referenceHeight;
    }
    
    float ViewportManager::GetWindowAspectRatio() const {
        return (float)windowWidth / (float)windowHeight;
    }
    
    ScalingMode ViewportManager::ParseScalingMode(const std::string& modeStr) {
        if (modeStr == "STRETCH") return ScalingMode::STRETCH;
        if (modeStr == "LETTER") return ScalingMode::LETTER;
        if (modeStr == "PILLAR") return ScalingMode::PILLAR;
        if (modeStr == "FIT") return ScalingMode::FIT;
        if (modeStr == "CROP") return ScalingMode::CROP;
        if (modeStr == "INTEGER") return ScalingMode::INTEGER;
        
        std::cout << "[ViewportManager] Unknown scaling mode: " << modeStr << ", using FIT" << std::endl;
        return ScalingMode::FIT;
    }
    
    UpscalingTechnique ViewportManager::ParseUpscalingTechnique(const std::string& techniqueStr) {
        if (techniqueStr == "NEAREST") return UpscalingTechnique::NEAREST;
        if (techniqueStr == "LINEAR") return UpscalingTechnique::LINEAR;
        if (techniqueStr == "TAAU") return UpscalingTechnique::TAAU;
        if (techniqueStr == "DLSS") return UpscalingTechnique::DLSS;
        if (techniqueStr == "FSR") return UpscalingTechnique::FSR;
        if (techniqueStr == "XeSS") return UpscalingTechnique::XeSS;
        
        std::cout << "[ViewportManager] Unknown upscaling technique: " << techniqueStr << ", using LINEAR" << std::endl;
        return UpscalingTechnique::LINEAR;
    }
    
    std::string ViewportManager::ScalingModeToString(ScalingMode mode) {
        switch(mode) {
            case ScalingMode::STRETCH: return "STRETCH";
            case ScalingMode::LETTER: return "LETTER";
            case ScalingMode::PILLAR: return "PILLAR";
            case ScalingMode::FIT: return "FIT";
            case ScalingMode::CROP: return "CROP";
            case ScalingMode::INTEGER: return "INTEGER";
            default: return "FIT";
        }
    }
    
    std::string ViewportManager::UpscalingTechniqueToString(UpscalingTechnique technique) {
        switch(technique) {
            case UpscalingTechnique::NEAREST: return "NEAREST";
            case UpscalingTechnique::LINEAR: return "LINEAR";
            case UpscalingTechnique::TAAU: return "TAAU";
            case UpscalingTechnique::DLSS: return "DLSS";
            case UpscalingTechnique::FSR: return "FSR";
            case UpscalingTechnique::XeSS: return "XeSS";
            default: return "LINEAR";
        }
    }
    
    void ViewportManager::LoadFromConfig() {
        using namespace Engine::Settings;
        
        // Load reference resolution
        referenceWidth = Config::GetInt("Render.Resolution.Width", 1280);
        referenceHeight = Config::GetInt("Render.Resolution.Height", 720);
        
        // Load scale factor
        globalScale = Config::GetFloat("Render.Resolution.Scale", 1.0f);
        
        // Load scaling mode
        std::string modeStr = Config::GetString("Render.Resolution.Mode", "FIT");
        scalingMode = ParseScalingMode(modeStr);
        
        // Load upscaling technique
        std::string techniqueStr = Config::GetString("Render.Resolution.Technique", "LINEAR");
        upscalingTechnique = ParseUpscalingTechnique(techniqueStr);
        
        std::cout << "[ViewportManager] Loaded config - Resolution: " << referenceWidth << "x" << referenceHeight 
                  << ", Scale: " << globalScale << ", Mode: " << ScalingModeToString(scalingMode) 
                  << ", Technique: " << UpscalingTechniqueToString(upscalingTechnique) << std::endl;
    }
}