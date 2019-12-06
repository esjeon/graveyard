#!/usr/bin/env python3
#
# This script is kept here for a record. It tries to generate new "foreground"
# domain from an existing domain.
#
# This is abandonned because of the unreliability of "converting" domain.
# Libvirt XML interface is too low-level for this kind of operation.
#


import libvirt
import lxml.etree as ET

XMLNS_URI = "uuid://be034d06-4bbc-4340-b3e2-870cbd463e0c"
XMLNS = "{" + XMLNS_URI + "}"

def x_dig(tree, name):
    elem = tree.find(name)
    return elem if elem is not None else ET.SubElement(tree, name)

def x_dig_s(tree, names):
    for name in names:
        tree = x_dig(tree, name)
    return tree

def x_remove_all(tree, name):
    for child in tree.findall(name):
        tree.remove(child)

def x_remove_s(tree, names):
    parent = None
    for name in names:
        parent = tree
        tree = parent.find(name)
        if tree is None:
            break

    if parent is not None:
        parent.remove(tree)

def x_attr(tree, attrs):
    for key, val in attrs.items():
        tree.set(key, val)


def verify_domain_foregroundable(root):
    # check if already foreground
    metadata = root.find("metadata")
    if metadata is not None:
        foreground = metadata.find(XMLNS + "foreground")
        if foreground is not None:
            raise Exception("Already foreground")

    # verify efi
    try:
        loader = root.find("os").find("loader")
    except(e):
        raise e

    if loader is None or loader.get("type") != "pflash":
        raise Exception("Unsupported machine")


def add_hostpci(root, src, dest, multifunction=False, rom=None):
    devices = root.find("devices")

    #<hostdev mode='subsystem' type='pci' managed='yes'> 
    hostdev = ET.SubElement(devices, "hostdev")
    x_attr(hostdev, {
        "mode": "subsystem",
        "type": "pci",
        "managed": "yes",
    })

    src_addr = ET.SubElement(ET.SubElement(hostdev, "source"), "address")
    x_attr(src_addr, {
        "domain": src[0],
        "bus": src[1],
        "slot": src[2],
        "function": src[3],
    })

    if multifunction is True:
        src_addr.set("multifunction", "on")

    x_attr(ET.SubElement(hostdev, "address"), {
        "type": "pci",
        "domain": dest[0],
        "bus": dest[1],
        "slot": dest[2],
        "function": dest[3],
    })

    if rom is not None:
        ET.SubElement(hostdev, "rom").set("file", rom)

def convert_domain_foreground(root):
    verify_domain_foregroundable(root)

    # register namespaces
    #root.set("xmlns:" + XMLNS_NAME, XMLNS_VALUE)
    root.nsmap[XMLNS] = XMLNS_URI

    # rename VM
    name = root.find("name")
    name.text += "_foreground" #TODO: special signature?

    # add metadata "foreground"
    src_addr = ET.SubElement(x_dig(root, "metadata"), XMLNS + "foreground")

    # remove UUID to avoid ID collision
    x_remove_all(root, "uuid")

    # KVM Signature hiding (a workaround for NoVideo thingy)
    # <vendor_id state='on' value='whatever'/>
    vendor_id = x_dig_s(root, ["features", "hyperv", "vendor_id"])
    vendor_id.set("state", "on")
    vendor_id.set("value", "whatever") # TODO: a proper value?
    # <kvm><hidden state='on'/></kvm>
    kvm_hidden = x_dig_s(root, ["features", "kvm", "hidden"])
    kvm_hidden.set("state", "on")

    devices = x_dig(root, "devices")

    # extract video PCI info
    # e.g. <address type='pci' domain='0x0000' bus='0x00' slot='0x02' function='0x0'/>
    video_pci_addr = None
    for video in devices.iterfind("video"):
        addr = video.find("address")
        if addr is not None and addr.get("type") == "pci":
            video_pci_addr = (addr.get("domain"), addr.get("bus"), addr.get("slot"), addr.get("function"))
            break

    # remove virtual graphics and related devices
    x_remove_all(devices, "redirdev");
    x_remove_all(devices, "graphics");
    x_remove_all(devices, "video");
    for inputElm in devices.findall("input"):
        if inputElm.get("type") not in ["mouse", "keyboard"]:
            devices.remove(inputElm)

    # attach pci card
    # TODO: parameterize
    num_function = 4
    src_pci_slot = "0x26"
    for i in range(num_function):
        func = "0x" + str(i)
        src = ("0x0000", "0x00", src_pci_slot, func)
        dest = video_pci_addr[0:3] + (func,)
        if i == 0:
            add_hostpci(root, src, dest, multifunction=True, rom="/tmp/nvidia.rom")
        else:
            add_hostpci(root, src, dest)
    
    #return ET.tostring(root, encoding="unicode")
    return root


if __name__ == "__main__":
    conn = libvirt.open("qemu:///system")
    domain = conn.lookupByName("archlinux")
    domxml = convert_domain_foreground(
        ET.fromstring(domain.XMLDesc(), ET.XMLParser(remove_blank_text=True)))
    print(ET.tostring(domxml, encoding="unicode", pretty_print=True))
    conn.close()

