
Cross-compiling on Rust
=======================

Install adequate linker
-----------------------

installed `arm-linux-gnueabi-binutils`

Add target
----------

	$ rustup target add armv7-unknown-linux-musleabihf
	$ rustup target add arm-unknown-linux-musleabi

Set cargo to use proper linker
------------------------------

Configure cargo to use proper linkers:

	[target.arm-unknown-linux-musleabi]
	linker = "arm-linux-gnueabi-ld"

	[target.arm-unknown-linux-musleabihf]
	linker = "arm-linux-gnueabihf-ld"

... or set environment variables:

	export CARGO_TARGET_ARMV7_UNKNOWN_LINUX_MUSLEABIHF_LINKER=arm-linux-gnueabihf-gcc
	export CARGO_TARGET_ARM_UNKNOWN_LINUX_MUSLEABI_LINKER=arm-linux-gnueabi-ld


Build project
-------------

	cargo build --target arm-unknown-linux-musleabihf --release

