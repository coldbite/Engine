#include "Box.h"
#include "../../IRenderingAPI.h"
#include <cmath>

namespace Engine {
    namespace Graphics {
        // Define TextAlignment static constants
        const Alignment Alignment::TOP_LEFT(HorizontalAlignment::LEFT, VerticalAlignment::TOP);
        const Alignment Alignment::TOP_CENTER(HorizontalAlignment::CENTER, VerticalAlignment::TOP);
        const Alignment Alignment::TOP_RIGHT(HorizontalAlignment::RIGHT, VerticalAlignment::TOP);
        const Alignment Alignment::CENTER_LEFT(HorizontalAlignment::LEFT, VerticalAlignment::CENTER);
        const Alignment Alignment::CENTER(HorizontalAlignment::CENTER, VerticalAlignment::CENTER);
        const Alignment Alignment::CENTER_RIGHT(HorizontalAlignment::RIGHT, VerticalAlignment::CENTER);
        const Alignment Alignment::BOTTOM_LEFT(HorizontalAlignment::LEFT, VerticalAlignment::BOTTOM);
        const Alignment Alignment::BOTTOM_CENTER(HorizontalAlignment::CENTER, VerticalAlignment::BOTTOM);
        const Alignment Alignment::BOTTOM_RIGHT(HorizontalAlignment::RIGHT, VerticalAlignment::BOTTOM);

        Box::Box(float width, float height) : width(width), height(height),
            color(new RGBA(1.0f, 1.0f, 1.0f, 1.0f)),
            borderColor(new RGBA(0.0f, 0.0f, 0.0f, 1.0f)),
            borderWidth(0.0f), m_marginTop(0.0f), m_marginRight(0.0f), m_marginBottom(0.0f),
            m_marginLeft(0.0f), pulseEnabled(false), pulseTime(0.0f) {
        }

        void Box::Update(float deltaTime) {
            if (pulseEnabled) {
                pulseTime += deltaTime;
            }
        }

        void Box::SetColor(IColor* c) {
            color = c;
        }

        void Box::SetBorderColor(IColor* c) {
            borderColor = c;
        }

        void Box::SetMargin(float x, float y) {
            m_marginLeft = m_marginRight = x;
            m_marginTop = m_marginBottom = y;
        }

        void Box::SetMargin(float top, float right, float bottom, float left) {
            m_marginTop = top;
            m_marginRight = right;
            m_marginBottom = bottom;
            m_marginLeft = left;
        }

        void Box::Render(IRenderingAPI& context, float x, float y) {
            float renderX = x + m_marginLeft;
            float renderY = y + m_marginTop;
            float currentWidth = width;
            float currentHeight = height;
            IColor* currentColor = color;

            // Apply pulsing effect
            if (pulseEnabled) {
                float scale = 0.9f + 0.2f * (std::sin(pulseTime * 15.0f) + 0.5f) * 0.5f;
                float centerX = renderX + width * 0.5f;
                float centerY = renderY + height * 0.5f;
                currentWidth = width * scale;
                currentHeight = height * scale;
                renderX = centerX - currentWidth * 0.5f;
                renderY = centerY - currentHeight * 0.5f;
            }

            // Render the box
            context.DrawRect(renderX, renderY, currentWidth, currentHeight, currentColor);

            // Render border if specified
            if (borderWidth > 0.0f) {
                IColor* currentBorderColor = borderColor;

                // Draw border as four rectangles
                context.DrawRect(renderX, renderY, currentWidth, borderWidth, currentBorderColor); // Top
                context.DrawRect(renderX, renderY + currentHeight - borderWidth, currentWidth, borderWidth, currentBorderColor); // Bottom
                context.DrawRect(renderX, renderY, borderWidth, currentHeight, currentBorderColor); // Left
                context.DrawRect(renderX + currentWidth - borderWidth, renderY, borderWidth, currentHeight, currentBorderColor); // Right
            }
        }

        void Box::Render(IRenderingAPI& context, const Alignment& alignment) {
            float textWidth = GetWidth();
            float textHeight = GetHeight();
            float totalWidth = textWidth + m_marginLeft + m_marginRight;
            float totalHeight = textHeight + m_marginTop + m_marginBottom;

            // Get current rendering context dimensions (which should be reference resolution after Begin2D)
            const float REFERENCE_WIDTH = static_cast<float>(context.GetWidth());
            const float REFERENCE_HEIGHT = static_cast<float>(context.GetHeight());

            // Calculate X position based on horizontal alignment (in reference coordinates)
            float x = 0.0f;
            switch (alignment.horizontal) {
                case HorizontalAlignment::LEFT:
                    x = 0.0f;
                    break;
                case HorizontalAlignment::CENTER:
                    x = (REFERENCE_WIDTH - textWidth) / 2.0f;  // Use textWidth, not totalWidth
                    break;
                case HorizontalAlignment::RIGHT:
                    x = REFERENCE_WIDTH - totalWidth;
                    break;
            }

            // Calculate Y position based on vertical alignment (in reference coordinates)
            // Note: OpenGL uses glOrtho(0, width, height, 0, -1, 1) so Y=0 is TOP, Y=height is BOTTOM
            float y = 0.0f;
            switch (alignment.vertical) {
                case VerticalAlignment::TOP:
                    y = 0.0f;
                    break;
                case VerticalAlignment::CENTER:
                    // For top-left origin coordinate system: center = (height / 2) - (textHeight / 2)
                    y = (REFERENCE_HEIGHT / 2.0f) - (textHeight / 2.0f);
                    break;
                case VerticalAlignment::BOTTOM:
                    y = REFERENCE_HEIGHT - totalHeight;
                    break;
            }

            // Use the position-based render method (which will apply scaling)
            Render(context, x, y);
        }
    }
}
