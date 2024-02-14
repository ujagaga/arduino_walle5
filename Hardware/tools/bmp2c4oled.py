#!/usr/bin/env python3

import sys
from PIL import Image
import os

FILE_PATH = None
WHITE = 1
BLACK = 0
img_width = 0
img = None
COLOR = False


def check_params():
    global FILE_PATH
    global BLACK
    global WHITE
    global COLOR
    global img

    if len(sys.argv) < 2:
        print("\nERROR: Missing input file path.")
        print(f"Usage: {os.path.realpath(__file__)} <img_path.bmp> [<invert_bits>]")
        return False
    else:
        FILE_PATH = sys.argv[1]

        if not os.path.isfile(FILE_PATH):
            print(f"\nERROR: File not found: {FILE_PATH}")
            return False

        img = Image.open(FILE_PATH)
        img_bands_count = len(img.getbands())

        if not FILE_PATH.endswith(".bmp") or img_bands_count != 1:
            print('ERROR: Only ".bmp" file supported.')
            return False

    img_width, image_height = img.size
    pix_value_max = 0
    for y in range(0, image_height):
        for x in range(0, img_width):
            pix_val = img.getpixel((x, y))
            if pix_value_max < pix_val:
                pix_value_max = pix_val

    if pix_value_max > 1:
        # Color. White is 215.
        COLOR = True

    if len(sys.argv) > 2:
        # INVERT_COLOR
        BLACK = 0
        WHITE = 1
    else:
        BLACK = 1
        WHITE = 0

    return True


def extract_pixels():
    global img_width

    img_width, image_height = img.size
    row_count = image_height // 8

    if COLOR:
        print("WARNING: Detected a coloured image. Will consider all non white pixels as black!")
    else:
        print("Detected a monochromatic image.")
    print("Extracting pixels from bitmap. Assuming 8 vertical pixels make a byte for OLED display.")
    print(f"Width: {img_width}, Height: {image_height}, 8-bit high rows: {row_count}\n")

    pixels = []

    for row in range(0, row_count):
        for x in range(0, img_width):
            pixData = 0
            for row_y in range(0, 8):
                y = row * 8 + row_y
                pixVal = img.getpixel((x, y))

                if COLOR:
                    if pixVal > 214:
                        pixBit = WHITE
                    else:
                        pixBit = BLACK
                else:
                    if pixVal > 0:
                        pixBit = BLACK
                    else:
                        pixBit = WHITE

                if pixBit:
                    pixData += 1 << row_y

            pixels.append(pixData)

    return pixels


def format_out(pixels):
    print("C array to store graphic:\n")
    print("static const char my_graphic[] PROGMEM = {\n  ", end="")
    data_start = None
    data_end = None

    for i in range(0, len(pixels)):
        if pixels[i] > 0:
            if data_start is None:
                data_start = i
            data_end = i+1

    # Snap to row
    data_start_page = data_start // img_width
    data_start = data_start_page * img_width
    data_end_page = data_end // img_width
    data_end = (data_end_page + 1) * img_width

    pix_count = 0
    page_count = 1
    for i in range(0, len(pixels)):
        if page_count > img_width:
            page_count = 1

            if 0 < data_start == i:
                print(f"/* ----- White space before row {data_start_page} ----- */\n  ", end="")
            elif i == data_end:
                print(f"/* ----- White space after row {data_end_page} ----- */\n  ", end="")
            else:
                print("/* -------------------- */\n  ", end="")

        pix_count += 1
        page_count += 1
        if pix_count > 7:
            print(f"{hex(pixels[i])},\n  ", end="")
            pix_count = 0
        else:
            print(f"{hex(pixels[i])},", end=" ")
    print("}")
    print("\n\nTo extract data use:")
    print("  byte_val = pgm_read_byte(my_graphic[idx]);\n")


if check_params():
    data = extract_pixels()
    format_out(data)
