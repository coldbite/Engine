#include "Table.h"
#include "../../IRenderingAPI.h"
#include "../../RGBA.h"
#include <algorithm>

Table::Table() 
    : rowHeight(30.0f), headerHeight(35.0f), autoColumnWidths(true), showBorders(false) {
    InitializeDefaultStyles();
}

Table::~Table() {
    for (auto& pair : styles) {
        delete pair.second;
    }
}

void Table::InitializeDefaultStyles() {
    styles[HEADER_BACKGROUND] = new Engine::Graphics::RGBA(60, 60, 80, 200);
    styles[HEADER_TEXT_COLOR] = new Engine::Graphics::RGBA(255, 255, 255, 255);
    styles[ROW_BACKGROUND] = new Engine::Graphics::RGBA(0, 0, 0, 127); // 50% alpha = 127
    styles[ROW_ALT_BACKGROUND] = new Engine::Graphics::RGBA(0, 0, 0, 127); // Same as ROW_BACKGROUND for uniform color
    styles[ROW_TEXT_COLOR] = new Engine::Graphics::RGBA(220, 220, 220, 255);
    styles[BORDER_COLOR] = new Engine::Graphics::RGBA(100, 100, 120, 255);
    styles[SELECTED_ROW_BACKGROUND] = new Engine::Graphics::RGBA(70, 120, 180, 200);
    
    // Initialize text styles
    textStyles[HEADER_TEXT_STYLE] = Engine::Graphics::FontStyle::BOLD;
    textStyles[ROW_TEXT_STYLE] = Engine::Graphics::FontStyle::NORMAL;
}

void Table::AddHeader(const std::string& text, int colspan, HorizontalAlignment alignment) {
    auto header = std::make_unique<Engine::Graphics::Text>();
    header->SetValue(text);
    SetupTextStyle(*header, true);
    headers.emplace_back(std::move(header), colspan, alignment);
}

void Table::SetHeader(int index, const std::string& text, HorizontalAlignment alignment) {
    if (index >= 0 && index < static_cast<int>(headers.size())) {
        headers[index].text->SetValue(text);
        headers[index].alignment = alignment;
        SetupTextStyle(*headers[index].text, true);
    }
}

void Table::ClearHeaders() {
    headers.clear();
    columnWidths.clear();
}

void Table::RemoveHeader(int index) {
    if (index >= 0 && index < static_cast<int>(headers.size())) {
        headers.erase(headers.begin() + index);
        if (index < static_cast<int>(columnWidths.size())) {
            columnWidths.erase(columnWidths.begin() + index);
        }
        
        // Remove corresponding column from all rows
        for (auto& row : rows) {
            if (index < static_cast<int>(row.size())) {
                row.erase(row.begin() + index);
            }
        }
    }
}

void Table::AddRow(const std::vector<std::string>& rowData, HorizontalAlignment alignment) {
    std::vector<HorizontalAlignment> alignments(rowData.size(), alignment);
    AddRow(rowData, alignments);
}

void Table::AddRow(const std::vector<std::string>& rowData, const std::vector<HorizontalAlignment>& alignments) {
    rows.emplace_back();
    rowAlignments.emplace_back();
    auto& row = rows.back();
    auto& rowAlign = rowAlignments.back();
    
    for (size_t i = 0; i < rowData.size(); ++i) {
        auto cell = std::make_unique<Engine::Graphics::Text>();
        cell->SetValue(rowData[i]);
        SetupTextStyle(*cell, false);
        row.push_back(std::move(cell));
        
        // Store alignment (use LEFT if alignments vector is too small)
        HorizontalAlignment cellAlignment = (i < alignments.size()) ? alignments[i] : HorizontalAlignment::LEFT;
        rowAlign.push_back(cellAlignment);
    }
}

void Table::SetRow(int rowIndex, const std::vector<std::string>& rowData, HorizontalAlignment alignment) {
    std::vector<HorizontalAlignment> alignments(rowData.size(), alignment);
    SetRow(rowIndex, rowData, alignments);
}

void Table::SetRow(int rowIndex, const std::vector<std::string>& rowData, const std::vector<HorizontalAlignment>& alignments) {
    if (rowIndex >= 0 && rowIndex < static_cast<int>(rows.size())) {
        rows[rowIndex].clear();
        
        // Ensure rowAlignments has the same size as rows
        if (rowIndex >= static_cast<int>(rowAlignments.size())) {
            rowAlignments.resize(rowIndex + 1);
        }
        rowAlignments[rowIndex].clear();
        
        for (size_t i = 0; i < rowData.size(); ++i) {
            auto cell = std::make_unique<Engine::Graphics::Text>();
            cell->SetValue(rowData[i]);
            SetupTextStyle(*cell, false);
            rows[rowIndex].push_back(std::move(cell));
            
            // Store alignment
            HorizontalAlignment cellAlignment = (i < alignments.size()) ? alignments[i] : HorizontalAlignment::LEFT;
            rowAlignments[rowIndex].push_back(cellAlignment);
        }
    }
}

