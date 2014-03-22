#!/usr/bin/env python3
# usbwatch.py - monitor addition/removal of USB devices
#
#

import pyudev

class UsbDevice:
  @staticmethod
  def fromUdevDevice(udev):
    attr = lambda name: udev.attributes.asstring(name)
    try:
      try:
        manufacturer = attr('manufacturer')
      except KeyError:
        manufacturer = None
      return UsbDevice( udev.device_path,
          ( attr('busnum') + '-' + attr('devpath'),
            attr('idVendor') + ":" + attr('idProduct'),
            manufacturer,
            attr('product')
          )
        )
    except KeyError:
      return None

  def __init__(self, devicePath, info):
    self.path = devicePath
    self.bus, self.id, self.manufacturer, self.product = info

  def __repr__(self):
    return "UsbDevice(%s, %s)" % (self.path, (self.bus, self.id, self.manufacturer, self.product))

  def __str__(self):
    return "%s (%s): %s, %s" % (self.id, self.bus, self.manufacturer, self.product)


# UsbWatcher monitors the connection status of USB devices.
# It remembers the devices which are "known" to be connected to the system.
class UsbWatcher:
  def __init__(self):
    self.ctx = pyudev.Context()
    self.mon = pyudev.Monitor.from_netlink(self.ctx)
    self.mon.filter_by('usb')
    self.knowns = dict()

  # Query the currently connected USB devices
  # Forcefully updates the list of "known" devices
  def poll(self):
    old_knowns = self.knowns
    self.knowns = dict()

    for udev in self.ctx.list_devices(subsystem="usb"):
      dev = UsbDevice.fromUdevDevice(udev)
      if dev is not None:
          self.knowns[udev.device_path] = dev
          if udev.device_path in old_knowns:
            old_knowns.pop(udev.device_path)
          else:
            self.onAdd(dev)

    for path, dev in old_knowns.items():
      self.onRemove(dev)

  # Monitor newly added devices. Any devices connected beforehand are ignored
  def watch(self):
    for action, udev in iter(self.mon):
      if action == 'add':
        dev = UsbDevice.fromUdevDevice(udev)
        if dev is not None:
          self.knowns[udev.device_path] = dev
          self.onAdd(dev)
      elif action == 'remove':
        if udev.device_path in self.knowns:
          dev = self.knowns.pop(udev.device_path)
          self.onRemove(dev)

  # Called upon a device is added to the system
  # Override this
  def onAdd(self,dev):
    print("add %s " % str(dev))

  # Called upon a device is removed from the system
  # Override this
  def onRemove(self,dev):
    print("remove %s " % str(dev))

if __name__ == "__main__":
  try:
    wat = UsbWatcher()
    wat.poll()
    wat.watch()
  except KeyboardInterrupt:
    pass
