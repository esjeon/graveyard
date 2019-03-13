#!/bin/sh
# 

# Example Script:
#
# script="
# import sys
# 
# def setOpacity(fname, level):
#     img = pdb.gimp_file_load(fname, fname)
#     img.layers[0].opacity = float(level)
#     img.merge_visible_layers(NORMAL_MODE)
#     pdb.file_png_save(img, img.layers[0], fname + '.out.png', fname + '.png', 0, 9, 1, 0, 0, 1, 1)
#     pdb.gimp_image_delete(img)
# 
# setOpacity('input.png', 50)
# "

usage() {
	cat >&2 <<- EOF
		Usage: $(basename $0) python-script

		       Executes the given script inside GIMP pythonfu console.
		       For scripting for GIMP, check PythonFu documentation. 
	EOF
}

if [ "$#" -ne 1 -o ! -f "$1" ]; then
	usage
	exit
fi

script="$(cat "$1")"

gimp-console \
	--new-instance --no-data --no-fonts \
	--batch-interpreter python-fu-eval \
	-b "${script}" -b "pdb.gimp_quit(1)"