void Table::SetRowCell(int rowIndex, int columnIndex, const std::string& text, HorizontalAlignment alignment) {
    if (rowIndex >= 0 && rowIndex < static_cast<int>(rows.size()) &&
        columnIndex >= 0 && columnIndex < static_cast<int>(rows[rowIndex].size())) {
        rows[rowIndex][columnIndex]->SetValue(text);
        SetupTextStyle(*rows[rowIndex][columnIndex], false);
        
        // Update alignment if rowAlignments exists for this row
        if (rowIndex < static_cast<int>(rowAlignments.size()) &&
            columnIndex < static_cast<int>(rowAlignments[rowIndex].size())) {
            rowAlignments[rowIndex][columnIndex] = alignment;
        }
    }
}

void Table::ClearRows() {
    rows.clear();
    rowAlignments.clear();
}

void Table::RemoveRow(int rowIndex) {
    if (rowIndex >= 0 && rowIndex < static_cast<int>(rows.size())) {
        rows.erase(rows.begin() + rowIndex);
        if (rowIndex < static_cast<int>(rowAlignments.size())) {
            rowAlignments.erase(rowAlignments.begin() + rowIndex);
        }
    }
}

void Table::SetStyle(StyleProperty property, Engine::Graphics::IColor* color) {
    // Don't delete the color - it might be managed elsewhere
    // Only delete if it's different from the new one
    if (styles.find(property) != styles.end() && styles[property] != color) {
        delete styles[property];
    }
    styles[property] = color;
}

void Table::SetStyle(StyleProperty property, Engine::Graphics::FontStyle style) {
    textStyles[property] = style;
}

Engine::Graphics::IColor* Table::GetStyle(StyleProperty property) {
    auto it = styles.find(property);
    if (it != styles.end()) {
        return it->second;
    }
    return nullptr;
}

Engine::Graphics::FontStyle Table::GetTextStyle(StyleProperty property) {
    auto it = textStyles.find(property);
    if (it != textStyles.end()) {
        return it->second;
    }
    return Engine::Graphics::FontStyle::NORMAL;
}

void Table::SetColumnWidth(int columnIndex, float width) {
    if (columnIndex >= 0) {
        if (columnIndex >= static_cast<int>(columnWidths.size())) {
            columnWidths.resize(columnIndex + 1, 100.0f);
        }
        columnWidths[columnIndex] = width;
        autoColumnWidths = false;
    }
}

float Table::GetColumnWidth(int columnIndex) const {
    if (columnIndex >= 0 && columnIndex < static_cast<int>(columnWidths.size())) {
        return columnWidths[columnIndex];
    }
    return 100.0f;
}

void Table::SetAutoColumnWidths(bool enabled) {
    autoColumnWidths = enabled;
}

void Table::CalculateColumnWidths(float totalWidth) {
    if (headers.empty()) return;

    int numColumns = GetColumnCount(); // Use GetColumnCount() instead of headers.size()
    columnWidths.resize(numColumns);

    if (autoColumnWidths) {
        float columnWidth = totalWidth / numColumns;
        for (int i = 0; i < numColumns; ++i) {
            columnWidths[i] = columnWidth;
        }
    } else {
        // Normalize existing widths to fit total width
        float totalSetWidth = 0.0f;
        for (float width : columnWidths) {
            totalSetWidth += width;
        }
        if (totalSetWidth > 0) {
            float scale = totalWidth / totalSetWidth;
            for (float& width : columnWidths) {
                width *= scale;
            }
        }
    }
}

void Table::SetupTextStyle(Engine::Graphics::Text& text, bool isHeader) {
    text.SetFont("Sansation");
    if (isHeader) {
        text.SetSize(16.0f);
        text.SetColor(GetStyle(HEADER_TEXT_COLOR));
        text.SetStyle(GetTextStyle(HEADER_TEXT_STYLE));
    } else {
        text.SetSize(14.0f);
        text.SetColor(GetStyle(ROW_TEXT_COLOR));
        text.SetStyle(GetTextStyle(ROW_TEXT_STYLE));
    }
    text.SetPadding(2.0f, 2.0f); // Reduced padding for better alignment control
}

void Table::Update(float deltaTime) {
    for (auto& header : headers) {
        header.text->Update(deltaTime);
    }
    for (auto& row : rows) {
        for (auto& cell : row) {
            cell->Update(deltaTime);
        }
    }
}

void Table::Render(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height) {
    if (headers.empty()) return;

    CalculateColumnWidths(width);

    float currentY = y;
    
    // Render headers
    RenderHeaders(context, x, currentY, width);
    currentY += headerHeight;

    // Render rows
    float availableHeight = height - headerHeight;
    RenderRows(context, x, currentY, width, availableHeight);
}

