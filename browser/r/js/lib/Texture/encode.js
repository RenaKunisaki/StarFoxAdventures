function encode_image_from_path(new_image_file_path, image_format, palette_format, mipmap_count=1) {
    let image = Image.open(new_image_file_path); //XXX
    let [image_width, image_height] = image.size;
    let [new_image_data, new_palette_data, encoded_colors] = encode_image(image, image_format, palette_format, mipmap_count=mipmap_count);
    return [new_image_data, new_palette_data, encoded_colors, image_width, image_height];
}

function encode_image(image, image_format, palette_format, mipmap_count=1) {
    image = image.convert("RGBA"); //XXX
    let [image_width, image_height] = image.size;

    if(mipmap_count < 1) mipmap_count = 1;

    let [encoded_colors, colors_to_color_indexes] = generate_new_palettes_from_image(image, image_format, palette_format);

    let block_width = BLOCK_WIDTHS[image_format];
    let block_height = BLOCK_HEIGHTS[image_format];
    let block_data_size = BLOCK_DATA_SIZES[image_format];

    let new_image_data = BytesIO(); //XXX
    let mipmap_image = image;
    let mipmap_width = image_width;
    let mipmap_height = image_height;
    for(let i=0; i<mipmap_count; i++) {
        if(i != 0) {
            mipmap_width = Math.floor(mipmap_width / 2);
            mipmap_height = Math.floor(mipmap_height / 2);
            mipmap_image = image.resize((mipmap_width, mipmap_height), Image.NEAREST);
        }

        let mipmap_image_data = encode_mipmap_image(
            mipmap_image, image_format,
            colors_to_color_indexes,
            block_width, block_height,
            mipmap_width, mipmap_height
        );

        mipmap_image_data.seek(0);
        new_image_data.write(mipmap_image_data.read());
    }

    let new_palette_data = encode_palette(encoded_colors, palette_format, image_format);
    return [new_image_data, new_palette_data, encoded_colors];
}

function encode_mipmap_image(image, image_format, colors_to_color_indexes, block_width, block_height, image_width, image_height) {
    let pixels = image.load(); //XXX
    let offset_in_image_data = 0;
    let block_x = 0;
    let block_y = 0;
    let mipmap_image_data = BytesIO(); //XXX
    while(block_y < image_height) {
        let block_data = encode_image_to_block(
            image_format,
            pixels, colors_to_color_indexes,
            block_x, block_y, block_width, block_height, image_width, image_height
        );
        console.assert(block_data.length == BLOCK_DATA_SIZES[image_format]);
        write_bytes(mipmap_image_data, offset_in_image_data, block_data);

        offset_in_image_data += BLOCK_DATA_SIZES[image_format];
        block_x += BLOCK_WIDTHS[image_format];
        if(block_x >= image_width) {
            block_x = 0;
            block_y += BLOCK_HEIGHTS[image_format];
        }
    }
    return mipmap_image_data;
}

