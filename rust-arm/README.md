
Cross-compiling on Rust
=======================

Install adequate linker
-----------------------

Install `binutils` for target `arm-linux-gnueabi` and `arm-linux-gnueabihf`.

Note: `eabi` stands for Embedded ABI, and targets bare-metal ARM systems w/o
any operating system. `gnueabi` targets Linux system, and `hf` means system w/
hard(hardware) floating-point support.

Add target
----------

	$ rustup target add arm-unknown-linux-musleabi
	$ rustup target add arm-unknown-linux-musleabihf

Set cargo to use proper linker
------------------------------

Configure cargo to use proper linkers:

	[target.arm-unknown-linux-musleabi]
	linker = "arm-linux-gnueabi-ld"

	[target.arm-unknown-linux-musleabihf]
	linker = "arm-linux-gnueabihf-ld"

... or set environment variables:

	export CARGO_TARGET_ARM_UNKNOWN_LINUX_MUSLEABI_LINKER=arm-linux-gnueabi-ld
	export CARGO_TARGET_ARM_UNKNOWN_LINUX_MUSLEABIHF_LINKER=arm-linux-gnueabihf-ld


Build project
-------------

	cargo build --target arm-unknown-linux-musleabihf
	cargo build --target arm-unknown-linux-musleabihf --release

