import sys
import soco

# one argument for volume button pressed (buttons 6 and 7)
myArgs = sys.argv
# volumeButton = myArgs[1]
#print 'Input args: {0}', format(myArgs)

SoCo = False

for zone in soco.discover():
    # someday maybe save this to a config file instead of hardwired zone
    if zone.player_name == 'Master Bedroom':
        SoCo = zone
        break

if SoCo:
    print str(SoCo.volume)