import { decode_palettes } from "./palette.js";
import { decode_cmpr_block } from "./cmpr.js";
import { BLOCK_DATA_SIZES, BLOCK_WIDTHS, BLOCK_HEIGHTS, ImageFormat } from "./types.js";
import Image from "./Image.js";
import {
    convert_i4_to_color,     convert_i8_to_color,
    convert_ia4_to_color,    convert_ia8_to_color,
    convert_rgb565_to_color, convert_rgb5a3_to_color,
} from "./color.js";
import { DataError } from "../../app/errors.js";
import { assertType } from "../../Util.js";

export function decode_image(image_data, palette_data, image_format,
palette_format, num_colors, image_width, image_height, offset=0, canvas=null) {
    assertType(image_data, DataView);
    const colors = decode_palettes(palette_data, palette_format,
        num_colors, image_format);

    const block_width     = BLOCK_WIDTHS    [image_format];
    const block_height    = BLOCK_HEIGHTS   [image_format];
    const block_data_size = BLOCK_DATA_SIZES[image_format];

    const image = new Image(image_width, image_height, canvas);
    let block_x = 0;
    let block_y = 0;
    while(block_y < image_height) {
        let pixel_color_data = decode_block(image_format, image_data,
            offset, block_data_size, colors);

        for(let [i, color] of Object.entries(pixel_color_data)) {
            console.assert(color != undefined && color != null);
            const x = block_x+(i % block_width);
            const y = block_y+Math.trunc(i / block_width);
            if(x >= image_width || y >= image_height) continue;
            image.setPixel(x,y,color);
        }

        offset  += block_data_size;
        block_x += block_width;
        if(block_x >= image_width) {
            block_x = 0;
            block_y += block_height;
        }
    }
    return image;
}

function decode_i4_block(image_format, image_data, offset,
block_data_size, colors) {
    const pixel_color_data = [];
    for(let byte_index=0; byte_index<block_data_size; byte_index++) {
        const byte = image_data.getUint8(offset+byte_index);
        for(let nibble_index=0; nibble_index<2; nibble_index++) {
            pixel_color_data.push(convert_i4_to_color(
                (byte >> (1-nibble_index)*4) & 0xF));
        }
    }
    return pixel_color_data;
}

function decode_i8_block(image_format, image_data, offset,
block_data_size, colors) {
    const pixel_color_data = [];
    for(let i=0; i<block_data_size; i++) {
        pixel_color_data.push(convert_i8_to_color(
            image_data.getUint8(offset+i)));
    }
    return pixel_color_data;
}

function decode_ia4_block(image_format, image_data,
offset, block_data_size, colors) {
    const pixel_color_data = [];
    for(let i=0; i<block_data_size; i++) {
        pixel_color_data.push(convert_ia4_to_color(
            image_data.getUint8(offset+i)));
    }
    return pixel_color_data
}

function decode_ia8_block(image_format, image_data, offset,
block_data_size, colors) {
    const pixel_color_data = [];
    for(let i=0; i < Math.trunc(block_data_size/2); i++) {
        pixel_color_data.push(convert_ia8_to_color(
            image_data.getUint16(offset+i*2)));
    }
    return pixel_color_data;
}

function decode_rgb565_block(image_format, image_data, offset,
block_data_size, colors) {
    const pixel_color_data = [];
    for(let i=0; i<Math.trunc(block_data_size/2); i++) {
        pixel_color_data.push(convert_rgb565_to_color(
            image_data.getUint16(offset+i*2)));
    }
    return pixel_color_data;
}

function decode_rgb5a3_block(image_format, image_data, offset,
block_data_size, colors) {
    const pixel_color_data = [];
    for(let i=0; i<Math.trunc(block_data_size/2); i++) {
        pixel_color_data.push(convert_rgb5a3_to_color(
            image_data.getUint16(offset+i*2)));
    }
    return pixel_color_data;
}

function decode_rgba32_block(image_format, image_data, offset,
block_data_size, colors) {
    const pixel_color_data = [];
    for(let i=0; i<16; i++) {
        pixel_color_data.push([
            image_data.getUint8(offset+(i*2)),
            image_data.getUint8(offset+(i*2)+1),
            image_data.getUint8(offset+(i*2)+32),
            image_data.getUint8(offset+(i*2)+33),
        ]);
    }
    return pixel_color_data;
}

function decode_c4_block(image_format, image_data, offset,
block_data_size, colors) {
    const pixel_color_data = [];
        for(let byte_index=0; byte_index<block_data_size; byte_index++) {
        const byte = image_data.getUint8(offset+byte_index);
        for(let nibble_index=0; nibble_index<2; nibble_index++) {
            const color_index = (byte >> (1-nibble_index)*4) & 0xF;
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
    const pixel_color_data = [];
    for(let i=0; i<block_data_size; i++) {
        const color_index = image_data.getUint8(offset+i);
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
    const pixel_color_data = [];
    for(let i=0; i<Math.trunc(block_data_size/2); i++) {
        const color_index = image_data.getUint16(offset+i*2) & 0x3FFF;
        if(color_index >= colors.length) {
            //This block bleeds past the edge of the image
            color = null;
        }
        else color = colors[color_index];
        pixel_color_data.push(color);
    }
    return pixel_color_data;
}

const blockDecoders = {
    [ImageFormat.I4]:     decode_i4_block,
    [ImageFormat.I8]:     decode_i8_block,
    [ImageFormat.IA4]:    decode_ia4_block,
    [ImageFormat.IA8]:    decode_ia8_block,
    [ImageFormat.RGB565]: decode_rgb565_block,
    [ImageFormat.RGB5A3]: decode_rgb5a3_block,
    [ImageFormat.RGBA32]: decode_rgba32_block,
    [ImageFormat.C4]:     decode_c4_block,
    [ImageFormat.C8]:     decode_c8_block,
    [ImageFormat.C14X2]:  decode_c14x2_block,
    [ImageFormat.CMPR]:   decode_cmpr_block,
};
function decode_block(image_format, image_data, offset, block_data_size, colors) {
    const decoder = blockDecoders[image_format];
    if(decoder) return decoder(image_format, image_data, offset, block_data_size, colors);
    else throw new DataError("Unknown image format: " + image_format.name);
}
