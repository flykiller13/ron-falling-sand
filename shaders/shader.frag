#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

// Cell type constants
const int EMPTY = 0;
const int STONE = 1;
const int SAND = 2;
const int WATER = 3;
const int GAS = 4;

struct Cell {
    int type;
};

layout(std430, binding = 0) readonly buffer CellBuffer {
    Cell cells[];
} cellData;

uniform int width;
uniform int height;

vec4 getColorForCell(int type) {
    switch (type) {
        case SAND:
        return vec4(1.0, 1.0, 0.0, 1.0);// Yellow
        case WATER:
        return vec4(0.0, 0.0, 1.0, 0.7);// Blue
        case STONE:
        return vec4(0.6, 0.6, 0.6, 1.0);// Grey
        case GAS:
        return vec4(0.2, 0.5, 0.1, 0.3);// Gas
        case EMPTY:
        return vec4(0.0, 0.0, 0.0, 1.0);// Black
        default :// Debug
        return vec4(1.0, 0.0, 1.0, 1.0);// Magenta
    }
}

void main()
{
    // Convert texture coordinates to grid position
    ivec2 gridPos = ivec2(TexCoord.x * width, TexCoord.y * height);

    // Clamp to bounds
    gridPos.x = clamp(gridPos.x, 0, width - 1);
    gridPos.y = clamp(gridPos.y, 0, height - 1);

    // Calculate index in the cell buffer
    int idx = gridPos.y * width + gridPos.x;

    // Read cell type from buffer
    int cellType = cellData.cells[idx].type;

    // Get color based on cell type
    FragColor = getColorForCell(cellType);
}