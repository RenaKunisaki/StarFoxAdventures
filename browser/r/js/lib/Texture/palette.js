import { IMAGE_FORMATS_THAT_USE_PALETTES, PaletteFormat, MAX_COLORS_FOR_IMAGE_FORMAT } from "./types.js";
import { read_u16, write_u16 } from "./bits.js";
import BytesIO from "./BytesIO.js";
import { DataError } from "../../app/errors.js";

export function decode_palettes(palette_data, palette_format, num_colors, image_format) {
    //if not isinstance(image_format, ImageFormat):
    //  raise Exception("Invalid image format: %s" % image_format)
    if(IMAGE_FORMATS_THAT_USE_PALETTES[image_format] == undefined) {
        return [];
    }

    let colors = [];
    let offset = 0;
    for(let i=0; i<num_colors; i++) {
        let raw_color = read_u16(palette_data, offset);
        let color = decode_color(raw_color, palette_format);
        colors.push(color);
        offset += 2;
    }
    return colors;
}

export function decode_color(raw_color, palette_format) {
    let color;
    if(palette_format == PaletteFormat.IA8) {
        color = convert_ia8_to_color(raw_color);
    }
    else if(palette_format == PaletteFormat.RGB565) {
        color = convert_rgb565_to_color(raw_color);
    }
    else if(palette_format == PaletteFormat.RGB5A3) {
        color = convert_rgb5a3_to_color(raw_color);
    }
    return color;
}

export function generate_new_palettes_from_image(image, image_format, palette_format) {
    if(IMAGE_FORMATS_THAT_USE_PALETTES[image_format] == undefined) {
        return [[],{}];
    }

    let pixels = image;
    let [width, height] = image.size;
    let encoded_colors = [];
    let colors_to_color_indexes = {};
    for(let y=0; y<height; y++) {
        for(let x=0; x<width; x++) {
            let color = pixels.getPixel(x,y);
            let encoded_color = encode_color(color, palette_format);
            if(encoded_colors[encoded_color] == undefined) {
                encoded_colors.push(encoded_color);
            }
            if(colors_to_color_indexes[color] == undefined) {
                colors_to_color_indexes[color] = encoded_colors.indexOf(encoded_color);
            }
        }
    }

    if(encoded_colors.length > MAX_COLORS_FOR_IMAGE_FORMAT[image_format]) {
        //If the image has more colors than the selected image format can support,
        //we automatically reduce the number of colors.
        let limited_palette = create_limited_palette_from_image(image, MAX_COLORS_FOR_IMAGE_FORMAT[image_format]);

        let encoded_colors = [];
        let colors_to_color_indexes = {};
        for(let y=0; y<height; y++) {
            for(let x=0; x<width; x++) {
                let color = pixels.getPixel(x,y);
                let new_color = get_nearest_color_fast(color, limited_palette);
                let encoded_color = encode_color(new_color, palette_format);
                if(encoded_colors[encoded_color] == undefined) {
                    encoded_colors.push(encoded_color);
                }
                if(colors_to_color_indexes[color] == undefined) {
                    colors_to_color_indexes[color] = encoded_colors.indexOf(encoded_color);
                }
            }
        }
    }
    return [encoded_colors, colors_to_color_indexes];
}

export function generate_new_palettes_from_colors(colors, palette_format) {
    let encoded_colors = [];
    for(let color of colors) {
        let encoded_color = encode_color(color, palette_format);
        encoded_colors.push(encoded_color);
    }
    return encoded_colors;
}

export function encode_color(color, palette_format) {
    let raw_color;
    if(palette_format == PaletteFormat.IA8) {
        raw_color = convert_color_to_ia8(color);
    }
    else if(palette_format == PaletteFormat.RGB565) {
        raw_color = convert_color_to_rgb565(color);
    }
    else if(palette_format == PaletteFormat.RGB5A3) {
        raw_color = convert_color_to_rgb5a3(color);
    }
    return raw_color;
}

export function encode_palette(encoded_colors, palette_format, image_format) {
    if(IMAGE_FORMATS_THAT_USE_PALETTES[image_format] == undefined) {
        return new BytesIO();
    }

    if(encoded_colors.length > MAX_COLORS_FOR_IMAGE_FORMAT[image_format]) {
        throw new DataError(
            `Maximum number of colors supported by image format ${image_format.name} is ${MAX_COLORS_FOR_IMAGE_FORMAT[image_format]}, but replacement image has ${encoded_colors.length} colors`
        );
    }

    let offset = 0;
    let new_palette_data = new BytesIO();
    for(let raw_color of encoded_colors) {
        write_u16(new_palette_data, offset, raw_color);
        offset += 2;
    }
    return new_palette_data;
}
