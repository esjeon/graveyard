#!/bin/sh

set -e

cd /sys/bus/pci/devices
for i in 0000\:26*; do
	if [ -f "${i}/driver/unbind" ]; then
		driver_name=$(basename $(readlink "${i}/driver"))
		if [ "$driver_name" = "vfio-pci" ]; then
			echo "[ Unbinding $i from vfio-pci ]"
			echo "$i" > "${i}/driver/unbind"

			echo "[ Probing driver for $i ]"
			echo "" > "${i}/driver_override"
			echo "${i}" > "/sys/bus/pci/drivers_probe"
		fi
	else
		echo "[ Probing driver for $i ]"
		echo "" > "${i}/driver_override"
		echo "${i}" > "/sys/bus/pci/drivers_probe"
	fi

	lspci -knn -s "${i}"
done

if [ ! -e "/sys/devices/platform/efi-framebuffer.0/driver" ]; then
	echo "[ Binding EFI framebuffer ]"
	echo "efi-framebuffer.0" > "/sys/bus/platform/drivers/efi-framebuffer/bind"
fi

target="/sys/class/vtconsole/vtcon1/bind"
if [ -f "$target" ]; then
	echo "[ Binding vtcon1 ]"
	echo 1 > "$target"
fi

echo "[ Starting display manager ]"
sleep 1
systemctl start sddm
