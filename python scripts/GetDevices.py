import sys
import soco

SoCo = False
deviceList = []
for zone in soco.discover():
    deviceList.append(zone.player_name + "|" + zone.ip_address)

print ";".join(deviceList)