interesting findings in kiosk version:

there is a 45 minute time limit in-game after which it resets. disable with code 04022630 60000000

pressing Z on controller 3 warps you to the world map. it doesn't unlock things though.
(I wonder if it bypasses the time limit? it's started somewhere on the title screen...)
it does, but it doesn't init properly, so you softlock after the arwing section.

either there's something preventing the controller from being updated, or Dolphin is a butt.
that, and the functions left in the final, are all the calls to getButtonsJustPressed for controllers other than the first.

New Game start in animtest:
062b3718 0000000c
c6146000 c2fe7511
c694a200 00000000
(same coords as final)

Debug Print:
c6142428 8007ce08
c2132d8c 0000000d
91410024 7d6802a6
91610068 3c60803c
80635614 80810008
80a1000c 80c10010
80e10014 81010018
8121001c 81410020
81610024 3c008028
6000d158 7c0903a6
4e800421 3c80803c
80a45614 7ca51a14
38a50001 90a45614
80a10068 7ca803a6
60000000 00000000

Infinite Money:
0038cec4 0000007f

Map Debug:
0405954c 38000001

Double Speed:
043c8674 42f00000

Bypass Ice Mountain Reset:
041a46d8 38000000

map dir idxs:
0x00 animtest
0x01 animtest
0x02 animtest
0x03 arwing
0x04 dragrock
0x05 animtest
0x06 dfptop
0x07 volcano
0x08 animtest
0x09 mazecave
0x0a dragrockbot
0x0b dfalls
0x0c swaphol
0x0d shipbattle
0x0e nwastes
0x0f warlock
0x10 shop
0x11 animtest
0x12 crfort
0x13 swapholbot
0x14 wallcity
0x15 lightfoot
0x16 cloudtreasure
0x17 animtest
0x18 clouddungeon
0x19 mmpass
0x1a darkicemines
0x1b animtest
0x1c desert
0x1d animtest
0x1e icemountain
0x1f animtest
0x20 animtest
0x21 animtest
0x22 darkicemines2
0x23 bossgaldon
0x24 animtest
0x25 insidegal
0x26 magiccave
0x27 dfshrine
0x28 mmshrine
0x29 ecshrine
0x2a gpshrine
0x2b dbshrine
0x2c nwshrine
0x2d worldmap
0x2e animtest
0x2f capeclaw
0x30 dbay
0x31 animtest
0x32 cloudrace
0x33 bossdrakor
0x34 animtest
0x35 bosstrex
0x36 linkb
0x37 cloudjoin
0x38 arwingtoplan
0x39 arwingdarkice
0x3a arwingcloud
0x3b arwingcity
0x3c arwingdragon
0x3d gamefront
0x3e linklevel
0x3f greatfox
0x40 linka
0x41 linkc
0x42 linkd
0x43 linke
0x44 linkf
0x45 linkg
0x46 linkh
0x47 linkj
0x48 linki

LUT:
 id|kiosk        |final         |final name
 00|shipbattle   |frontend      |Ship Battle
 01|animtest_05  |frontend2     |frontend2 (unused)
 02|dragrock     |dragrock      |Dragon Rock
 03|animtest_05  |krazoapalace  |Old Krazoa Palace
 04|volcano      |temple        |Volcano
 05|animtest_05  |hightop       |hightop (unused)
 06|animtest_05  |discovery     |discovery (unused)
 07|swaphol      |hollow        |ThornTail Hollow
 08|swapholbot   |hollow2       |ThornTail Bottom
 09|mazecave     |mazecave      |mazecave
 0A|nwastes      |wastes        |Snow Wastes
 0B|warlock      |warlock       |Krazoa Palace
 0C|crfort       |fortress      |CloudRun Fort
 0D|wallcity     |wallcity      |Walled City
 0E|lightfoot    |swapcircle    |LightFoot Village
 0F|cloudtreasure|cloudtreasure |CloudRun Treasure
 10|clouddungeon |clouddungeon  |CloudRun Dungeon
 11|animtest_05  |cloudtrap     |cloudtrap (unused)
 12|mmpass       |moonpass      |Moon Pass
 13|darkicemines |snowmines     |DarkIce Mines
 14|animtest_05  |krashrin2     |krashrin2 (unused)
*15|desert       |kraztest      |Ocean Force Bottom  - walled city magic cave - many things don't work
 16|animtest_05  |krazchamber   |krazchamber (unused)
 17|icemountain  |newicemount   |Ice Mountain
 18|animtest_1F  |newicemount2  |newicemount2 (unused)
 19|animtest_20  |newicemount3  |newicemount3 (unused)
 1A|animtest_05  |animtest      |animtest (unused)
 1B|darkicemines2|snowmines2    |DarkIce Mines 2
 1C|bossgaldon   |snowmines3    |Galdon
 1D|capeclaw     |capeclaw      |Cape Claw
 1E|insidegal    |insidegal     |Inisde Galdon
 1F|dfshrine     |dfshrine      |Test of Combat
 20|mmshrine     |mmshrine      |Test of Fear
 21|ecshrine     |ecshrine      |Test of Skill
 22|gpshrine     |gpshrine      |Test of Knowledge
 23|dbay         |diamondbay    |diamondbay (unused)
 24|animtest_05  |earthwalker   |earthwalker (unused)
 25|animtest_05  |willow        |willow (unused)
 26|arwing       |arwing        |Andross
 27|dbshrine     |dbshrine      |Test of Strength
 28|nwshrine     |nwshrine      |Scales Boss
 29|worldmap     |ccshrine      |World Map
 2A|animtest_05  |wgshrine      |wgshrine (unused)
 2B|cloudrace    |cloudrace     |CloudRun Race
 2C|bossdrakor   |finalboss     |Drakor
 2D|animtest_05  |wminsert      |wminsert (unused)
 2E|animtest_05  |snowmines4    |snowmines4 (unused)
 2F|animtest_05  |snowmines5    |snowmines5 (unused)
 30|bosstrex     |trexboss      |TRex Boss
 31|animtest_05  |mikelava      |mikelava (unused)
 32|dfptop       |dfptop        |Ocean Force Top
 33|shop         |swapstore     |Shop
 34|dragrockbot  |dragbot       |DragRock Bottom
 35|animtest_05  |kamdrag       |kamdrag (unused)
 36|magiccave    |magicave      |Magic Cave
*37|cloudjoin    |duster        |duster (unused)  a very bugged unused cutscene (maybe because of double speed?)
 38|animtest_05  |linkb         |linkb (Ice/Wastes)
 39|linkb        |cloudjoin     |cloudjoin (unused)
 3A|arwingtoplane|arwingtoplanet|Arwing to Planet
 3B|arwingdarkice|arwingdarkice |Arwing to Mines
 3C|arwingcloud  |arwingcloud   |Arwing to CloudRun
 3D|arwingcity   |arwingcity    |Arwing to WalledCity
 3E|arwingdragon |arwingdragon  |Arwing to DragRock
 3F|gamefront    |gamefront     |Title Screen
 40|linklevel    |linklevel     |linklevel (unused)
 41|greatfox     |greatfox      |GreatFox Scenes
 42|linka        |linka         |linka (WarpStone)
 43|linkc        |linkc         |linkc (Wastes/Hollow)
 44|linkd        |linkd         |linkd (Mines)
 45|linke        |linke         |linke (Moon/Hollow)
 46|linkf        |linkf         |linkf (Moon/Volcano)
 47|linkg        |linkg         |linkg (LightFoot/Hollow)
 48|linkh        |linkh         |linkh (LightFoot/CapeClaw)
 49|linkj        |linkj         |linkj (CapeClaw/OceanForce)
 4A|linki        |linki         |linki (CloudRun/Race)

Map act bit table:
mapActBitIdx[1]            XREF[3,1  gplayNewGame:800e7018(R),
mapActBitIdx                         dll_SaveGame_gplayGetAct:800
                                     dll_SaveGame_gplaySetAct:800
                                     gplayNewGame:800e7018(R)  
803005f0 00 00 00     GameBit
       00 07 6E
       08 EC 04
   3005f0 00 00        GameBit  CantCallTricky          [0x0]                         XREF[3]:   gplayNewGame:800e7018(R),
                                                                                                dll_SaveGame_gplayGetAct:800
                                                                                                dll_SaveGame_gplaySetAct:800
   3005f2 00 00        GameBit  CantCallTricky          [0x1]                         XREF[1]:   gplayNewGame:800e7018(R)  
   3005f4 07 6E        GameBit  0x76e                   [0x2]
   3005f6 08 EC        GameBit  0x8ec                   [0x3]
   3005f8 04 FE        GameBit  0x4fe                   [0x4]
   3005fa 00 DF        GameBit  0xdf                    [0x5]
   3005fc 00 E0        GameBit  0xe0                    [0x6]
   3005fe 00 E1        GameBit  0xe1                    [0x7]
   300600 00 E1        GameBit  0xe1                    [0x8]
   300602 00 E2        GameBit  0xe2                    [0x9]
   300604 00 E3        GameBit  0xe3                    [0xa]
   300606 00 E4        GameBit  0xe4                    [0xb]
   300608 00 E5        GameBit  0xe5                    [0xc]
   30060a 00 E6        GameBit  0xe6                    [0xd]
   30060c 00 E7        GameBit  0xe7                    [0xe]
   30060e 00 E8        GameBit  0xe8                    [0xf]
   300610 00 E9        GameBit  0xe9                    [0x10]
   300612 00 EA        GameBit  0xea                    [0x11]
   300614 00 EB        GameBit  0xeb                    [0x12]
   300616 04 92        GameBit  0x492                   [0x13]
   300618 00 00        GameBit  CantCallTricky          [0x14]
   30061a 05 D0        GameBit  0x5d0                   [0x15]
   30061c 00 00        GameBit  CantCallTricky          [0x16]
   30061e 00 ED        GameBit  0xed                    [0x17]
   300620 00 ED        GameBit  0xed                    [0x18]
   300622 00 ED        GameBit  0xed                    [0x19]
   300624 00 F0        GameBit  0xf0                    [0x1a]
   300626 00 00        GameBit  CantCallTricky          [0x1b]
   300628 02 29        GameBit  0x229                   [0x1c]
   30062a 00 EE        GameBit  0xee                    [0x1d]
   30062c 00 00        GameBit  CantCallTricky          [0x1e]
   30062e 00 EF        GameBit  0xef                    [0x1f]
   300630 00 00        GameBit  CantCallTricky          [0x20]
   300632 00 00        GameBit  CantCallTricky          [0x21]
   300634 00 00        GameBit  CantCallTricky          [0x22]
   300636 03 EE        GameBit  0x3ee                   [0x23]
   300638 00 00        GameBit  CantCallTricky          [0x24]
   30063a 00 00        GameBit  CantCallTricky          [0x25]
   30063c 00 00        GameBit  CantCallTricky          [0x26]
   30063e 00 00        GameBit  CantCallTricky          [0x27]
   300640 00 00        GameBit  CantCallTricky          [0x28]
   300642 00 00        GameBit  CantCallTricky          [0x29]
   300644 00 00        GameBit  CantCallTricky          [0x2a]
   300646 00 00        GameBit  CantCallTricky          [0x2b]
   300648 03 49        GameBit  0x349                   [0x2c]
   30064a 00 00        GameBit  CantCallTricky          [0x2d]
   30064c 04 92        GameBit  0x492                   [0x2e]
   30064e 04 92        GameBit  0x492                   [0x2f]
   300650 05 47        GameBit  0x547                   [0x30]
   300652 00 00        GameBit  CantCallTricky          [0x31]
   300654 05 D0        GameBit  0x5d0                   [0x32]
   300656 00 00        GameBit  CantCallTricky          [0x33]
   300658 07 6F        GameBit  0x76f                   [0x34]
   30065a 00 00        GameBit  CantCallTricky          [0x35]
   30065c 01 44        GameBit  0x144                   [0x36]
   30065e 00 00        GameBit  CantCallTricky          [0x37]
   300660 00 00        GameBit  CantCallTricky          [0x38]
   300662 00 00        GameBit  CantCallTricky          [0x39]
   300664 00 00        GameBit  CantCallTricky          [0x3a]
   300666 00 00        GameBit  CantCallTricky          [0x3b]
   300668 00 00        GameBit  CantCallTricky          [0x3c]
   30066a 00 00        GameBit  CantCallTricky          [0x3d]
   30066c 00 00        GameBit  CantCallTricky          [0x3e]
   30066e 00 00        GameBit  CantCallTricky          [0x3f]
   300670 00 00        GameBit  CantCallTricky          [0x40]
   300672 0C C2        GameBit  0xcc2                   [0x41]
   300674 0B 81        GameBit  0xb81                   [0x42]
   300676 00 00        GameBit  CantCallTricky          [0x43]
   300678 00 00        GameBit  CantCallTricky          [0x44]
   30067a 00 00        GameBit  CantCallTricky          [0x45]
   30067c 00 00        GameBit  CantCallTricky          [0x46]
   30067e 00 E1        GameBit  0xe1                    [0x47]
   300680 00 00        GameBit  CantCallTricky          [0x48]
   300682 00 00        GameBit  CantCallTricky          [0x49]
   300684 00 00        GameBit  CantCallTricky          [0x4a]
   300686 00 00        GameBit  CantCallTricky          [0x4b]
   300688 00 00        GameBit  CantCallTricky          [0x4c]
   30068a 00 00        GameBit  CantCallTricky          [0x4d]
   30068c 00 00        GameBit  CantCallTricky          [0x4e]
   30068e 00 00        GameBit  CantCallTricky          [0x4f]
   300690 00 00        GameBit  CantCallTricky          [0x50]
   300692 00 00        GameBit  CantCallTricky          [0x51]
   300694 00 00        GameBit  CantCallTricky          [0x52]
   300696 00 00        GameBit  CantCallTricky          [0x53]
   300698 00 00        GameBit  CantCallTricky          [0x54]
   30069a 00 00        GameBit  CantCallTricky          [0x55]
   30069c 00 00        GameBit  CantCallTricky          [0x56]
   30069e 00 00        GameBit  CantCallTricky          [0x57]
   3006a0 00 00        GameBit  CantCallTricky          [0x58]
   3006a2 00 00        GameBit  CantCallTricky          [0x59]
   3006a4 00 00        GameBit  CantCallTricky          [0x5a]
   3006a6 00 00        GameBit  CantCallTricky          [0x5b]
   3006a8 00 00        GameBit  CantCallTricky          [0x5c]
   3006aa 00 00        GameBit  CantCallTricky          [0x5d]
   3006ac 00 00        GameBit  CantCallTricky          [0x5e]
   3006ae 00 00        GameBit  CantCallTricky          [0x5f]
   3006b0 00 00        GameBit  CantCallTricky          [0x60]
   3006b2 00 00        GameBit  CantCallTricky          [0x61]
   3006b4 00 00        GameBit  CantCallTricky          [0x62]
   3006b6 00 00        GameBit  CantCallTricky          [0x63]
   3006b8 00 00        GameBit  CantCallTricky          [0x64]
   3006ba 00 00        GameBit  CantCallTricky          [0x65]
   3006bc 00 00        GameBit  CantCallTricky          [0x66]
   3006be 00 00        GameBit  CantCallTricky          [0x67]
   3006c0 00 00        GameBit  CantCallTricky          [0x68]
   3006c2 00 00        GameBit  CantCallTricky          [0x69]
   3006c4 00 00        GameBit  CantCallTricky          [0x6a]
   3006c6 00 00        GameBit  CantCallTricky          [0x6b]
   3006c8 00 00        GameBit  CantCallTricky          [0x6c]
   3006ca 00 00        GameBit  CantCallTricky          [0x6d]
   3006cc 00 00        GameBit  CantCallTricky          [0x6e]
   3006ce 00 00        GameBit  CantCallTricky          [0x6f]
   3006d0 00 00        GameBit  CantCallTricky          [0x70]
   3006d2 00 00        GameBit  CantCallTricky          [0x71]
   3006d4 00 00        GameBit  CantCallTricky          [0x72]
   3006d6 00 00        GameBit  CantCallTricky          [0x73]
   3006d8 00 00        GameBit  CantCallTricky          [0x74]
   3006da 00 00        GameBit  CantCallTricky          [0x75]
   3006dc 00 00        GameBit  CantCallTricky          [0x76]
   3006de 00 00        GameBit  CantCallTricky          [0x77]
