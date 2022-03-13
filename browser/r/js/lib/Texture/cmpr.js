import { read_u16, read_u32, write_u16, write_u32 } from "./bits.js";
import { convert_rgb565_to_color, convert_color_to_rgb565 } from "./color.js";

export function get_interpolated_cmpr_colors(color_0_rgb565, color_1_rgb565) {
    let r0, g0, b0, r1, g1, b1, _, color_0, color_1, color_2, color_3;
    color_0 = convert_rgb565_to_color(color_0_rgb565);
    color_1 = convert_rgb565_to_color(color_1_rgb565);
    [r0, g0, b0, _] = color_0;
    [r1, g1, b1, _] = color_1;
    if(color_0_rgb565 > color_1_rgb565) {
        color_2 = [
            Math.trunc((2*r0 + 1*r1)/3),
            Math.trunc((2*g0 + 1*g1)/3),
            Math.trunc((2*b0 + 1*b1)/3),
            255
        ];
        color_3 = [
            Math.trunc((1*r0 + 2*r1)/3),
            Math.trunc((1*g0 + 2*g1)/3),
            Math.trunc((1*b0 + 2*b1)/3),
            255
        ];
    }
    else {
        color_2 = [
            Math.trunc(r0/2) + Math.trunc(r1/2),
            Math.trunc(g0/2) + Math.trunc(g1/2),
            Math.trunc(b0/2) + Math.trunc(b1/2),
            255
        ];
        color_3 = [0, 0, 0, 0];
    }
    let colors = [color_0, color_1, color_2, color_3];
    return colors;
}
export function get_best_cmpr_key_colors(all_colors) {
    let max_dist = -1;
    let color_1 = null;
    let color_2 = null;
    for(let i=0; i < all_colors.length; i++) {
        let curr_color_1 = all_colors[i];
        for(let j=i+1; j < all_colors.length; j++) {
            let curr_color_2 = all_colors[j];
            let curr_dist = get_color_distance_fast(curr_color_1, curr_color_2);
            if(curr_dist > max_dist) {
                max_dist = curr_dist;
                color_1 = curr_color_1;
                color_2 = curr_color_2;
            }
        }
    }

    if(max_dist == -1) {
        return [[0, 0, 0, 0xFF], [0xFF, 0xFF, 0xFF, 0xFF]];
    }
    else {
        let [r1, g1, b1, a1] = color_1;
        color_1 = [r1, g1, b1, 0xFF];
        let [r2, g2, b2, a2] = color_2;
        color_2 = [r2, g2, b2, 0xFF];

        if((r1 >> 3) == (r2 >> 3) && (g1 >> 2) == (g2 >> 2)
        && (b1 >> 3) == (b2 >> 3)) {
            if((r1 >> 3) == 0 && (g1 >> 2) == 0 && (b1 >> 3) == 0) {
                color_2 = [0xFF, 0xFF, 0xFF, 0xFF];
            }
            else {
                color_2 = [0, 0, 0, 0xFF];
            }
        }
        return [color_1, color_2];
    }
}

export function decode_cmpr_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let i=0; i<64; i++) pixel_color_data.push(null);

    let subblock_offset = offset;
    for(let subblock_index=0; subblock_index < 4; subblock_index++) {
        let subblock_x = (subblock_index%2)*4;
        let subblock_y = Math.trunc(subblock_index/2)*4;

        let color_0_rgb565 = read_u16(image_data, subblock_offset);
        let color_1_rgb565 = read_u16(image_data, subblock_offset+2);
        let colors = get_interpolated_cmpr_colors(color_0_rgb565, color_1_rgb565);

        let color_indexes = read_u32(image_data, subblock_offset+4);
        for(let i=0; i<16; i++) {
            let color_index = ((color_indexes >> ((15-i)*2)) & 3);
            let color = colors[color_index];

            let x_in_subblock = i % 4;
            let y_in_subblock = Math.trunc(i / 4);
            let pixel_index_in_block = subblock_x + subblock_y*8 + y_in_subblock*8 + x_in_subblock;
            pixel_color_data[pixel_index_in_block] = color;
        }
        subblock_offset += 8;
    }
    return pixel_color_data;
}

export function encode_image_to_cmpr_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = new BytesIO();
    let subblock_offset = 0;
    for(let subblock_index=0; subblock_index<4; subblock_index++) {
        let subblock_x = block_x + (subblock_index%2)*4;
        let subblock_y = block_y + Math.trunc(subblock_index/2)*4;

        let all_colors_in_subblock = [];
        let needs_transparent_color = false;
        for(let i=0; i<16; i++) {
            let x_in_subblock = i % 4;
            let y_in_subblock = Math.trunc(i / 4);
            let x = subblock_x+x_in_subblock;
            let y = subblock_y+y_in_subblock;
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                continue;
            }

            let color = pixels.getPixel(x,y);
            let [r, g, b, a] = get_rgba(color);
            if(a < 16) needs_transparent_color = true;
            else all_colors_in_subblock.push(color);
        }

        let [color_0, color_1] = get_best_cmpr_key_colors(all_colors_in_subblock);
        let color_0_rgb565 = convert_color_to_rgb565(color_0);
        let color_1_rgb565 = convert_color_to_rgb565(color_1);

        if(needs_transparent_color && color_0_rgb565 > color_1_rgb565) {
            color_0_rgb565, color_1_rgb565 = color_1_rgb565, color_0_rgb565;
            color_0, color_1 = color_1, color_0;
        }
        else if(!needs_transparent_color && color_0_rgb565 < color_1_rgb565) {
            color_0_rgb565, color_1_rgb565 = color_1_rgb565, color_0_rgb565;
            color_0, color_1 = color_1, color_0;
        }

        let colors = get_interpolated_cmpr_colors(color_0_rgb565, color_1_rgb565);
        colors[0] = color_0;
        colors[1] = color_1;

        write_u16(new_data, subblock_offset, color_0_rgb565);
        write_u16(new_data, subblock_offset+2, color_1_rgb565);

        let color_indexes = 0;
        for(let i=0; i<16; i++) {
            let x_in_subblock = i % 4;
            let y_in_subblock = Math.trunc(i / 4);
            let x = subblock_x+x_in_subblock;
            let y = subblock_y+y_in_subblock;
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                continue;
            }

            let color = pixels[x,y];

            let color_index;
            if(colors[color] != undefined) color_index = colors.indexOf(color);
            else {
                let new_color = get_nearest_color_fast(color, colors);
                color_index = colors.indexOf(new_color);
            }
            color_indexes |= (color_index << ((15-i)*2));
        }
        write_u32(new_data, subblock_offset+4, color_indexes);
        subblock_offset += 8;
    }
    new_data.seek(0);
    return new_data.read();
}
