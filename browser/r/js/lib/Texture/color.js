import { rgb2hsv, hsv2rgb } from "../../Util.js";
import { PackRGBA, UnpackRGBA } from "./Image.js";
import { swizzle_3_bit_to_8_bit, swizzle_4_bit_to_8_bit, swizzle_5_bit_to_8_bit, swizzle_6_bit_to_8_bit } from "./bits.js";
import { BugCheck, DataError, NotImplementedError } from "../../app/errors.js";

export function get_rgba(color) {
    switch(color.length) {
        case undefined: return UnpackRGBA(color);
        case 4: return color;
        case 3: return [color[0], color[1], color[2], 0xFF];
        default: throw new BugCheck(`Unexpected object for color: ${color}`);
    }
}

export function convert_rgb_to_greyscale(r, g, b) {
    return Math.round(((r * 30) + (g * 59) + (b * 11)) / 100);
}

export function convert_rgb565_to_color(rgb565) {
    return [
        swizzle_5_bit_to_8_bit(((rgb565 >> 11) & 0x1F)),
        swizzle_6_bit_to_8_bit(((rgb565 >>  5) & 0x3F)),
        swizzle_5_bit_to_8_bit(( rgb565        & 0x1F)),
        255];
}
export function convert_color_to_rgb565(color) {
    const [r, g, b, a] = get_rgba(color);
    return (
        (((r >> 3) & 0x1F) << 11) |
        (((g >> 2) & 0x3F) <<  5) |
         ((b >> 3) & 0x1F));
}
export function convert_rgb5a3_to_color(rgb5a3) {
    //RGB5A3 format.
    //Each color takes up two bytes.
    //Format depends on the most significant bit. Two possible formats:
    //Top bit is 0: 0AAARRRRGGGGBBBB
    //Top bit is 1: 1RRRRRGGGGGBBBBB (Alpha set to 0xff)
    if((rgb5a3 & 0x8000) == 0) {
        return [
            swizzle_4_bit_to_8_bit(((rgb5a3 >>  8) & 0xF)),
            swizzle_4_bit_to_8_bit(((rgb5a3 >>  4) & 0xF)),
            swizzle_4_bit_to_8_bit(( rgb5a3        & 0xF)),
            swizzle_3_bit_to_8_bit(((rgb5a3 >> 12) & 0x7)) ];
    }
    else {
        return [
            swizzle_5_bit_to_8_bit(((rgb5a3 >> 10) & 0x1F)),
            swizzle_5_bit_to_8_bit(((rgb5a3 >>  5) & 0x1F)),
            swizzle_5_bit_to_8_bit(( rgb5a3        & 0x1F)),
            255];
    }
}
export function convert_color_to_rgb5a3(color) {
    const [r, g, b, a] = get_rgba(color);
    if(a != 255) {
        return (
            (((a >> 5) & 0x7) << 12) |
            (((r >> 4) & 0xF) <<  8) |
            (((g >> 4) & 0xF) <<  4) |
             ((b >> 4) & 0xF));
    }
    else {
        return (0x8000 |
            (((r >> 3) & 0x1F) << 10) |
            (((g >> 3) & 0x1F) <<  5) |
             ((b >> 3) & 0x1F));
    }
}
export function convert_ia4_to_color(ia4) {
    let r, g, b, a;
    r = g = b = swizzle_4_bit_to_8_bit(ia4 & 0xF);
    a = swizzle_4_bit_to_8_bit((ia4 >> 4) & 0xF);
    return [r, g, b, a];
}
export function convert_color_to_ia4(color) {
    const [r, g, b, a] = get_rgba(color);
    return ((convert_rgb_to_greyscale(r, g, b) >> 4) & 0xF) | (a & 0xF0);
}
export function convert_ia8_to_color(ia8) {
    const low_byte = ia8 & 0xFF;
    return [low_byte, low_byte, low_byte, (ia8 >> 8) & 0xFF];
}
export function convert_color_to_ia8(color) {
    const [r, g, b, a] = get_rgba(color);
    return (convert_rgb_to_greyscale(r, g, b) & 0x00FF) | ((a << 8) & 0xFF00);
}
export function convert_i4_to_color(i4) {
    const v = swizzle_4_bit_to_8_bit(i4);
    return [v, v, v, v];
}
export function convert_color_to_i4(color) {
    const [r, g, b, a] = get_rgba(color);
    return ((convert_rgb_to_greyscale(r, g, b) >> 4) & 0xF);
}
export function convert_i8_to_color(i8) {
    return [i8, i8, i8, i8];
}
export function convert_color_to_i8(color) {
    const [r, g, b, a] = get_rgba(color);
    return convert_rgb_to_greyscale(r, g, b) & 0xFF;
}

