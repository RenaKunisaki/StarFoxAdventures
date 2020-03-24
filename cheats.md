# Note
Most AR codes beginning with 00, 02, or 04 will also work as Gecko codes and vice-versa.
Codes are for v1.00 unless otherwise noted.

# AR codes
## From random sites

### All Staff Upgrades
```
003A33AB 000000FF
003A33AC 000000FF
```

### Hold B for Moon Jump
```
4A3398B2 00000200
423428F8 00143FFF
```

### High Quality Models (ASM)
```
040415DC 48000028
04041610 48000020
040416C8 4800003C
04041738 60000000
04041744 60000000
```
(this code is for V1.01; it makes the fur effect always active. probably lags on real hardware)

### Player Wags Tail
```
04295AB0 38000003
```

### Infinite Staff Power
```
0429B28C A8040006
```

### Unlock All Cheats
```
043A31D4 FFFFFFFF
```

## From me

### Debug Print
```
04137948 4BF45D94
04148B78 4BF34B64
042510CC 4BE2C610
04246E04 4BE368D8
0428F7DC 4BDEDF00
```
(this doesn't include Tricky debug print because there's a lot of that)

### Tricky Debug Print
```
04148BC8 4BF34B14
```

### Enable Other Controllers
```
00014E73 00000004
```
(this lets the game read controllers 2-4. the only known use is that pressing A/B on controller 3 triggers some debug messages.)

### Enable Debug Text
```
003DDA28 00000001
```
(this does the same thing as the button code)

### Bypass Save File Checksum
```
0407EEF4 60000000
```
(this lets you load a hacked save file)

# Gecko codes
## From random sites

### Warp to Red Eye King
```
48000000 80343558
de000000 80008180
1200000c 0000c61e
e2000001 80008000
48000000 80343558
de000000 80008180
1200000e 00006a56
e2000001 80008000
48000000 80343558
de000000 80008180
12000010 00003d75
e2000001 80008000
48000000 80343558
de000000 80008180
12000012 0000c290
e2000001 80008000
48000000 80343558
de000000 80008180
12000014 0000c649
e2000001 80008000
48000000 80343558
de000000 80008180
12000016 00006675
e2000001 80008000
```
(this might be for a different version; it's meant to work instantly)
(or, it might be an AR code instead)

### staff
```
009F4C2D 00000000
011F1DA6 00000020
```
(unsure what this is meant to do)

## By me

### Unlock All Cheats
```
043a31d4 ffffffff
```

### Press Z for Level Select
```
04119d90 60000000
```
This shortens the normal button code to enable the level select feature. Just press Z before selecting Play Game on the title screen.

### No Save Confirmation
```
0412a97c 4bfbdd55
```
Selecting "Save" from the pause menu will save the game without asking if you're sure.

### Save Confirmation is Yes by Default
```
0012a983 00000000
```
When choosing Save, the cursor will be at Yes when the confirmation appears.

### Disable Pause Menu Voices
```
0412a904 60000000
0412a8c4 60000000
0412a95c 60000000
0412ba94 60000000
0412b8dc 60000000
0412b88c 60000000
0412b8b4 60000000
0412bd78 60000000
```

other places that call streamPlay from pause menu:
```
8012baa8
8012bad4
8012bb3c
8012bc54
8012bd50
8012bdcc
```

### D-Up to Change Game Speed
```
283398d2 00000008
82200000 803dea9c
86000000 01000000
86300000 fbffffff
84200000 803dea9c
e2000002 00000000
```
(cycles between 1x, 4x, 1/16x, 1/4x)

### D-Down to Switch Character
```
283398d2 00000004
82200000 803428f8
86000000 000000ac
86410000 00010000
e2000001 80000000
```
(if Krystal isn't loaded, a cube appears instead)
(these codes are obsolete; refer to https://github.com/RenaKunisaki/StarFoxAdventures/blob/master/patches/modelswap.gecko for the full mod.)

### D-Down to Switch Model
```
283398d2 00000004
48000000 803428f8
58010000 0000007c
92110000 00000000
92110001 00000004
94210000 00000000
94210001 00000004
e2000001 80000000
```
(swaps the model pointers. may be broken)

### Debug Print on-screen
```
c6148b78 80137948
c62510cc 80137948
c6246e04 80137948
c61378a8 80137948
c213798c 0000000d
91410024 7d6802a6
91610068 3c60803e
8063bc14 80810008
80a1000c 80c10010
80e10014 81010018
8121001c 81410020
81610024 3c008028
6000f688 7c0803a6
4e800021 3c80803e
80a4bc14 7ca51a14
38a50001 90a4bc14
80a10068 7ca803a6
60000000 00000000
```
(re-enables a debug message display function. not compatible with other Debug Print code above)

#### Include Tricky debug print
```
c6148bc8 80137948
```
(use with above code. displays a lot of text.)

#### Include all OSReport messages
```
c607d6dc 80137948
```
(many of these aren't intended to show on screen so may be hard to read)
