## bmp2c4oled.py ##

To simplify drawing custom graphics to my OLED display (SSD1306 or SH1106) I created this script "bmp2c4oled.py".
It supports either monochromatic or coloured ".bmp" files. My display is 128x64 pixels large with 8 pixel rows grouped 
into pages. To write an 0xFF into graphic memory location is to draw a vertical line 8 pixel long.
The script should support any OLED size as long as it has 8-bit wide pages.

# How to use #
1. Prepare a ".bmp" image 128x64 pixels large (or as large as is your display). It is best to use a monochromatic 
   format, so you know exactly which pixels will light up, but you can use a coloured image, just be aware that any
   non-white pixel will be considered black. I assume that the black areas are the ones to be shown on the OLED screen.
   If this is not so, just use the inverse flag for the script.
2. Install dependencies:

       pip install pillow

3. Run the script:

       ./bmp2c4oled.py <path/to/my/image.bmp> [<inverse>]

4. Copy the console output to your C file. I use Arduino IDE, so the command suggestion is formatted the way I used it.

