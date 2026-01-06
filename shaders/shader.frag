#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

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
        case 1:// Sand
        return vec4(1.0, 1.0, 0.0, 1.0);// Yellow
        case 2:// Water
        return vec4(27.0/255.0, 81.0/255.0, 1.0, 200.0/255.0);// Blue
        case 3:// Gas
        return vec4(42.0/255.0, 132.0/255.0, 24.0/255.0, 80.0/255.0);// Gas
        case 4:// Stone
        return vec4(149.0/255.0, 149.0/255.0, 149.0/255.0, 1.0);// Grey
        case 5:// Debug - Workgroup border
        return vec4(1.0, 1.0, 1.0, 1.0);// White
        case 0:// Empty
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