//Picks a color from a palette that is visually the closest to the given color.
//Based off Aseprite's code: https://github.com/aseprite/aseprite/blob/cc7bde6cd1d9ab74c31ccfa1bf41a000150a1fb2/src/doc/palette.cpp#L226-L272
export function get_nearest_color_slow(color, palette) {
    if(palette[color] != undefined) return color;
    let [r, g, b, a] = get_rgba(color);

    if(a == 0) { // Transparent
        for(let indexed_color of palette) {
            if(indexed_color.length == 4 && indexed_color[3] == 0) {
                return indexed_color;
            }
        }
    }

    let min_dist = 9999999999.0;
    let value = null;

    let col_diff_g = [];
    let col_diff_r = [];
    let col_diff_b = [];
    let col_diff_a = [];
    for(let i=0; i<128; i++) {
        col_diff_g.push(0);
        col_diff_r.push(0);
        col_diff_b.push(0);
        col_diff_a.push(0);
    }
    for(let i=1; i<64; i++) {
        let k = i*i;
        col_diff_g[i] = col_diff_g[128-i] = k * 59 * 59;
        col_diff_r[i] = col_diff_r[128-i] = k * 30 * 30;
        col_diff_b[i] = col_diff_b[128-i] = k * 11 * 11;
        col_diff_a[i] = col_diff_a[128-i] = k * 8 * 8;
    }

    for(let indexed_color of palette) {
        let [r1, g1, b1, a1] = get_rgba(color);
        let [r2, g2, b2, a2] = get_rgba(indexed_color);
        r1 >>= 3;
        g1 >>= 3;
        b1 >>= 3;
        a1 >>= 3;
        r2 >>= 3;
        g2 >>= 3;
        b2 >>= 3;
        a2 >>= 3;

        let coldiff = col_diff_g[g2 - g1 & 127];
        if(coldiff < min_dist) {
            coldiff += col_diff_r[r2 - r1 & 127];
            if(coldiff < min_dist) {
                coldiff += col_diff_b[b2 - b1 & 127];
                    if(coldiff < min_dist) {
                    coldiff += col_diff_a[a2 - a1 & 127];
                    if(coldiff < min_dist) {
                        min_dist = coldiff;
                        value = indexed_color;
                    }
                }
            }
        }
    }
    return value;
}

export function get_nearest_color_fast(color, palette) {
    if(palette[color] != undefined) return color;

    let [r, g, b, a] = get_rgba(color);

    if(a < 16) { //Transparent
        for(let indexed_color of palette) {
            if(indexed_color.length == 4 && indexed_color[3] == 0) {
                return indexed_color;
            }
        }
    }

    let min_dist   = 0x7FFFFFFF;
    let best_color = palette[0];

    for(let indexed_color of palette) {
        let curr_dist = get_color_distance_fast(color, indexed_color);

        if(curr_dist < min_dist) {
            if(curr_dist == 0) {
                return indexed_color;
            }

            min_dist = curr_dist;
            best_color = indexed_color;
        }
    }
    return best_color;
}

export function get_color_distance_fast(color_1, color_2) {
    return (
        Math.abs(color_1[0] - color_2[0]) +
        Math.abs(color_1[1] - color_2[1]) +
        Math.abs(color_1[2] - color_2[2]));
}

//Generates a palette with a certain number of colors or less
//based on an image (color quantization).
export function create_limited_palette_from_image(image, max_colors) {
    // (2**depth) will be max_colors.
    let depth;
    if     (max_colors ==    16) depth =  4;
    else if(max_colors ==   256) depth =  8;
    else if(max_colors == 16384) depth = 14;
    else {
        throw new BugCheck("Unsupported maximum number of colors to generate a palette for: " + max_colors);
    }

    const all_pixel_colors = [];
    for(let y=0; y<image.height; y++) {
        for(let x=0; x<image.width; x++) {
            all_pixel_colors.push(image.getPixel(x, y));
        }
    }

    return split_colors_into_buckets(all_pixel_colors, depth);
}

