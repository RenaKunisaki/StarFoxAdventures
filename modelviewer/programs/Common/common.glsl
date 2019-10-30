


void drawPoint(vec4 pos, vec4 color, float size) {
    /** Draw a square point. Mainly used for debugging.
     *  pos:   Coordinates of the point's centre.
     *  color: Color of the point.
     *  size:  Width and height of the point.
     */
    float s = size / 2.0;
    drawVtx(vec4(pos.x-s, pos.y-s, pos.zw), color, vec2(0,0));
    drawVtx(vec4(pos.x-s, pos.y+s, pos.zw), color, vec2(0,0));
    drawVtx(vec4(pos.x+s, pos.y-s, pos.zw), color, vec2(0,0));
    drawVtx(vec4(pos.x+s, pos.y+s, pos.zw), color, vec2(0,0));
    EndPrimitive();
}

void drawBinary(int val, vec4 pos, int bits) {
    /** Debug: draw value in binary.
     *  val:  value to draw.
     *  pos:  position to draw at.
     *  bits: number of bits to draw.
     *  This can quickly run into the max_vertices limit
     *  if drawing 32 bits at a time.
     */
    for(int i=bits-1; i>=0; i--) {
        drawPoint(
            vec4(pos.x + (i*10) + (i/8)*5, pos.yzw),
            vec4(
                0, ((val & (1 << ((bits-1)-i))) != 0) ? 1 : 0,
                0, 1),
            8);
    }
}