function encode_image_to_i4_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let offset = 0;

    let color_1, color_1_i4, color_2, color_2_i4;
    for(let y=block_y; y<block_y+block_height; y++) {
        for(let x=block_x; x<block_x+block_width; x += 2) {
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                color_1_i4 = 0xF;
            }
            else {
                color_1 = pixels[x,y];
                color_1_i4 = convert_color_to_i4(color_1);
                console.assert(0 <= color_1_i4 <= 0xF);
            }

            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                color_2_i4 = 0xF;
            }
            else {
                color_2 = pixels[x+1,y];
                color_2_i4 = convert_color_to_i4(color_2);
                console.assert(0 <= color_2_i4 <= 0xF);
            }
            let byte = ((color_1_i4 & 0xF) << 4) | (color_2_i4 & 0xF);
            write_u8(new_data, offset, byte);
            offset += 1;
        }
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_i8_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let offset = 0;

    let color, i8;
    for(let y=block_y; y<block_y+block_height; y++) {
        for(let x=block_x; x<block_x+block_width; x++) {
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                i8 = 0xFF;
            }
            else {
                color = pixels[x,y];
                i8 = convert_color_to_i8(color);
                console.assert(0 <= i8 <= 0xFF);
            }
            write_u8(new_data, offset, i8);
            offset += 1;
        }
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_ia4_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let offset = 0;

    let ia4, color;
    for(let y=block_y; y<block_y+block_height; y++) {
        for(let x=block_x; x<block_x+block_width; x++) {
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                ia4 = 0xFF;
            }
            else {
                color = pixels[x,y];
                ia4 = convert_color_to_ia4(color);
                console.assert(0 <= ia4 <= 0xFF);
            }
            write_u8(new_data, offset, ia4);
            offset += 1;
        }
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_ia8_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let offset = 0;

    let ia8, color;
    for(let y=block_y; y<block_y+block_height; y++) {
        for(let x=block_x; x<block_x+block_width; x++) {
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                ia8 = 0xFF;
            }
            else {
                color = pixels[x,y];
                ia8 = convert_color_to_ia8(color);
                console.assert(0 <= ia8 <= 0xFFFF);
            }
            write_u16(new_data, offset, ia8);
            offset += 2;
        }
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_rgb563_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let offset = 0;
    let color, rgb565;
    for(let y=block_y; y<block_y+block_height; y++) {
        for(let x=block_x; x<block_x+block_width; x++) {
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                rgb565 = 0xFFFF;
            }
            else {
                color = pixels[x,y];
                rgb565 = convert_color_to_rgb565(color);
            }
            write_u16(new_data, offset, rgb565);
            offset += 2;
        }
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_rgb5a3_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let offset = 0;
    let color, rgb5a3;
    for(let y=block_y; y<block_y+block_height; y++) {
        for(let x=block_x; x<block_x+block_width; x++) {
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                rgb5a3 = 0xFFFF;
            }
            else {
                color = pixels[x,y];
                rgb5a3 = convert_color_to_rgb5a3(color);
            }
            write_u16(new_data, offset, rgb5a3);
            offset += 2;
        }
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_rgba32_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let r, g, b, a, color;
    for(let i=0; i<16; i++) {
        let x = block_x + (i % block_width);
        let y = block_y + Math.floor(i / block_width);
        if(x >= image_width || y >= image_height) {
            //This block bleeds past the edge of the image
            r = g = b = a = 0xFF;
        }
        else {
            color = pixels[x, y];
            [r, g, b, a] = color;
        }
        write_u8(new_data, (i*2),    a);
        write_u8(new_data, (i*2)+ 1, r);
        write_u8(new_data, (i*2)+32, g);
        write_u8(new_data, (i*2)+33, b);
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_c4_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let offset = 0;

    let color_1, color_1_index, color_2, color_2_index;
    for(let y=block_y; y<block_y+block_height; y++) {
        for(let x=block_x; x<block_x+block_width; x += 2) {
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                color_1_index = 0xF;
            }
            else {
                color_1 = pixels[x,y];
                color_1_index = colors_to_color_indexes[color_1];
                console.assert(0 <= color_1_index <= 0xF);
            }
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                color_2_index = 0xF;
            }
            else {
                color_2 = pixels[x+1,y];
                color_2_index = colors_to_color_indexes[color_2];
                console.assert(0 <= color_2_index <= 0xF);
            }
            let byte = ((color_1_index & 0xF) << 4) | (color_2_index & 0xF);
            write_u8(new_data, offset, byte);
            offset += 1;
        }
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_c8_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let offset = 0;

    let color_index, color;
    for(let y=block_y; y<block_y+block_height; y++) {
        for(let x=block_x; x<block_x+block_width; x++) {
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                color_index = 0xFF;
            }
            else {
                color = pixels[x,y];
                color_index = colors_to_color_indexes[color];
            }
            write_u8(new_data, offset, color_index);
            offset += 1;
        }
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_c14x2_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    let new_data = BytesIO(); //XXX
    let offset = 0;

    let color_index, color;
        for(let y=block_y; y<block_y+block_height; y++) {
        for(let x=block_x; x<block_x+block_width; x++) {
            if(x >= image_width || y >= image_height) {
                //This block bleeds past the edge of the image
                color_index = 0x3FFF;
            }
            else {
                color = pixels[x,y];
                color_index = colors_to_color_indexes[color];
            }
            write_u16(new_data, offset, color_index);
            offset += 2;
        }
    }
    new_data.seek(0);
    return new_data.read();
}

function encode_image_to_block(image_format, pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height) {
    switch(image_format) {
    case ImageFormat.I4:
    return encode_image_to_i4_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.I8:
    return encode_image_to_i8_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.IA4:
    return encode_image_to_ia4_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.IA8:
    return encode_image_to_ia8_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.RGB565:
    return encode_image_to_rgb563_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.RGB5A3:
    return encode_image_to_rgb5a3_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.RGBA32:
    return encode_image_to_rgba32_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.C4:
    return encode_image_to_c4_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.C8:
    return encode_image_to_c8_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.C14X2:
    return encode_image_to_c14x2_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    case ImageFormat.CMPR:
    return encode_image_to_cmpr_block(pixels, colors_to_color_indexes, block_x, block_y, block_width, block_height, image_width, image_height);
    default:
    throw new Error("Unknown image format: " + ImageFormat(image_format).name);
    }
}
