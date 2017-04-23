#ifndef GRID_H
#define GRID_H

bool InitGrid();
void load_atlas(char * filename, int w, int h);
void paint_tile(int id, int x, int y);
void update_screen();

#endif /* end of include guard: GRID_H */
