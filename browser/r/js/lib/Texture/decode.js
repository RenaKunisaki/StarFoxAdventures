import { read_u8, read_u16 } from "./bits.js";
import { decode_palettes } from "./palette.js";
import { decode_cmpr_block } from "./cmpr.js";
import { BLOCK_DATA_SIZES, BLOCK_WIDTHS, BLOCK_HEIGHTS, ImageFormat } from "./types.js";
import Image from "./Image.js";
import {
    convert_i4_to_color, convert_i8_to_color,
    convert_ia4_to_color, convert_ia8_to_color,
    convert_rgb565_to_color, convert_rgb5a3_to_color,
} from "./color.js";

export function decode_image(image_data, palette_data, image_format, palette_format, num_colors, image_width, image_height) {
    let colors = decode_palettes(palette_data, palette_format, num_colors, image_format);

    let block_width = BLOCK_WIDTHS[image_format];
    let block_height = BLOCK_HEIGHTS[image_format];
    let block_data_size = BLOCK_DATA_SIZES[image_format];

    let image   = new Image(image_width, image_height);
    let pixels  = image;
    let offset  = 0;
    let block_x = 0;
    let block_y = 0;
    while(block_y < image_height) {
        let pixel_color_data = decode_block(image_format, image_data, offset, block_data_size, colors);

        for(let [i, color] of Object.entries(pixel_color_data)) {
            console.assert(color != undefined && color != null);
            let x_in_block = i % block_width;
            let y_in_block = Math.trunc(i / block_width);
            let x = block_x+x_in_block;
            let y = block_y+y_in_block;
            if(x >= image_width || y >= image_height) continue;
            pixels.setPixel(x,y,color);
        }

        offset += block_data_size;
        block_x += block_width;
        if(block_x >= image_width) {
            block_x = 0;
            block_y += block_height;
        }
    }
    return image;
}

function decode_i4_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let byte_index=0; byte_index<block_data_size; byte_index++) {
        let byte = read_u8(image_data, offset+byte_index);
        for(let nibble_index=0; nibble_index<2; nibble_index++) {
            let i4 = (byte >> (1-nibble_index)*4) & 0xF;
            let color = convert_i4_to_color(i4);
            pixel_color_data.push(color);
        }
    }
    return pixel_color_data;
}

function decode_i8_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let i=0; i<block_data_size; i++) {
        let i8 = read_u8(image_data, offset+i);
        let color = convert_i8_to_color(i8);
        pixel_color_data.push(color);
    }
    return pixel_color_data;
}

function decode_ia4_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let i=0; i<block_data_size; i++) {
        let ia4 = read_u8(image_data, offset+i);
        let color = convert_ia4_to_color(ia4);
        pixel_color_data.push(color);
    }
    return pixel_color_data
}

function decode_ia8_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let i=0; i < Math.trunc(block_data_size/2); i++) {
        let ia8 = read_u16(image_data, offset+i*2);
        let color = convert_ia8_to_color(ia8);
        pixel_color_data.push(color);
    }
    return pixel_color_data;
}

function decode_rgb565_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let i=0; i<Math.trunc(block_data_size/2); i++) {
        let rgb565 = read_u16(image_data, offset+i*2);
        let color = convert_rgb565_to_color(rgb565);
        pixel_color_data.push(color);
    }
    return pixel_color_data;
}

function decode_rgb5a3_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let i=0; i<Math.trunc(block_data_size/2); i++) {
        let rgb5a3 = read_u16(image_data, offset+i*2);
        let color = convert_rgb5a3_to_color(rgb5a3);
        pixel_color_data.push(color);
    }
    return pixel_color_data;
}

function decode_rgba32_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let i=0; i<16; i++) {
        let a = read_u8(image_data, offset+(i*2));
        let r = read_u8(image_data, offset+(i*2)+1);
        let g = read_u8(image_data, offset+(i*2)+32);
        let b = read_u8(image_data, offset+(i*2)+33);
        let color = [r, g, b, a];
        pixel_color_data.push(color);
    }
    return pixel_color_data;
}

function decode_c4_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
        for(let byte_index=0; byte_index<block_data_size; byte_index++) {
        let byte = read_u8(image_data, offset+byte_index);
        for(let nibble_index=0; nibble_index<2; nibble_index++) {
            let color_index = (byte >> (1-nibble_index)*4) & 0xF;
            let color;
            if(color_index >= colors.length) {
                //This block bleeds past the edge of the image
                color = null;
            }
            else color = colors[color_index];
            pixel_color_data.push(color);
        }
    }
    return pixel_color_data;
}

function decode_c8_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let i=0; i<block_data_size; i++) {
        let color_index = read_u8(image_data, offset+i);
        if(color_index >= colors.length) {
            //This block bleeds past the edge of the image
            color = null;
        }
        else color = colors[color_index];
        pixel_color_data.push(color);
    }
    return pixel_color_data;
}

function decode_c14x2_block(image_format, image_data, offset, block_data_size, colors) {
    let pixel_color_data = [];
    for(let i=0; i<Math.trunc(block_data_size/2); i++) {
        let color_index = read_u16(image_data, offset+i*2) & 0x3FFF;
        if(color_index >= colors.length) {
            //This block bleeds past the edge of the image
            color = null;
        }
        else color = colors[color_index];
        pixel_color_data.push(color);
    }
    return pixel_color_data;
}

function decode_block(image_format, image_data, offset, block_data_size, colors) {
    switch(image_format) {
        case ImageFormat.I4:
            return decode_i4_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.I8:
            return decode_i8_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.IA4:
            return decode_ia4_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.IA8:
            return decode_ia8_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.RGB565:
            return decode_rgb565_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.RGB5A3:
            return decode_rgb5a3_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.RGBA32:
            return decode_rgba32_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.C4:
            return decode_c4_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.C8:
            return decode_c8_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.C14X2:
            return decode_c14x2_block(image_format, image_data, offset, block_data_size, colors);
        case ImageFormat.CMPR:
            return decode_cmpr_block(image_format, image_data, offset, block_data_size, colors);
        default:
            throw new Error("Unknown image format: " + image_format.name);
    }
}
