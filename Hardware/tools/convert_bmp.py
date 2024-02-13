#!/usr/bin/env python3

import sys
from PIL import Image
import os

FILE_PATH = None
INVERT_COLOR = False

def check_params():
    global FILE_PATH
    global INVERT_COLOR

    if len(sys.argv) < 2:
        print("Usage: ", os.path.realpath(__file__)), "<monochrome_img_path.bmp> [<invert_bits>]")

    if len(sys.argv) > 1:
        FILE_PATH = sys.argv[1]

    if len(sys.argv) > 2:
        INVERT_COLOR = True


if INVERT_COLOR:
    BLACK = 1
    WHITE = 0
else:
    WHITE = 1
    BLACK = 0

img = Image.open(FILE_PATH)
imageSizeW, imageSizeH = img.size
row_count = imageSizeH // 8

print("W:", imageSizeW, " H:", imageSizeH, " ROWS:", row_count)

pixels = []

for row in range(0, row_count):
    for x in range(0, imageSizeW):
        pixData = 0
        for row_y in range(0, 8):
            y = row * 8 + row_y
            pixVal = img.getpixel((x, y))
            if pixVal > 0:
                pixBit = WHITE
            else:
                pixBit = BLACK
            if pixBit:
                pixData += 1 << row_y

        pixels.append(pixData)

print(pixels)
print(len(pixels))

