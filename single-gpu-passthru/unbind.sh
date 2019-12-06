#!/bin/sh

set -e

echo "[ Stopping display manager ]"
systemctl stop sddm
sleep 1

target="/sys/class/vtconsole/vtcon1/bind"
if [ -f "$target" ]; then
	echo "[ Unbinding vtcon1 ]"
	echo 0 > "$target"
fi

target="/sys/devices/platform/efi-framebuffer.0/driver/unbind"
if [ -f "$target" ]; then
	echo "[ Unbinding EFI framebuffer ]"
	echo efi-framebuffer.0 > "$target"
fi

modprobe vfio-pci

# TODO: device pattern
cd /sys/bus/pci/devices
for i in 0000\:26*; do
	if [ -f "${i}/driver/unbind" ]; then
		driver_name=$(basename $(readlink "${i}/driver"))
		if [ "$driver_name" = "vfio-pci" ]; then
			echo "[ Ignoring $i ... ]"
			continue
		else
			echo "[ Unbinding $i ]"
			echo "$i" > "${i}/driver/unbind"
		fi
	fi

	echo "[ Binding $i to vfio-pci ]"
	echo "vfio-pci" > "${i}/driver_override"
	echo "${i}" > "/sys/bus/pci/drivers_probe"

	lspci -knn -s "${i}"
done