void Table::RenderHeaders(Engine::Graphics::IRenderingAPI& context, float x, float y, float width) {
    // Draw header background
    context.DrawRect(x, y, width, headerHeight, GetStyle(HEADER_BACKGROUND));

    float currentX = x;
    int columnIndex = 0;
    
    for (size_t i = 0; i < headers.size(); ++i) {
        const HeaderInfo& headerInfo = headers[i];
        
        // Calculate the width for this header (sum of colspan columns)
        float headerWidth = 0.0f;
        for (int span = 0; span < headerInfo.colspan && (columnIndex + span) < static_cast<int>(columnWidths.size()); ++span) {
            headerWidth += columnWidths[columnIndex + span];
        }

        // Draw header border
        if (showBorders && i > 0) {
            context.DrawRect(currentX - 1, y, 2, headerHeight, GetStyle(BORDER_COLOR));
        }

        // Render header text with alignment
        float textX = currentX + 5.0f; // Default left alignment
        float textY = y + (headerHeight - 20.0f) / 2;
        
        switch (headerInfo.alignment) {
            case HorizontalAlignment::LEFT:
                textX = currentX + 5.0f;
                break;
            case HorizontalAlignment::CENTER:
                textX = currentX + (headerWidth / 2.0f);
                break;
            case HorizontalAlignment::RIGHT:
                textX = currentX + headerWidth - 5.0f;
                break;
        }
        
        headerInfo.text->Render(context, textX, textY);

        currentX += headerWidth;
        columnIndex += headerInfo.colspan;
    }

    // Draw bottom border for header
    if (showBorders) {
        context.DrawRect(x, y + headerHeight - 1, width, 2, GetStyle(BORDER_COLOR));
    }
}

void Table::RenderRows(Engine::Graphics::IRenderingAPI& context, float x, float y, float width, float height) {
    float currentY = y;
    float rowSpacing = 1.0f; // 1px spacing between rows
    float adjustedRowHeight = rowHeight - rowSpacing;
    int maxVisibleRows = static_cast<int>(height / rowHeight);
    int rowsToShow = std::min(maxVisibleRows, static_cast<int>(rows.size()));

    for (int rowIndex = 0; rowIndex < rowsToShow; ++rowIndex) {
        // Use uniform row background (both ROW_BACKGROUND and ROW_ALT_BACKGROUND are same now)
        Engine::Graphics::IColor* rowBg = GetStyle(ROW_BACKGROUND);
        
        context.DrawRect(x, currentY, width, adjustedRowHeight, rowBg);

        float currentX = x;
        const auto& row = rows[rowIndex];

        for (size_t colIndex = 0; colIndex < row.size() && colIndex < columnWidths.size(); ++colIndex) {
            float columnWidth = columnWidths[colIndex];

            // Render cell text with alignment
            if (colIndex < row.size()) {
                float textY = currentY + (adjustedRowHeight - 18.0f) / 2;
                
                // Get alignment for this cell if available
                HorizontalAlignment alignment = HorizontalAlignment::LEFT; // Default
                if (rowIndex < static_cast<int>(rowAlignments.size()) &&
                    colIndex < rowAlignments[rowIndex].size()) {
                    alignment = rowAlignments[rowIndex][colIndex];
                }
                
                // Use TextAlignment for proper alignment
                Engine::Graphics::TextAlignment textAlignment;
                switch (alignment) {
                    case HorizontalAlignment::LEFT:
                        textAlignment = Engine::Graphics::TextAlignment(HorizontalAlignment::LEFT, VerticalAlignment::CENTER);
                        break;
                    case HorizontalAlignment::CENTER:
                        textAlignment = Engine::Graphics::TextAlignment(HorizontalAlignment::CENTER, VerticalAlignment::CENTER);
                        break;
                    case HorizontalAlignment::RIGHT:
                        textAlignment = Engine::Graphics::TextAlignment(HorizontalAlignment::RIGHT, VerticalAlignment::CENTER);
                        break;
                }
                
                // Set alignment position based on column bounds
                float centerX = currentX + (columnWidth / 2.0f);
                float centerY = textY;
                
                // Temporarily adjust text position for alignment
                float textX = currentX + 5.0f; // LEFT default
                if (alignment == HorizontalAlignment::CENTER) {
                    textX = centerX - 10.0f; // Approximate center offset
                } else if (alignment == HorizontalAlignment::RIGHT) {
                    textX = currentX + columnWidth - 15.0f; // Right with padding
                }
                
                row[colIndex]->Render(context, textX, textY);
            }

            currentX += columnWidth;
        }

        currentY += rowHeight; // Full rowHeight includes the spacing
    }
}