export function split_colors_into_buckets(all_pixel_colors, depth) {
    if(depth == 0) return average_colors_together(all_pixel_colors);

    let r_min= 99999999, g_min= 99999999, b_min= 99999999;
    let r_max=-99999999, g_max=-99999999, b_max=-99999999;
    for(let c of all_pixel_colors) {
        let [r,g,b,a] = c;
        r_min = Math.min(r_min, r); r_max = Math.max(r_max, r);
        g_min = Math.min(g_min, g); g_max = Math.max(g_max, g);
        b_min = Math.min(b_min, b); b_max = Math.max(b_max, b);
    }
    let r_range = r_max - r_min;
    let g_range = g_max - g_min;
    let b_range = b_max - b_min;
    //r_range = max(r for r,g,b,a in all_pixel_colors) - min(r for r,g,b,a in all_pixel_colors)
    //g_range = max(g for r,g,b,a in all_pixel_colors) - min(g for r,g,b,a in all_pixel_colors)
    //b_range = max(b for r,g,b,a in all_pixel_colors) - min(b for r,g,b,a in all_pixel_colors)

    let channel_index_with_highest_range = 0;
    if(g_range >= r_range && g_range >= b_range) {
        channel_index_with_highest_range = 1;
    }
    else if(r_range >= g_range && r_range >= b_range) {
        channel_index_with_highest_range = 0;
    }
    else if(b_range >= r_range && b_range >= g_range) {
        channel_index_with_highest_range = 2;
    }

    //all_pixel_colors.sort(key=color => color[channel_index_with_highest_range]);
    all_pixel_colors.sort(
        (a,b) => a[channel_index_with_highest_range] - b[channel_index_with_highest_range]
    );
    const median_index = Math.trunc((all_pixel_colors.length+1)/2);
    const pA = split_colors_into_buckets(all_pixel_colors.slice(0,median_index), depth-1);
    const pB = split_colors_into_buckets(all_pixel_colors.slice(median_index), depth-1);
    return pA.concat(pB);
}

export function average_colors_together(colors) {
    let r_sum=0, g_sum=0, b_sum=0, a_sum=0;
    for(const c of colors) {
        r_sum += c[0];
        g_sum += c[1];
        b_sum += c[2];
        a_sum += c[3];
    }
    return [
        Math.trunc(r_sum/colors.length),
        Math.trunc(g_sum/colors.length),
        Math.trunc(b_sum/colors.length),
        Math.trunc(a_sum/colors.length),
    ];
}

