import sys
import soco

# one argument for favorite button pressed (0-5)
myArgs = sys.argv
stationIndex = int(myArgs[1])
#print 'Input args: {0}', format(myArgs)

# SoCo = soco.SoCo("10.0.1.29")
for zone in soco.discover():
    # someday maybe save this to a config file instead of hardwired zone
    if zone.player_name == 'Master Bedroom':
        SoCo = zone
        break

# for zone in soco.discover():
#     # someday maybe save this to a config file instead of hardwired zone
#     if zone.player_name == "Living Room":
#         SoCo = zone
#         break
#
if SoCo:
   # print 'Living Room IP: {0}'.format(SoCo.ip_address)

    meta_template = """
    <DIDL-Lite xmlns:dc="http://purl.org/dc/elements/1.1/"
        xmlns:upnp="urn:schemas-upnp-org:metadata-1-0/upnp/"
        xmlns:r="urn:schemas-rinconnetworks-com:metadata-1-0/"
        xmlns="urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/">
        <item id="R:0/0/0" parentID="R:0/0" restricted="true">
            <dc:title>{title}</dc:title>
            <upnp:class>object.item.audioItem.audioBroadcast</upnp:class>
            <desc id="cdudn" nameSpace="urn:schemas-rinconnetworks-com:metadata-1-0/">
                {service}
            </desc>
        </item>
    </DIDL-Lite>' """

    tunein_service = 'SA_RINCON65031_'

    stations = SoCo.get_favorite_radio_stations(0, 6)
    station = stations['favorites'][stationIndex]

    uri = station['uri'].replace('&', '&amp;')

    metadata = meta_template.format(title=station['title'], service=tunein_service)

    SoCo.play_uri(uri, metadata)
    print station['title'].replace(' | ', '\r', 1)
