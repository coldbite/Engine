#pragma once
#include "../../IColor.h"
#include "../Text/Text.h"
#include "../Alignment.h"
#include <vector>
#include <string>
#include <map>

namespace Engine {
    namespace Graphics {
        class IRenderingAPI;
    }
}

class Table {
public:
    enum StyleProperty {
        HEADER_BACKGROUND,
        HEADER_TEXT_COLOR,
        HEADER_TEXT_STYLE,
        ROW_BACKGROUND,
        ROW_ALT_BACKGROUND,
        ROW_TEXT_COLOR,
        ROW_TEXT_STYLE,
        BORDER_COLOR,
        SELECTED_ROW_BACKGROUND
    };

    Table();
    ~Table();

    // Header methods
    void AddHeader(const std::string& text, int colspan = 1, HorizontalAlignment alignment = HorizontalAlignment::LEFT);
    void SetHeader(int index, const std::string& text, HorizontalAlignment alignment = HorizontalAlignment::LEFT);
    void ClearHeaders();
    void RemoveHeader(int index);

    // Row methods
    void AddRow(const std::vector<std::string>& rowData, HorizontalAlignment alignment = HorizontalAlignment::LEFT);
    void AddRow(const std::vector<std::string>& rowData, const std::vector<HorizontalAlignment>& alignments);
    void SetRow(int rowIndex, const std::vector<std::string>& rowData, HorizontalAlignment alignment = HorizontalAlignment::LEFT);
    void SetRow(int rowIndex, const std::vector<std::string>& rowData, const std::vector<HorizontalAlignment>& alignments);
    void SetRowCell(int rowIndex, int columnIndex, const std::string& text, HorizontalAlignment alignment = HorizontalAlignment::LEFT);
    void ClearRows();
    void RemoveRow(int rowIndex);

    // Style methods
    void SetStyle(StyleProperty property, Engine::Graphics::IColor* color);
    void SetStyle(StyleProperty property, Engine::Graphics::FontStyle style);
    Engine::Graphics::IColor* GetStyle(StyleProperty property);
    Engine::Graphics::FontStyle GetTextStyle(StyleProperty property);

    // Column width methods
    void SetColumnWidth(int columnIndex, float width);
    float GetColumnWidth(int columnIndex) const;
    void SetAutoColumnWidths(bool enabled);

    // Rendering
    void Render(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height);
    void Update(float deltaTime);

    // Utility methods
    int GetRowCount() const { return static_cast<int>(rows.size()); }
    int GetColumnCount() const { 
        int totalColumns = 0;
        for (const auto& header : headers) {
            totalColumns += header.colspan;
        }
        return totalColumns;
    }
    void SetRowHeight(float height) { rowHeight = height; }
    void SetHeaderHeight(float height) { headerHeight = height; }

private:
    struct HeaderInfo {
        std::unique_ptr<Engine::Graphics::Text> text;
        int colspan;
        HorizontalAlignment alignment;
        
        HeaderInfo(std::unique_ptr<Engine::Graphics::Text> t, int span, HorizontalAlignment align = HorizontalAlignment::LEFT) 
            : text(std::move(t)), colspan(span), alignment(align) {}
    };
    
    std::vector<HeaderInfo> headers;
    std::vector<std::vector<std::unique_ptr<Engine::Graphics::Text>>> rows;
    std::vector<std::vector<HorizontalAlignment>> rowAlignments;
    std::map<StyleProperty, Engine::Graphics::IColor*> styles;
    std::map<StyleProperty, Engine::Graphics::FontStyle> textStyles;
    std::vector<float> columnWidths;
    
    float rowHeight;
    float headerHeight;
    bool autoColumnWidths;
    bool showBorders;
    
    void InitializeDefaultStyles();
    void CalculateColumnWidths(float totalWidth);
    void RenderHeaders(Engine::Graphics::IRenderingAPI& context, float x, float y, float width);
    void RenderRows(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height);
    void SetupTextStyle(Engine::Graphics::Text& text, bool isHeader);
};