export function color_exchange(image, base_color, replacement_color, mask_path=None, validate_mask_colors=True, ignore_bright=False) {
    throw new NotImplementedError();
    let mask_image;
    if(mask_path) {
        mask_image = Image.open(mask_path).convert("RGBA"); //XXX
        if(image.size != mask_image.size) {
            throw new DataError("Mask image is not the same size as the texture.");
        }
    }

    //When recoloring via native Python code, explicitly make a copy of the
    //image and modify that.
    //This is for consistency with the C function, which has to return a copy.
    image = image.copy();

    let mask_pixels;
    if(mask_path) mask_pixels = mask_image;

    let [base_r, base_g, base_b] = base_color;
    let [base_h, base_s, base_v] = rgb2hsv(base_r/255, base_g/255, base_b/255);
    base_h = Math.trunc(base_h*360);
    base_s = Math.trunc(base_s*100);
    base_v = Math.trunc(base_v*100);

    let [replacement_r, replacement_g, replacement_b] = replacement_color;
    let [replacement_h, replacement_s, replacement_v] = rgb2hsv(replacement_r/255, replacement_g/255, replacement_b/255);
    replacement_h = Math.trunc(replacement_h*360);
    replacement_s = Math.trunc(replacement_s*100);
    replacement_v = Math.trunc(replacement_v*100);

    let s_change = replacement_s - base_s;
    let v_change = replacement_v - base_v;

    let pixels = image.data;
    for(let x=0; x<image.width; x++) {
        for(let y=0; y<image.height; y++) {
            if(mask_path) {
                if(validate_mask_colors) {
                    if(mask_pixels.getPixel(x, y) == PackRGBA(255, 0, 0, 255)) {
                        //Red, masked
                        //pass
                    }
                    else if(mask_pixels.getPixel(x, y) == PackRGBA(255, 255, 255, 255)) {
                        //White, unmasked
                        continue;
                    }
                    else if(mask_pixels.getPixel(x, y) & 0xFF == 0) {
                        //Completely transparent, unmasked
                        continue;
                    }
                    else {
                        //Not red or white and also not completely transparent,
                        //so this is an invalid color.
                        let [r, g, b, a] = UnpackRGBA(mask_pixels.getPixel(x, y));
                        throw new DataError(`Invalid color ${hex(r,2)}${hex(g,2)}${hex(b,2)}${hex(a,2)} in mask ${mask_path}`);
                    }
                }
                else {
                    if(mask_pixels.getPixel(x, y) != PackRGBA(255, 0, 0, 255)) continue;
                }
            }

            let [r, g, b, a] = UnpackRGBA(pixels.getPixel(x, y));
            if(ignore_bright && r > 128 && g > 128 && b > 128 && a == 0xFF) {
                continue;
            }

            let [h, s, v] = rgb2hsv(r/255, g/255, b/255);
            h = Math.trunc(h*360);
            s = Math.trunc(s*100);
            v = Math.trunc(v*100);

            if(s == 0) {
                //Prevent issues when recoloring black/white/grey parts
                //of a texture where the base color is not black/white/grey.
                s = base_s;
            }

            let new_h = replacement_h;
            let new_s = s + s_change;
            let new_v = v + v_change;
            new_h = new_h % 360;
            new_s = Math.max(0, Math.min(100, new_s));
            new_v = Math.max(0, Math.min(100, new_v));
            [r, g, b] = hsv2rgb(new_h/360, new_s/100, new_v/100);
            r = Math.trunc(r*255);
            g = Math.trunc(g*255);
            b = Math.trunc(b*255);
            pixels.setPixel(x, y, [r, g, b, a]);
        }
    }
    return image;
}

export function hsv_shift_image(image, h_shift, v_shift) {
    let pixels = image;
    for(let x=0; x<image.width; x++) {
        for(let y=0; y<image.height; y++) {
            pixels.setPixel(x, y, hsv_shift_color(pixels.getPixel(x, y), h_shift, v_shift));
        }
    }
}

export function hsv_shift_palette(colors, h_shift, v_shift) {
    for(let [i, color] of Object.entries(colors)) {
        colors[i] = hsv_shift_color(color, h_shift, v_shift);
    }
    return colors;
}

export function hsv_shift_color(color, h_shift, v_shift) {
    let r, g, b, a;
    if(color.length == 4) [r, g, b, a] = color;
    else {
        [r, g, b] = color;
        a = null;
    }

    let [h, s, v] = rgb2hsv(r/255, g/255, b/255);
    h = (Math.trunc(h*360) + h_shift) % 360;
    s =  Math.trunc(s*100);
    v =  Math.trunc(v*100);

    let orig_v = v;
    v += v_shift;
    if(v <   0) v =   0;
    if(v > 100) v = 100;
    if(v <  30 && orig_v >= 30) v = 30;
    if(v >  90 && orig_v <= 90) v = 90;
    let v_diff = v - orig_v;

    //Instead of shifting saturation separately, we simply make it
    //relative to the value shift.
    //As value increases we want saturation to decrease and vice versa.
    //This is because bright colors look bad if they are too saturated,
    //and dark colors look bland if they aren't saturated enough.
    let orig_s = s;
    if(orig_s < 15 && v_shift > 0) {
        //For colors that were originally very unsaturated,
        //we want saturation to increase regardless of which
        //direction value is shifting in.
        if(orig_v < 30) {
            //Very dark, nearly black. Needs extra saturation for the
            //change to be noticeable.
            s += v_shift*2;
        }
        else {
            //Not that dark, probably grey or whitish.
            s += v_shift;
        }
    }
    else s -= v_diff;
    if(s <   0) s =   0;
    if(s > 100) s = 100
    if(s <   5 && orig_s >=  5) s =  5;
    if(s >  80 && orig_s <= 80) s = 80;

    [r, g, b] = hsv2rgb(h/360, s/100, v/100);
    r = Math.trunc(r*255);
    g = Math.trunc(g*255);
    b = Math.trunc(b*255);

    if(a == null) return [r, g, b];
    else return [r, g, b, a];
}
