#!/usr/bin/env python

for jong in range(0, 28):
	for jung in range(0, 21):
		chars = [
			chr(0xAC00 + (21*28) * cho + 28 * jung + jong) for cho in range(0, 19)
		]
		print("".join(chars))
	print()

