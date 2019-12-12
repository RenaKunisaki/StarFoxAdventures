- the map `dbay` has `animtest` as its parent
- the lighting debug object loads sometimes in DarkIce Mines. still doesn't appear to function.
    - The object is "LGTDirectio", ID 0079, def 061C
    - its ObjectFileStruct.fileId = 0x2AA which is the lighting debug
- game text ID 0x02AC is used for the map screen but it has some unused entries:
    - Level [unused]
    - Choose
    - Select
    - Map [unused]
    - Rotate
    - Zoom
    - OK
    - Exit
    - TM &  2002 NINTENDO  GAME BY RARE [unused]

- warping to map 0x5D gives us AnimTest which is an empty void where Krystal does an animation and then the map reloads
    after reloading a few times it crashes
    the objects are:
    objPtr = 816B22A0 nObjs = 7
    Obj│Address │Name       │ID  │Def │ModelPtr│XPos     │YPos     │ZPos     │Ch
      0│812FB760│KP_Transpor│0025│059A│812FB86C│+19373.80│   +23.00│+30080.00│ 0
      1│812FB880│KP_Transpor│0025│059A│812FB98C│+13440.99│  +973.00│+30397.73│ 0
      2│81337280│AnimCamera │0010│001E│8133738C│+13440.99│  +973.00│+30397.73│ 0
      3│8133B8C0│Override   │0010│0006│8133B9CC│+13440.99│  +973.00│+30397.73│ 0
      4│81336D00│Override   │0010│0006│81336E0C│+13440.99│  +973.00│+30397.73│ 0
      5│806EDD00│Krystal    │0001│001F│806EDE0C│+13440.99│  +973.00│+30397.73│ 2
      6│8133A420│fox_shield │0030│0773│8133A52C│+13440.99│  +973.00│+30397.73│ 0

    map ID is 5 "ZNot Used - Krazoa Palace"
    Coords: +13440.99  +973.00 +30397.73  Cell   14   40 @ 8960 25600
    Anim 035A
