void set_cells(cell **cells, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        cells[i] = malloc(sizeof(cell) * columns);

        for (int j = 0; j < columns; j++) {
            cells[i][j].alive = false;
            cells[i][j].rect = { 
                .x = ,
                .y = ,
                .w = RECT_WIDTH,
                .h = RECT_HEIGHT
            };
        }
    }
}
