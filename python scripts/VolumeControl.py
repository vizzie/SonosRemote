import sys
import soco

# one argument for volume button pressed (buttons 6 and 7)
myArgs = sys.argv
setVolume = myArgs[1]
#print 'Input args: {0}', format(myArgs)

SoCo = False

# SoCo = soco.SoCo("10.0.1.29")
for zone in soco.discover():
    # someday maybe save this to a config file instead of hardwired zone
    if zone.player_name == 'Master Bedroom':
        SoCo = zone
        break

if SoCo:
    #print 'Living Room IP: {0}'.format(SoCo.ip_address)
    SoCo.volume = int(setVolume)
    print SoCo.volume