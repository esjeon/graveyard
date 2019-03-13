#!/usr/bin/env python

import os

def conv_jpeg(infile, outfile, quality=0.90):
    image = pdb.file_heif_load(infile, infile)
    pdb.file_jpeg_save(image, image.layers[0], outfile, outfile, quality, 0, 1, 0, "", 2, 0, 0, 0)
    pdb.gimp_image_delete(image)

for root, dirs, files in os.walk("."):
    for file in files:
        if file.lower().endswith(".heic"):
            path = os.path.join(root, file)
            print(path)
            conv_jpeg(path, path + ".jpg")

