void drawSierpinski(int x0, int y0, int x1, int y1, int x2, int y2, int depth) {
  if (depth == 0) {
// draw filled triangle
    triangle(x0, y0, x1, y1, x2, y2);
    return;
  }
// compute midpoints
  int x01 = (x0 + x1) / 2; int y01 = (y0 + y1) / 2;
  int x12 = (x1 + x2) / 2; int y12 = (y1 + y2) / 2;
  int x20 = (x2 + x0) / 2; int y20 = (y2 + y0) / 2;

// recurse on three sub-triangles
  drawSierpinski(x0, y0, x01, y01, x20, y20, depth - 1);
  drawSierpinski(x01, y01, x1, y1, x12, y12, depth - 1);
  drawSierpinski(x20, y20, x12, y12, x2, y2, depth - 1);
}
void main()
{
 int w = 800;
  int h = 480;
// main triangle (fits screen)
  int x0 = w / 2;
  int  y0 = 0;
  int x1 = 0;
 int y1 = h - 1;
  int x2 = w - 1;
  int y2 = h - 1;

// choose recursion depth depending on resolution; 6..8 is common
  int depth = 7;
  drawSierpinski(x0, y0, x1, y1, x2, y2, depth); 
}