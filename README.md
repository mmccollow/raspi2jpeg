raspi2jpeg
==========

This is a small program to take a screenshot of *whatever* is displayed on the screen of a Raspberry Pi (i.e. not just X11) and save it to a small JPEG thumbnail.

It borrows heavily from [https://github.com/AndrewFromMelbourne/raspi2png](https://github.com/AndrewFromMelbourne/raspi2png). Previously I had been piping the output of that program to ImageMagick's `convert` CLI program to produce a small PNG thumbnail, but that takes about 3 seconds and produces a PNG file about 95k in size. This program runs in about 0.3 seconds and produces a file about 13k in size.

This program does not account for Pi screens that may be rotated, although the raspi2png program does, and the code could mostly be copy/pasted here if required. This program takes no command line arguments and will simply dump the screenshot.jpg file in the current working folder.

Building
--------

Run make in the project folder. MagickWand and it's -dev packages are required. On Raspbian, run:

`sudo apt-get install libmagickwand5 libmagickwand-dev`

