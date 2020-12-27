* Obj#: The object ID
* Idx#: The "real" object ID from OBJINDEX.bin (FFFF=use Obj#)
* Obj Name: The name in the object data
* Models: The first 4 model IDs
* DLL: Which DLL this object loads
* NP: Number of "player objects" this object creates
* Description: Some explanation

Obj#|Idx#|Obj Name   |Models              |DLL |NP|Description
----|----|-----------|--------------------|----|--|-----------
0000|0002|DummyObject|0000                |FFFF| 0|
0001|FFFF|AnimDummy  |0000                |00C6| 0|
0002|02A5|Sabre      |04E8 0001 04E9      |FFFF| 2|Fox player character
0003|FFFF|Krystal    |04E8 0001 04E9      |FFFF| 2|Krystal player character
0004|FFFF|Tricky     |0003                |00C4| 2|
0005|0492|TrickyBadge|0007                |FFFF| 0|
0006|04C8|TrickyBadge|0008                |FFFF| 0|
0007|FFFF|AnimTrickyB|0007                |00C6| 0|
0008|04BF|ModelsWithT|0004 0005 0006 00B5 |FFFF| 0|
0009|05B9|AnimTricky |0003                |00C6| 0|
000A|04C3|AnimKyte   |0009                |00C6| 0|
000B|04E1|TrickyExcla|0004                |FFFF| 0|"!" over Tricky's head
000C|004E|TrickyQuest|0005                |FFFF| 0|"?" over Tricky's head
000D|0493|TrickyFood |0006                |FFFF| 0|mushroom over Tricky's head
000E|FFFF|sword      |0467                |00E2| 0|crashes :(
000F|04AE|staff      |007E                |00E2| 0|
0010|0277|fox_shield |0084                |00E5| 0|
0011|0060|BackPack   |0087                |FFFF| 0|
0012|0094|groundquake|0014 0013           |FFFF| 0|effect for ground quake attack
0013|0099|staffAnim  |007E                |00C6| 0|
0014|04AF|omni_shield|0085                |00E5| 0|
0015|0513|staffEnd   |0081 007F 0080 0082…|FFFF| 0|
0016|009A|AnimStandar|0081                |00C6| 0|
0017|FFFF|SCweaponT1 |0077                |FFFF| 0|
0018|05BC|animSCweapo|0077                |00C6| 0|
0019|FFFF|SCweaponT2 |0078                |FFFF| 0|
001A|04C1|SCweaponT3 |0079                |FFFF| 0|
001B|FFFF|SCweaponT4 |007A                |FFFF| 0|
001C|FFFF|LFweaponT1 |007B                |FFFF| 0|
001D|FFFF|LFweaponT2 |007C                |FFFF| 0|
001E|04C7|BGSweapon  |007D                |FFFF| 0|
001F|0003|projball   |048A 0489           |00E3| 0|fireball shot from staff
0020|FFFF|AnimProjBal|048A 0489           |00E3| 0|
0021|FFFF|rapidFireLa|0487                |029B| 0|
0022|02F6|FlameThrowe|045F                |00E4| 0|
0023|000E|FlameBall  |00AA                |00E4| 0|
0024|0004|SkeetlaWall|0461                |00D4| 0|
0025|0008|ChukChuk   |0034                |00CC| 0|
0026|FFFF|Kaldachom  |002F                |00D5| 0|
0027|FFFF|KaldachomMe|0030                |00D6| 0|
0028|04CC|Grimble    |002B                |00D0| 0|
0029|049D|TumbleWeedB|0039                |00D1| 0|
002A|FFFF|TumbleWeedB|003A                |00D1| 0|
002B|FFFF|TumbleWeedB|0039                |00D1| 0|
002C|03E7|TumbleWeedB|0039                |00D1| 0|
002D|FFFF|GuardClaw  |001D                |00C9| 0|
002E|049C|GCRobotPatr|001E                |00C9| 6|
002F|03E6|Vambat     |003D                |00C9| 0|
0030|0584|Firebat    |003D                |00C9| 0|
0031|02B1|HagabonMK2 |002E                |00C9| 0|jellyfish in Krazoa Palace
0032|0586|Mikaladon  |0031                |00C9| 0|
0033|0017|SpittingEba|0035                |00C9| 0|
0034|0130|MutatedEba |0038                |00C9| 0|
0035|0131|snowworm   |0021                |00C9| 0|
0036|0129|snowworm_ba|0021                |00C9| 0|
0037|04D1|Whirlpool  |04BB                |00C9| 0|
0038|0587|Rachnop    |0023                |00C9| 0|
0039|0588|PinPon     |002C                |00C9| 0|
003A|01B7|WB         |0032                |00C9| 0|annoying birds flying around TTH well
003B|0589|WBAnim     |0032                |00C6| 0|
003C|02A4|Weevil     |0033                |00C9| 0|
003D|013F|BattleDroid|0041                |00C9| 0|
003E|FFFF|Kooshy     |002A                |00C9| 0|
003F|FFFF|KooshyHill |0046                |FFFF| 0|
0040|FFFF|HoodedZyck |0029                |00C9| 0|
0041|FFFF|FireCrawler|003F                |00C9| 0|
0042|FFFF|RedEye     |003E                |00C9| 0|
0043|013D|ShadowHunte|0040                |00C9| 0|
0044|04D3|SwampStride|0042                |00C9| 0|
0045|02A0|Hagabon    |002D                |00DF| 0|
0046|04D4|SwarmBaddie|045F                |00E0| 0|
0047|FFFF|WispBaddie |045F                |00E1| 0|
0048|FFFF|KaldachomSh|004E                |FFFF| 0|
0049|FFFF|LaserTurret|004E                |012E| 0|
004A|FFFF|KaldachomSp|0048                |00D7| 0|
004B|051A|FireCrawler|03B7                |00D7| 0|
004C|051C|PinPonSpike|0049                |00D8| 0|
004D|051D|MikaBomb   |0048                |00DB| 0|
004E|051E|MikaBombSha|0048                |00DC| 0|
004F|051F|IceBall    |0044                |00CD| 0|
0050|0520|IceBallSmal|0044                |00CD| 0|
0051|0146|ChukaChuck |0058                |00CD| 0|
0052|0145|Pollen     |0036                |00D9| 0|
0053|04D6|PollenFragm|0037                |00DA| 0|
0054|0521|Tumbleweed1|004A 004B           |00D2| 0|
0055|02A3|Tumbleweed2|004C                |00D2| 0|
0056|0276|Tumbleweed3|004A 004B           |00D2| 0|
0057|0274|Tumbleweed4|004B                |00D2| 0|
0058|04B2|Tumbleweed1|003B                |00D2| 0|
0059|FFFF|Tumbleweed2|003C                |00D2| 0|
005A|04DE|Tumbleweed3|003B                |00D2| 0|
005B|FFFF|GCRobotLigh|001F                |0150| 0|
005C|FFFF|GCRobotBlas|0476                |0240| 0|
005D|027B|GCRobotPatr|03A5                |017C| 0|
005E|02A2|GCbaddieShi|004D                |00DD| 0|
005F|FFFF|sharpclawSn|0016                |00C9|10|
0060|04B3|sharpclawGr|0017                |00C9|10|
0061|029F|sharpclawSo|0018                |00C9|10|
0062|058F|sharpclawCo|001A                |00C9|10|
0063|0590|sharpclawAs|0019                |00C9|10|
0064|004F|sharpclawSh|0017                |00C9|10|
0065|027E|CannonClaw |0016                |00CF| 0|
0066|04B5|CannonClawO|0017                |00CF| 0|
0067|01BB|BossGeneral|004F                |00C9|10|
0068|009B|baddieInter|045F                |00DE| 0|
0069|000F|AnimSnowCla|0016                |00C6| 0|
006A|04DF|AnimSharpcl|0017                |00C6| 0|
006B|0272|AnimSharpcl|0018                |00C6| 0|
006C|FFFF|AnimSharpcl|001A                |00C6| 0|
006D|0501|AnimSharpcl|0019                |00C6| 0|
006E|0491|DIM_Boss   |0054                |01E0| 0|
006F|04D8|DIM_BossGut|0055                |01E1| 0|
0070|04D2|DIM_BossTon|0056                |01E2| 0|
0071|FFFF|DIM_BossGut|0057                |01E3| 0|
0072|01BD|DIMBossIceS|0059 005A 005B 005C…|013E| 0|
0073|FFFF|DIM_BossSpi|0058                |01E5| 0|
0074|FFFF|DIMSpellsto|02F2                |00C6| 0|
0075|0495|DIMSpellsto|02F2                |012E| 0|
0076|FFFF|DIMbosscrac|045F                |01E6| 0|
0077|00E9|DIMbossfire|045F                |01E7| 0|
0078|048A|AnimDrakor |0066                |00C6| 0|
0079|0271|BossDrakor |0066                |024D| 0|
007A|0270|DrakorMissi|04D7 04D6           |0262| 0|
007B|01F1|DrakorHover|0365                |0271| 0|
007C|00ED|DrakorEnerg|0498                |0241| 0|
007D|029A|DrakorCrate|02B6                |0105| 0|
007E|029B|DrakorSpell|02F4                |025E| 0|
007F|026E|DRDebrisGir|035C                |011A| 0|
0080|0000|InfoTablet |010D                |012E| 0|
0081|026F|DRDebrisPip|035D                |011A| 0|
0082|0275|DRDebrisPip|035E                |011A| 0|
0083|00FE|BossDrakorD|006C                |024E| 0|
0084|0299|Timer      |0336                |02B5| 0|
0085|FFFF|BossDarkorF|0000                |00E4| 0|
0086|01F8|BossDrakorF|03A1                |0273| 0|
0087|01F7|BossDrakor_|0466                |0112| 0|
0088|00FF|BossDrakor_|045F                |024C| 0|
0089|00C4|KT_RexLevel|0467                |024F| 0|
008A|0567|KT_Rex     |006D 006E           |0250| 0|
008B|05BD|KT_RexSeque|0466                |0112| 0|
008C|02F9|KT_RexDoorP|0070                |0110| 0|
008D|009C|KT_RexDoorT|0070                |0110| 0|
008E|02F5|KT_RexFloor|006F                |0251| 2|
008F|FFFF|KT_Lazerwal|0000                |0252| 0|
0090|FFFF|KT_Lazerlig|045F                |0253| 0|
0091|FFFF|KT_Torch   |0071                |0128| 0|
0092|01F6|KT_Fallingr|045F                |0254| 0|
0093|FFFF|KT_AnimSpel|02F2                |00C6| 0|
0094|FFFF|generalscal|0050                |01F5| 0|
0095|FFFF|CF_generals|0053                |01F5| 0|
0096|FFFF|KytesMum   |0009                |0266| 0|
0097|FFFF|AnimKytesMu|0009                |00C6| 0|
0098|01DF|AnimGuardCl|001D                |00C6| 0|
0099|01F5|steggy     |0000                |00C6| 0|
009A|02FB|Nutter     |0093                |00C6| 0|
009B|0100|Trex1      |003E                |0128| 0|
009C|0101|Krazoa     |0282                |00C6| 0|
009D|FFFF|AnimGuardCl|001D                |00C6| 0|
009E|04C2|CFrunnersta|034B                |012E| 0|
009F|0140|CFrunnersta|034C                |012E| 0|
00A0|00B1|CFLightPill|0177                |012E| 0|
00A1|0141|CFLightWall|0178                |012E| 0|
00A2|0142|CFPressureS|0179                |01FE| 2|
00A3|05BB|CFDungeonLi|017B                |012E| 0|
00A4|0143|CFGCrubble1|034D                |012E| 0|
00A5|0144|CFGCrubble2|034E                |012E| 0|
00A6|01F4|CFGCrubble3|034F                |012E| 0|
00A7|FFFF|CFGCrubble4|0350                |012E| 0|
00A8|01F3|CFGCpillbre|0351                |012E| 0|
00A9|FFFF|CFGCsmallbl|0352                |012E| 0|
00AA|FFFF|SkyDome    |0450                |012E| 0|
00AB|FFFF|CFLevelCont|0467                |0164| 0|sets music
00AC|FFFF|CFCloudCalP|0324                |00ED| 0|
00AD|FFFF|CFAnimCloud|0324                |00C6| 0|
00AE|FFFF|CFCloudCall|0324                |FFFF| 0|
00AF|FFFF|CloudPrison|0467                |0145| 0|
00B0|FFFF|CloudShipCo|0467                |0146| 0|
00B1|01F2|CF_ObjCreat|0469                |0152| 0|
00B2|FFFF|CFPrisonGua|0017                |014E| 2|
00B3|04B4|CFPickKryst|0086                |00ED| 0|
00B4|FFFF|CFPowerLock|033A 033B           |0111| 0|
00B5|00E5|CFPrisonDoo|030A                |0110| 0|
00B6|01F0|CFWinPrison|0346                |0110| 0|
00B7|00D0|CFPrisonUnc|000E                |014F| 2|
00B8|00AF|CFPerch    |031F                |0153| 4|
00B9|00B2|CFPrisonCag|0320                |0154| 0|
00BA|00C3|CFPrisonCag|0321                |00C6| 0|
00BB|FFFF|CFGuardian |0093                |0148| 0|
00BC|0096|CFbrokenGra|0347 0348 0349 034A |015A| 0|
00BD|00F2|CFbrokenGra|0347 0348 0349 034A |0166| 0|
00BE|FFFF|AnimCFGuard|0093                |00C6| 0|
00BF|006A|CFbrokenWal|017C 017D 017E 017F…|015A| 0|
00C0|00A1|CFbrokenWal|017C 017D 017E 017F…|0166| 0|
00C1|00B5|AnimSpell  |02EC                |00C6| 0|
00C2|00B7|CFWindLift |0319 031A           |0149| 0|
00C3|00BB|CFTreasWind|0047 0047           |0149| 0|
00C4|FFFF|CFGoldenPer|0327                |0112| 0|
00C5|00C2|CFCageSwitc|0325                |0154| 0|
00C6|FFFF|CFDoubleSwi|0325                |0111| 0|
00C7|FFFF|CFAttractor|0466                |015F| 0|
00C8|00CB|CFAnimSnowB|02FC                |00C6| 0|
00C9|00CC|CFRemovalSh|0017                |0165| 0|
00CA|00CD|CFMagicWall|0306                |0162| 0|
00CB|00CE|CFPowerDoor|0315                |00F4| 0|
00CC|FFFF|CFPowerDoor|0316                |00C6| 0|
00CD|00D1|CFPowerDoor|0317                |00C6| 0|
00CE|00D3|CFPowerDoor|0318                |00C6| 0|
00CF|00D6|CFSlideDoor|0326                |015D| 0|
00D0|FFFF|CFMainSlide|0326                |015D| 0|
00D1|FFFF|CFCrate    |02E8                |012A| 0|
00D2|00B0|CFSunTemple|0197                |0293| 0|
00D3|FFFF|CFScalesGal|0096                |0151| 0|
00D4|05A7|CF_DoorLigh|0345                |0131| 0|
00D5|00CF|CF_MovDeckD|00B1                |0110| 0|
00D6|01B3|AnimSpaceCr|0072                |00C6| 0|
00D7|FFFF|CFPowerCrys|031B                |00ED| 0|
00D8|002D|CFPowerCrys|031C                |00ED| 0|
00D9|00D7|CFPowerCrys|031D                |00ED| 0|
00DA|00DA|CFPowerBase|0312 0313 0314      |014A| 0|
00DB|00DB|CFPowerCrys|031B                |00C6| 0|
00DC|00E1|CFPowerCrys|031C                |00C6| 0|
00DD|FFFF|CFPowerCrys|031D                |00C6| 0|
00DE|01EF|CFSmallPowe|031B                |00C6| 0|
00DF|00D8|CFSmallPowe|031C                |00C6| 0|
00E0|00D9|CFSmallPowe|031D                |00C6| 0|
00E1|00DC|CFMainCryst|0310 0311           |014B| 0|
00E2|00DD|CFCloudBaby|000A                |014C| 0|
00E3|0191|CFAmbientCl|000F                |014C| 0|
00E4|00E2|CFAnimCloud|000E                |00C6| 0|
00E5|FFFF|CFAnimBaby |000A                |00C6| 0|
00E6|00E6|CFTreasureD|032B                |0110| 0|
00E7|FFFF|CFPuzzleSwi|02F5                |FFFF| 0|
00E8|00E7|LaserBeam  |0000                |014D| 0|
00E9|00E8|CFBlastedRo|0328                |0159| 0|
00EA|FFFF|CFBlastedWa|0342 0343 0344      |0159| 0|
00EB|FFFF|CFExplodeWa|0342 0343 0344      |015A| 0|
00EC|04B1|CFWallPieac|0342 0343 0344      |0166| 0|
00ED|01EE|CFExplodeFl|032C 032D 032E 032F…|015A| 0|
00EE|01B9|CF_RockPiec|032C                |00C6| 0|
00EF|01B8|CFExplodePi|032C 032D 032E 032F…|0166| 0|
00F0|01ED|CFBlastedTu|0466                |0159| 0|
00F1|01CA|LINKIseqobj|0466                |0112| 0|
00F2|01B2|CFseqobject|0466                |0112| 0|
00F3|01B1|CFRaceSeqob|0466                |0112| 0|
00F4|0522|CAMERAseqob|0466                |0112| 0|
00F5|03E4|CAMERAnewse|0466                |0113| 0|
00F6|03E5|AnimCFTreas|0335                |00C6| 2|
00F7|02FD|CFTreasRobo|0335                |0161| 2|
00F8|02FE|CFPowerRoom|031E                |00ED| 0|
00F9|02FF|CFAnimPower|031E                |00C6| 0|
00FA|01B0|CFExplosive|03E5                |00ED| 0|
00FB|03A0|CFExplosive|0305                |0110| 0|
00FC|01EC|CFPowerRoom|031E                |00C6| 0|
00FD|01EB|AnimRobotPa|001E                |00C6| 0|
00FE|01E9|CFForceFiel|0467                |015B| 0|
00FF|01E8|CFForceFiel|0467                |015C| 0|
0100|01E7|CFMainDoorA|0322                |0110| 0|
0101|026D|CFMainDoorB|0323                |00C6| 0|
0102|01E6|CFBrokenPil|0337                |0110| 0|
0103|01EA|CFBrokenPil|0338                |00C6| 0|
0104|04FF|CFBrokenPil|0339                |00C6| 0|
0105|01E4|CFWindLiftL|016D                |0277| 0|
0106|049E|CF_WallTorc|024B                |02A9| 0|
0107|0298|CF_Lamp    |00AF                |01F4| 0|
0108|01AF|CRCloudRace|0467                |025D| 0|
0109|018B|CRFuelTank |0000                |025F| 0|
010A|04FD|CRSnowClaw |0017                |025C| 0|
010B|0297|CRSnowClaw2|0017                |025C| 0|
010C|0296|CRSnowClaw3|0017                |025C| 0|
010D|018C|CRSnowBike |02FC                |0255| 0|
010E|0295|CRDropBomb |0308 0309           |0260| 0|
010F|0294|CRrockfall |0307                |016A| 0|
0110|0190|CRSnowClawB|02FC                |0255| 0|
0111|018D|CRSnowClawB|02FC                |0255| 0|
0112|04D9|CRSnowClawB|02FC                |0255| 0|
0113|0301|AnimSpellst|02F2                |00C6| 0|
0114|0302|AnimSpellst|02F3                |00C6| 0|
0115|FFFF|CRSpellston|02F3                |025E| 0|
0116|0279|CF_ChestSpe|02F3                |00ED| 0|
0117|027D|WCKingEarth|0435                |028A| 0|
0118|0186|WCEarthWalk|000C                |028A| 0|
0119|0304|AnimKingEar|0435                |00C6| 0|
011A|049F|WCTrex     |003E                |FFFF| 0|
011B|04A1|WCBouncyCra|03B4                |028C| 0|
011C|04A2|WCTeethBowl|03B4                |0111| 0|
011D|FFFF|WCSeqObject|0466                |0112| 0|
011E|00F8|WCTempleBlo|0407 0408           |0112| 0|
011F|00B8|WCTempleDoo|0409                |0110| 0|
0120|0306|WCTempleDoo|040A                |0110| 0|
0121|030A|WCTempleDoo|040B                |0110| 0|
0122|0307|WCLevelCont|0467                |028D| 0|
0123|00FC|WCSunTemple|040C                |0110| 0|
0124|FFFF|WCMoonTempl|040D                |0110| 0|
0125|030B|WCSunDoor  |040E                |012E| 0|
0126|FFFF|WCMoonDoor |040F                |012E| 0|
0127|00B9|WCSlabDoor |0410                |0110| 0|
0128|00C5|WCPressureS|0414 0415           |028F| 0|
0129|0308|WCTemplePre|0433                |00FB| 0|
012A|FFFF|WCBeacon   |0412 0413           |028E| 0|
012B|01A1|WCTrexStatu|0416 0417           |0292| 0|
012C|FFFF|WCAnimSilve|041F                |00C6| 0|
012D|FFFF|WCAnimGoldT|041E                |00C6| 0|
012E|FFFF|WCAnimSunSt|0423                |00C6| 0|
012F|028F|WCAnimMoonS|0424                |00C6| 0|
0130|04D5|WCSunStone |0423                |00ED| 0|
0131|FFFF|WCMoonStone|0424                |00ED| 0|
0132|01A2|WCInvUseObj|0467                |0293| 0|
0133|04C0|WCGoldTrexT|041E                |00ED| 0|
0134|FFFF|WCSilverTre|041F                |00ED| 0|
0135|019E|WCBossDoor |0411                |0110| 0|
0136|019F|WCGeneralDo|0422                |0110| 0|
0137|019D|WCAnimGener|0422                |00C6| 0|
0138|0303|WCMoonTempl|0421                |0294| 0|
0139|039F|WCSunTemple|0420                |0294| 0|
013A|005F|WCPushBlock|0418 0419           |0290| 0|
013B|039E|WCPushBlock|041A 041B           |00EF| 0|
013C|01A0|WCTile     |041D 041C           |0291| 0|
013D|03A2|WCApertures|0425                |0128| 0|
013E|03A5|WCAperturem|0426                |0128| 0|
013F|03A6|WCApertureS|041C 041D           |0295| 0|
0140|03A7|WCSunTemple|0427                |0293| 0|
0141|03A8|WCSunTemple|0428 0429           |0293| 0|
0142|03A3|WCTempleDia|042A 042B 042C      |0296| 0|
0143|0290|WCTempleBri|042D 042E           |0297| 0|
0144|03A4|WCSunInside|0430                |0110| 0|
0145|03AD|WCMoonInsid|0431                |0110| 0|
0146|03AE|WCFloorTile|0432                |0298| 0|
0147|0280|WCTempleTop|0434                |0110| 0|
0148|0282|WCSunRing  |0496                |02A0| 0|
0149|FFFF|WCMoonRing |0498                |02A0| 0|
014A|04D7|DIMafterbos|03AB                |00C6| 0|
014B|001F|DIMMagicLig|0302                |016B| 0|
014C|019A|DIMLavaBall|03B7                |01BE| 0|
014D|03AF|DIMLavaDebr|03B7                |01BE| 0|
014E|019B|DIMLavaBall|045F                |01BF| 0|
014F|0292|DIMLever   |0325                |0111| 0|
0150|03B3|DIMBridgeCo|03C1                |00C6| 0|
0151|03B0|DIMBridgeCo|03C1                |00ED| 0|
0152|FFFF|DIMBridgeCo|03BC                |0128| 0|
0153|FFFF|DIMBridgeCo|03BD                |0128| 0|
0154|01E3|DIMBridgeCo|03BE                |0128| 0|
0155|006F|DIMBridgeCo|03BF                |0128| 0|
0156|017D|DIMBridgeCo|03C0                |0128| 0|
0157|030D|DIMBridgeCo|03C2 03C3           |01C8| 0|
0158|02AD|DIMBridge  |03C4 03C5           |0112| 0|
0159|02AE|DIMSnowBall|0045                |01C1| 0|
015A|FFFF|DIMSnowBall|045F                |01C2| 0|
015B|02AC|DIMSnowHorn|03AA                |0256| 2|
015C|0293|DIMSnowHorn|03AC                |0111| 0|
015D|03B2|DIMExplosio|0012 0013 0015 0014 |01CA| 0|
015E|0199|DIMShackleK|03DB                |00ED| 0|
015F|0497|DIMplacehol|045F                |FFFF| 0|
0160|04BE|DIMLogFire |02DA                |01C0| 0|
0161|03B6|DIMLogFireR|02DB                |01C0| 0|
0162|FFFF|DIMSeqObjec|0466                |0112| 0|
0163|FFFF|DIMGate    |03AD 03AE 03AF      |01C3| 0|
0164|0009|DIM_Portcul|00E0 00DF           |0110| 0|
0165|0069|DIMHutDoor |03B1                |0128| 0|
0166|01C2|DIMDoorBrid|03D0                |0128| 0|
0167|0198|DIMWoodDoor|03E6                |0110| 0|
0168|01C5|DIMWoodDoor|03B0                |01CB| 0|
0169|04E0|DIMBarrier |03B2                |01C5| 0|
016A|02A6|DIMDismount|045F                |01C9| 0|
016B|01C3|DIMAlpineRo|00C2                |00ED| 0|
016C|01BE|DIMAlpineRo|00C2                |00ED| 0|
016D|01C0|DIMAlpineRo|00C2                |00C6| 0|
016E|02A7|DIMFlag    |03B3                |01F6| 0|
016F|01BF|DIMIceWall |045F                |01C4| 0|
0170|01C1|DIMPushBloc|00BB                |00EF| 0|
0171|02FA|DIMicefloor|03CB                |00C6| 0|
0172|01C4|DIMicefloor|03CC                |00C6| 0|
0173|0309|DIMicefloor|03CD                |00C6| 0|
0174|03B7|DIMicefloor|03CE                |00C6| 0|
0175|000B|DIMicefloor|03CF                |00C6| 0|
0176|0515|DIMCannon  |03B5                |01C6| 0|
0177|03B8|DIMCannonBa|03B6                |01C6| 0|
0178|03B9|DIMLavaSmas|03B9 03BA 03BB      |01C7| 0|
0179|03BA|DIMUseObjec|045F                |0111| 0|
017A|03BB|DIMUseObjec|045F                |0111| 0|
017B|000D|DIMMagicBri|03C8                |01CC| 0|
017C|000C|DIMTruthHor|03CA                |01D1| 0|
017D|0517|DIMTruthHor|03C9                |00ED| 0|
017E|03BC|DIMTruthHor|03C9                |00C6| 0|
017F|0185|DIM_LevelCo|0467                |01CD| 0|
0180|0187|DIM_tricky |0467                |01D0| 0|
0181|FFFF|DIM2_WallEx|03DE 03DF 03E0 03E1…|015A| 0|
0182|0188|DIM2_WallEx|03DE 03DF 03E0 03E1…|0166| 0|
0183|0189|DIM2_barrel|0160                |017D| 0|
0184|018A|DIM2Torch  |03B8                |012E| 0|
0185|0195|DIM2SeqObje|0466                |0112| 0|
0186|0196|DIM2UseObje|045F                |0111| 0|
0187|030C|DIM2Conveyo|045F                |01D5| 0|
0188|FFFF|DIM2Mammoth|03D3                |0112| 0|
0189|03A1|DIM2Mammoth|03AA                |0128| 0|
018A|0197|DIM2CellKey|03DB                |00ED| 0|
018B|027A|DIM2IceFloe|03E9                |01DC| 0|
018C|01DD|DIM2IceFloe|03EC                |01DC| 0|
018D|014C|DIM2IceFloe|03ED                |01DC| 0|
018E|014E|DIM2IcePlat|0405                |0110| 0|
018F|015B|DIM2IcePlat|0406                |0110| 0|
0190|015C|DIM2TrapDoo|03F8                |0110| 0|
0191|0160|DIM2TrapDoo|03F8                |0128| 0|
0192|0162|DIM2TrapDoo|03F7                |0110| 0|
0193|01D1|DIM2TrapDoo|03F7                |0128| 0|
0194|0508|DIM2CannonT|03D1 03D2           |01C5| 0|
0195|0281|DIM2SnowBal|03E4                |01D7| 0|
0196|0159|DIM2PathGen|045F                |01D8| 0|
0197|015A|DIM2PrisonM|03AA                |01D9| 0|
0198|050A|DIM2CellDoo|03D4 03D5           |0110| 0|
0199|01DE|DIM2CellDoo|03D6 03D7           |0128| 0|
019A|01E0|DIM2CellDoo|03D4 03D5           |0110| 0|
019B|04B8|DIM2CellDoo|03D6 03D7           |0128| 0|
019C|03F4|DIM2BossDoo|03D8 03D9 03DA      |01C3| 0|
019D|FFFF|DIM2SilverK|03DB                |00ED| 0|
019E|03F6|DIM2GoldKey|0098                |00ED| 0|
019F|01D2|DIM2CellKey|045F                |0111| 0|
01A0|03F7|DIM2CellLoc|03DC 03DD           |0112| 0|
01A1|FFFF|DIM2PuzzleD|03E6                |0110| 0|
01A2|03F8|DIM2Explode|03F9 03FA 03FB 03FC…|013E| 0|
01A3|03F9|DIM2RoofRub|03F9                |00C7| 0|
01A4|03FA|DIM2RoofRub|03FA                |00C7| 0|
01A5|03FB|DIM2RoofRub|03FB                |00C7| 0|
01A6|FFFF|DIM2RoofRub|03FC                |00C7| 0|
01A7|015E|DIM2RoofRub|03FD                |00C7| 0|
01A8|FFFF|DIM2RoofRub|03FE                |00C7| 0|
01A9|0163|DIM2RoofRub|03FF                |00C7| 0|
01AA|016A|DIM2RoofRub|0400                |00C7| 0|
01AB|016B|DIM2RoofRub|0401                |00C7| 0|
01AC|03FD|DIM2RoofRub|0402                |00C7| 0|
01AD|FFFF|DIM2RoofRub|0403                |00C7| 0|
01AE|FFFF|DIM2RoofRub|0404                |00C7| 0|
01AF|FFFF|DIM2IceBloc|00BB                |00EF| 0|
01B0|FFFF|DIM2Icicle |03E7                |01DD| 0|
01B1|FFFF|DIM2IceGrat|03E8                |0110| 0|
01B2|03FE|DIM2StonePi|03EE                |0112| 0|
01B3|04BA|DIM2LavaCon|0467                |01DE| 0|
01B4|FFFF|DIMAnimShar|0016                |0184| 0|
01B5|0022|MAGICMaker |045F                |01E4| 0|
01B6|FFFF|Blacksquare|0065                |00C6| 0|
01B7|0023|SpaceShip  |0072                |00C6| 0|
01B8|0400|IMIceMounta|0467                |0169| 0|
01B9|0405|IMSeqObject|0466                |0112| 0|
01BA|FFFF|IMMultiSeq |0466                |0114| 0|
01BB|040E|IMIcicle   |02FB                |016A| 0|
01BC|040F|IMIcePillar|02FB                |016D| 0|
01BD|0410|IMSnowBike |02FC                |0255| 0|
01BE|0411|IMSnowClawB|02FC                |0255| 0|
01BF|0408|IMSnowClawB|02FC                |0255| 0|
01C0|FFFF|IMSnowClaw |0016                |025C| 0|
01C1|0007|IMSnowClaw2|0016                |025C| 0|
01C2|01D4|IMPrisonGat|02FD 02FE           |0110| 0|
01C3|04A0|IMMagicLigh|0301                |016B| 0|
01C4|FFFF|IMMagicLigh|0303                |016B| 0|
01C5|FFFF|IMAnimSpace|0072                |016E| 0|
01C6|FFFF|IMSpaceThru|0073 0074           |016F| 0|
01C7|FFFF|IMSpaceRing|0076                |0170| 0|
01C8|016E|IMSpaceTrac|0075                |00C6| 0|
01C9|016F|IMSpaceRing|0076                |0171| 0|
01CA|04BB|IMFence    |02FF 0300           |0110| 0|
01CB|0170|LINK_levcon|0467                |0173| 0|
01CC|02B4|LINKF_InfoT|0010                |0121| 0|
01CD|FFFF|LinkG_InfoT|0010                |0121| 0|
01CE|FFFF|LINKE_Explo|03F0 03F1 03F2 03F3…|015A| 0|
01CF|0161|LINKE_Explo|03F0 03F1 03F2 03F3…|0166| 0|
01D0|03F3|LINKE_stone|03EF                |0112| 0|
01D1|FFFF|LINK_BlueMu|04DA                |01A7| 0|
01D2|FFFF|LINK_SnowPr|04DB                |00FB| 2|
01D3|0500|LINK_UnderW|0240                |00FB| 2|
01D4|0169|LINK_Portcu|04DC 04DD           |0110| 0|
01D5|0176|LINKStaffLe|04DE                |011C| 0|
01D6|0177|LINKSnowTre|04DF                |011A| 0|
01D7|040D|LINKPoleFla|04E0                |0206| 0|
01D8|FFFF|LINKSnowLog|04E1                |011A| 0|
01D9|03FF|LINKSnowTre|04E2                |011A| 0|
01DA|FFFF|LINKSnowGra|04E3                |02AD| 0|
01DB|027C|LINKJseqobj|0466                |0113| 0|
01DC|FFFF|VFP_Platfor|0200                |021F| 0|
01DD|FFFF|LINKB_levco|0467                |0172| 0|falling snow
01DE|FFFF|LINKB_seqob|0466                |0113| 0|
01DF|0406|linkB_Stone|04E5                |0112| 0|
01E0|0407|LINKC_seqob|0466                |0113| 0|
01E1|FFFF|LinkF_liftg|04D8                |012A| 0|
01E2|FFFF|LinkF_cog  |04D9                |012A| 0|
01E3|FFFF|CCTestInfot|0010                |0122| 0|
01E4|FFFF|CClevcontro|0467                |018B| 0|changes sky
01E5|FFFF|CC_HighTop |0094                |0272| 0|
01E6|FFFF|CCeyeVines |00D9                |00E7| 0|
01E7|0413|CCwaterShut|01EB                |0110| 0|
01E8|FFFF|CCExploding|01EA 01E4 01E5 01E6…|015A| 0|
01E9|FFFF|CCExploding|01E4 01E5 01E6 01E7…|0166| 0|
01EA|FFFF|CCOceanDoor|01E1                |00C6| 0|
01EB|FFFF|CCOceanDoor|01E1                |0110| 0|
01EC|FFFF|CCfireCryst|01DC                |00C7| 0|
01ED|FFFF|CCpedstal  |01E2 01E3           |018A| 0|
01EE|0178|CCskullDoor|01E0                |0110| 0|
01EF|FFFF|CCclawOnlyD|01DF                |0110| 0|
01F0|FFFF|CCprisonDoo|01DE                |0110| 0|
01F1|0414|CCriverflow|045F                |0174| 0|
01F2|FFFF|CCSharpclaw|01DD                |0189| 0|
01F3|0415|CCfireCryst|01DC                |00ED| 0|
01F4|0416|CClightfoot|01D8                |0188| 0|
01F5|0417|CCBridge   |01EC 01ED           |0113| 0|
01F6|0418|CCdarkness |01D9                |012E| 0|
01F7|0419|CCseqobj   |0466                |0113| 0|
01F8|041A|CCqueen    |01D7                |0187| 0|
01F9|041B|CCclawdoor |01DA                |0110| 0|
01FA|014D|CCgasvent  |0467                |0185| 0|
01FB|FFFF|CCgasventCo|0466                |0186| 0|
01FC|01DB|CCboulder  |01DB                |00EF| 0|
01FD|FFFF|CCAnimCanno|03B6                |00C6| 0|
01FE|04DB|CCgoldbar  |01EE                |00ED| 0|
01FF|02AF|CCFoundGold|01EE                |0269| 0|
0200|006E|CCAnimgoldb|01EE                |00C6| 0|
0201|FFFF|CC_Pressure|020E                |00FB| 2|
0202|041E|WORLDplanet|0198                |01D2| 0|
0203|FFFF|WORLDcloudr|0199                |01D3| 0|
0204|FFFF|WORLDdragro|019A                |01D3| 0|
0205|FFFF|WORLDwalled|019B                |01D3| 0|
0206|FFFF|WORLDdarkic|019C                |01D3| 0|
0207|FFFF|WORLDcloudl|019D                |01D3| 0|
0208|FFFF|WORLDpath1 |019E                |01D3| 0|
0209|01D5|WORLDpath2 |019F                |01D3| 0|
020A|01D6|WORLDpath3 |01A0                |01D3| 0|
020B|01D7|WORLDpath4 |01A1                |01D3| 0|
020C|FFFF|WORLDpath5 |01A2                |01D3| 0|
020D|041F|WORLDpath6 |01A3                |01D3| 0|
020E|FFFF|WORLDpath7 |01A4                |01D3| 0|
020F|041D|WORLDpath8 |01A5                |01D3| 0|
0210|04E6|WORLDarwing|01A6                |01D3| 0|
0211|01D8|WORLDgreatf|01A7                |01D3| 0|
0212|01D9|WORLDsun   |01A8 01A9 01AA      |01D3| 0|
0213|0150|WORLDsunray|01AB                |01D3| 0|
0214|01DA|WORLDpepper|01BD 01BB 01BC 01BD |01D3| 0|
0215|0200|WORLDsunfla|01AC                |01D3| 0|
0216|FFFF|WORLDAstero|01B2 01B3 01B4 01B5…|01D4| 0|
0217|FFFF|WORLDComet |01B2 01B3 01B4 01B5…|01D3| 0|
0218|FFFF|WORLDastGen|0467                |01D3| 0|
0219|FFFF|WORLDhalo  |01AD                |01D3| 0|
021A|FFFF|WORLDshield|01AF                |012E| 0|
021B|FFFF|WORLDhalobr|01AE                |01D3| 0|
021C|FFFF|WORLDarrow |01B0 01B1           |01D3| 0|
021D|FFFF|WORLDcomm  |01CF                |00C6| 0|
021E|FFFF|communicato|01CD                |00C6| 0|communicator on pause screen
021F|0152|commCube   |01CE                |00C6| 0|pause menu background
0220|0157|commCubeFro|01CE                |00C6| 0|
0221|FFFF|commRobHead|01CA                |00C6| 0|pause menu heads/icons
0222|FFFF|commPepperH|01C9                |00C6| 0|
0223|0158|commPeppyHe|01CB                |00C6| 0|
0224|0153|commSlippyH|01CC                |00C6| 0|
0225|0154|commMemoryC|01C7                |00C6| 0|
0226|0155|commQuitObj|01C8                |00C6| 0|
0227|0156|GFRONT_Asse|045F                |012E| 0|
0228|014F|LINKH_Asset|045F                |012E| 0|
0229|FFFF|LINKC_Asset|045F                |012E| 0|
022A|FFFF|LINKE_Asset|045F                |012E| 0|
022B|0179|LINKJ_Asset|045F                |012E| 0|
022C|FFFF|LINKF_Asset|045F                |012E| 0|
022D|0398|ANDSeqObjec|0466                |0112| 0|
022E|026C|robotscorpi|04C5                |FFFF| 0|
022F|FFFF|firebaddie |04C6                |FFFF| 0|
0230|051B|Andross    |04C7                |02BC| 0|
0231|0420|Androssleft|04C8                |02BD| 0|
0232|FFFF|Androssrigh|04C9                |02BD| 0|
0233|FFFF|AndrossBrai|04CA                |02BE| 0|
0234|FFFF|AndrossLigh|04CA                |02BF| 0|
0235|FFFF|ANDAsteroid|04A2                |029B| 0|
0236|FFFF|ANDSuckAste|04A2                |029B| 0|
0237|FFFF|AndrossRing|04CB                |029B| 0|
0238|FFFF|ANDSilverRi|0498 0499           |02A0| 0|
0239|FFFF|AndrossSpir|0294                |012E| 0|
023A|FFFF|AnimARWArwi|0480                |00C6| 0|
023B|FFFF|AnimWhiteSp|04CE                |00C6| 0|
023C|FFFF|HazeSphere2|04CC                |00C6| 0|
023D|FFFF|AnimWhitePl|04CD                |00C6| 0|
023E|FFFF|AnimConcuss|0012                |00C6| 0|
023F|FFFF|WORLDanimpl|0198                |00C6| 0|
0240|FFFF|WORLDanimcl|0199                |00C6| 0|
0241|FFFF|WORLDanimdr|019A                |00C6| 0|
0242|FFFF|WORLDanimwa|019B                |00C6| 0|
0243|FFFF|WORLDanimda|019C                |00C6| 0|
0244|0005|WORLDanimcl|019D                |00C6| 0|
0245|FFFF|AnimFalco  |01C4                |00C6| 0|
0246|0151|Animpeppy  |01C1                |00C6| 0|
0247|FFFF|GF_PepperHe|01C9                |00C6| 0|
0248|FFFF|GF_planet  |01C5                |00C6| 0|
0249|FFFF|GF_LevelCon|01C2                |02BB| 0|slow fade in, starry sky
024A|FFFF|GF_sequence|0466                |0112| 0|
024B|0167|Animrob    |01C3                |00C6| 0|
024C|0071|AnimPilotse|01D6                |00C6| 0|
024D|0070|GF_Pilotsea|01D6                |012E| 0|
024E|FFFF|AnimPlanet |01C5                |00C6| 0|
024F|04BC|AnimGreatfo|01A7                |00C7| 0|
0250|016D|Animspanner|01C6                |00C6| 0|
0251|0039|GF_JukeBox |01BE                |012E| 0|
0252|FFFF|GF_DoorLeft|01BF                |0110| 0|
0253|015D|GF_DoorRigh|01C0                |00C6| 0|
0254|0006|AnimSpaceCr|01D0                |00C6| 0|
0255|FFFF|MMP_levelco|0467                |017E| 0|
0256|029E|MMPass_Info|0010                |0121| 0|
0257|017B|DIM_InfoTex|0010                |0121| 0|
0258|0516|MMP_MagicCa|0158                |0110| 0|
0259|04F6|MMP_WarpPoi|02D1                |00F0| 0|
025A|000A|MMP_CraterF|045F                |01E7| 0|
025B|FFFF|MMP_Organic|0166                |012A| 0|
025C|FFFF|MMP_barrel |0160                |017D| 0|
025D|0038|MMP_seqobje|0466                |0112| 0|
025E|FFFF|MMP_telesco|015F                |FFFF| 0|
025F|03B5|MMP_defenDo|0168                |012E| 0|
0260|FFFF|MMP_defenDo|0169                |012E| 0|
0261|FFFF|MMP_padlock|0163                |0111| 0|
0262|039A|MMP_asteroi|0159 015A           |0180| 0|
0263|04B0|MMP_trenchF|045F                |0181| 0|
0264|0399|MMP_asteroi|015E                |00C6| 0|
0265|0397|MMP_gyserve|045F                |0183| 0|
0266|FFFF|MMP_moonroc|015C                |0182| 0|
0267|FFFF|MMP_Krazoa |0282                |00C6| 0|
0268|039B|MMP_Bridge |042D 042E 042F      |010F| 0|
0269|FFFF|MMPAnimShar|001A                |0184| 0|
026A|FFFF|NWCellLock |03DD                |012E| 0|
026B|FFFF|NW_GeneralS|0052                |01F5| 0|
026C|02B5|NW_Portcull|013E 013F           |0110| 0|
026D|026A|NW_SmallSca|00D1                |00ED| 0|
026E|02A1|NW_IcePriso|00C5 00C6 00C7 00C8…|015A| 0|
026F|FFFF|NW_IcePriso|00C6 00C7 00C8 00C9…|0166| 0|
0270|FFFF|NWSteppingS|00C4                |0113| 0|
0271|0396|NWCallOFEld|00C3                |012F| 0|
0272|05BF|NW_levcontr|0466                |01A5| 0|falling snow
0273|0394|NW_seqobjec|0466                |0113| 0|
0274|01D3|NW_geyser  |00B9                |01A0| 0|
0275|0391|NW_AnimGeys|00BA                |00C6| 0|
0276|039C|NW_boulder |00BB                |00EF| 0|
0277|039D|NW_AnimBoul|00BB                |00C6| 0|
0278|02D1|NW_trickyba|00B5                |00ED| 0|
0279|FFFF|NW_alpinero|00C2                |00ED| 0|
027A|02D9|NW_alpinero|00C2                |00C6| 0|
027B|02D4|NW_treebrid|00BE 00BF           |019F| 0|
027C|02D5|NW_treebrid|00C0 00C1           |019F| 0|
027D|02D7|NW_mammothh|00B6                |01A1| 2|
027E|02D2|NW_mammothb|00B7                |01A1| 2|
027F|FFFF|AnimGoldRoo|0443                |00C6| 0|
0280|FFFF|NW_mammothw|00B6                |01A1| 0|
0281|FFFF|NW_mammothg|00B8                |01A1| 0|
0282|02D0|NW_guardian|00B6                |00C6| 0|
0283|FFFF|NW_animmamm|00B6                |00C6| 0|
0284|03B1|NW_tricky  |0467                |01A2| 0|
0285|048F|NW_animice1|03E9                |01A3| 0|
0286|0390|NW_animice2|03EC                |01A3| 0|
0287|038F|NW_animice3|03ED                |01A3| 0|
0288|02DE|NW_ice1    |03E9                |01A4| 0|
0289|02DF|NW_ice2    |03EC                |01A4| 0|
028A|FFFF|NW_ice3    |03ED                |01A4| 0|
028B|02D8|nw_testobj |02C6                |011A| 0|
028C|0395|NW_InfoText|0010                |0121| 0|
028D|0029|NW_InfoText|0010                |0121| 0|
028E|FFFF|WaterFlowWe|0101                |02AE| 0|
028F|FFFF|SH_YellowWh|00F1                |02AD| 0|
0290|0073|SH_BlueWhit|00F2                |02AD| 0|
0291|FFFF|SH_InfoPoin|0010                |0121| 0|sign "this way to cape claw" (invisible)
0292|02D3|SH_RedYello|00F3                |02AD| 0|
0293|0392|SH_BlueFlow|00F4                |02AD| 0|
0294|0393|SH_MagicCav|00F0                |0110| 0|large round door
0295|FFFF|SH_QueensDo|00EF                |0110| 0|
0296|FFFF|SH_Portcull|00E0 00DF           |0110| 0|rusty iron gate
0297|FFFF|SH_FernTree|00EE                |02AF| 0|
0298|FFFF|SH_AnimMap |00ED                |00C6| 0|map from shop (disappears after 1 frame)
0299|FFFF|SH_FireWeed|00EC                |0121| 0|
029A|0562|SH_BombWall|00E1 00E2 00E3 00E4…|015A| 0|
029B|01B5|SH_BombWall|00E2 00E3 00E4 00E5…|0166| 0|
029C|FFFF|SH_BombWall|00F8 00F9 00FA 00FB…|015A| 0|
029D|02B9|SH_BombWall|00F9 00FA 00FB 00FC…|0166| 0|
029E|FFFF|SH_MediumSc|00DD                |00ED| 0|
029F|FFFF|SH_LargeSca|00DE                |00ED| 0|
02A0|FFFF|SH_MMPkey  |0164                |00ED| 0|
02A1|00C7|SH_NWkey   |00D2                |00ED| 0|
02A2|00F7|SH_EmptyTum|0039                |01B4| 0|
02A3|FFFF|SH_BeaconTw|004B                |012E| 0|
02A4|00CA|SH_Beacon  |0144                |01B3| 0|
02A5|FFFF|SH_newseqob|0466                |0113| 0|
02A6|00F0|SH_BottomSe|0466                |0113| 0|
02A7|02BA|SH_krystalH|00F5                |00C6| 0|Krystal head when you pick up staff
02A8|FFFF|SH_LevelCon|0466                |01AE| 0|displays "ThornTail Hollow" on screen
02A9|FFFF|SH_staff   |007E                |01B1| 0|
02AA|FFFF|SH_staffHaz|004D                |01B2| 0|
02AB|0079|SH_tricky  |0467                |01A6| 0|
02AC|0184|SH_seqobjec|0466                |0112| 0|
02AD|FFFF|SH_queenear|00D3                |01AC| 0|
02AE|FFFF|SH_thorntai|00D4                |01AD| 1|
02AF|FFFF|SH_thorntai|00D4                |00C6| 0|
02B0|00AB|SH_whitemus|00D7                |01A7| 0|
02B1|FFFF|SH_killermu|00D5                |01A8| 0|
02B2|00C8|SH_whitemus|00D7                |00C6| 0|
02B3|FFFF|SH_swaplift|00DB                |01AF| 0|warpstone platform; stand on and press A to warp to him
02B4|00C9|SH_swapston|00DC                |01B0| 0|warpstone
02B5|FFFF|SH_Pressure|0240                |00FB| 2|
02B6|FFFF|SH_boulder |00FF                |00EF| 0|
02B7|038E|SH_AnimBoul|00FF                |00C6| 0|
02B8|FFFF|AnimSwapGif|0444                |00C6| 0|
02B9|0028|SHAnimBaby |0100                |00C6| 0|
02BA|FFFF|SHCloudBaby|0100                |014C| 0|a CloudRunner, the PDA even recognizes it
02BB|038D|KP_sequence|0466                |0112| 0|
02BC|038C|KP_red_orb |045F 045F 045F      |FFFF| 0|
02BD|038B|KP_red_gree|026D 026E 026F      |FFFF| 0|
02BE|FFFF|KP_red_blue|0270 0271 0272      |FFFF| 0|
02BF|FFFF|KP_red_yell|0273 0274 0275      |FFFF| 0|
02C0|04FE|KP_red_purp|0276 0277           |FFFF| 0|
02C1|01BC|KP_AnimKraz|0282                |00C6| 0|
02C2|FFFF|KP_RedKrazo|0283                |00C6| 0|
02C3|033E|KP_Transpor|045F                |012C| 0|
02C4|04E7|KP_console_|0278                |012E| 0|
02C5|025C|KP_console_|0279                |012E| 0|
02C6|FFFF|KP_console_|027B                |012E| 0|
02C7|FFFF|KP_console_|027A                |012E| 0|
02C8|FFFF|KP_textbloc|027C                |0239| 0|spams console: "<textblock.c Init>No Longer supported"
02C9|FFFF|KP_textbloc|027D                |0239| 0|
02CA|0025|KP_textbloc|027E                |0239| 0|
02CB|0051|KP_textbloc|027F                |0239| 0|
02CC|025D|KP_SectionD|0280                |0110| 0|
02CD|04E9|KP_FinDoor |0281                |0110| 0|
02CE|04EB|AnimKrystal|04E8                |00C6| 0|used when Fox finds her in Krazoa Palace
02CF|04ED|ScalesBelt |0051                |00C6| 0|
02D0|FFFF|SC_levelcon|0467                |01B6| 0|
02D1|FFFF|SC_sequence|0466                |0112| 0|
02D2|FFFF|SC_warppoin|02D1                |00F0| 0|
02D3|025F|SC_chieflig|0132                |01B5| 0|
02D4|0260|SC_lightfoo|0133                |01B5| 0|
02D5|0261|SC_babyligh|0134                |01B5| 0|
02D6|FFFF|SC_blTarget|045F                |01B5| 0|
02D7|0582|SC_animligh|0133                |00C6| 0|
02D8|FFFF|SC_animchie|0132                |00C6| 0|
02D9|FFFF|SC_animbaby|0134                |00C6| 0|
02DA|FFFF|SC_muscleli|0135                |01B5| 0|
02DB|FFFF|SC_animmusc|0135                |00C6| 0|
02DC|FFFF|SC_Pressure|0139                |00FB| 0|
02DD|FFFF|SC_Shrine_d|013E 013F           |0110| 0|
02DE|04AA|SC_mounddoo|013C                |0110| 0|
02DF|0427|SC_settelme|013E 013F           |0110| 0|
02E0|FFFF|BossTRexPor|0140 0141           |0110| 0|
02E1|042A|SC_capeclaw|0142                |0110| 0|
02E2|038A|SC_paypoint|0157                |01BD| 0|
02E3|FFFF|SC_stepping|0147                |0110| 0|
02E4|FFFF|SC_hutdoor |013D                |0110| 0|
02E5|FFFF|SC_block_tr|0136                |00ED| 0|
02E6|FFFF|SC_block_sq|0137                |00ED| 0|
02E7|FFFF|SC_block_ci|0138                |00ED| 0|
02E8|FFFF|SC_blockpla|0136 0137 0138      |0111| 0|
02E9|FFFF|SC_totempol|0148                |01B8| 0|
02EA|FFFF|SC_totempuz|014F 0150 0151 0152…|01BA| 0|
02EB|FFFF|SC_totembon|0146                |01BB| 0|
02EC|FFFF|SC_MusicTre|014B                |01B7| 0|
02ED|FFFF|MMP_Key    |0164                |00C6| 0|
02EE|02AB|SC_DummyTri|0136                |011A| 0|
02EF|FFFF|SC_DummySqu|0137                |011A| 0|
02F0|FFFF|SC_DummyCir|0138                |011A| 0|
02F1|FFFF|SC_totemstr|013A                |01BC| 0|
02F2|01BA|SC_throne  |013B                |011A| 0|
02F3|FFFF|SC_BirchTre|014A                |01B7| 0|
02F4|FFFF|SC_Cloudrun|000E                |01B9| 0|
02F5|01F9|SB_Galleon |0097 009B           |01E8| 0|
02F6|FFFF|SB_Key     |0098                |00ED| 0|
02F7|FFFF|SB_Wheel   |0099                |01F6| 0|
02F8|02B6|SB_Spellsto|02F2                |00ED| 0|
02F9|02B7|SB_Cloudrun|000E                |0259| 0|
02FA|01DC|SB_FireBall|00AC                |01F5| 0|
02FB|01C6|SB_Propelle|009D                |01E9| 0|
02FC|0065|SB_Propelle|009E 009F 00A0 00A1 |01E9| 0|
02FD|0066|SB_ShipHead|00A4                |01EA| 0|
02FE|FFFF|SB_ShipMast|00A2                |01EB| 0|
02FF|033D|SB_ShipGun |00A6                |01EC| 0|
0300|FFFF|SB_ShipGunB|00A7                |01F7| 0|
0301|01C7|SB_CannonBa|00AD                |01EE| 0|
0302|01C9|SB_FireBall|00AD                |01ED| 0|
0303|FFFF|SB_MiniFire|00AC                |01F3| 0|
0304|01C8|SB_CloudBal|00A8                |01EF| 0|
0305|FFFF|SB_Sail    |00A3                |01F6| 0|
0306|FFFF|SB_KyteCage|00AE                |01F0| 0|
0307|FFFF|SB_SeqDoor |00B3 00B4           |01F1| 0|
0308|FFFF|SB_SeqDoor2|00B3 00B4           |01F1| 0|
0309|FFFF|SB_DeckDoor|00B1                |01F1| 0|
030A|0050|SB_CageKyte|000A                |01F2| 0|
030B|025E|SB_Lamp    |00AF                |01F4| 0|
030C|FFFF|SB_ShipFlag|009C                |01F6| 0|
030D|FFFF|LevelName  |0468                |00F8| 0|
030E|FFFF|CNTcounter |0468                |02B4| 0|
030F|0164|CNTstopwatc|0468                |02B5| 0|
0310|FFFF|CNThitObjec|0468                |02B6| 0|
0311|0018|CNTColideOb|0468                |02B6| 0|
0312|017F|CameraColli|0468                |FFFF| 0|invisible solid object
0313|0313|VFP_InfoTex|0010                |0121| 0|sign
0314|FFFF|VFP_Warding|0213                |012E| 0|
0315|FFFF|VFP_newball|01F2 01F3 01F4      |012A| 0|
0316|FFFF|VFP_liftgra|0212                |012A| 0|
0317|FFFF|VFP_lavasta|045F                |0227| 0|
0318|FFFF|VFP_lavapoo|0210                |0226| 0|
0319|016C|VFP_flamepo|045F                |0225| 0|
031A|017E|VFPSpPl    |045F                |0228| 0|
031B|0273|VFP_Block2 |01F1                |00EF| 0|
031C|04C4|VFP_PowerBo|0469                |012A| 0|
031D|FFFF|VFP_Ladders|01FD                |021C| 0|
031E|FFFF|VFP_PitSwit|020F                |0111| 0|
031F|0284|VFP_PuzzleP|020E                |00FB| 2|
0320|FFFF|VFP_PodiumP|0467                |0111| 0|
0321|FFFF|VFP_corepla|020A                |0223| 0|
0322|FFFF|VFP_mocklaz|0211                |00C6| 0|
0323|FFFF|VFP_corepla|0209                |0223| 0|
0324|FFFF|VFP_corepla|0209                |00C6| 0|
0325|FFFF|VFP_SmallRo|0202                |00F4| 0|
0326|FFFF|VFPDragHead|0207                |0222| 0|
0327|FFFF|VFPLavaBloc|0206                |021F| 0|
0328|FFFF|VFPLift1   |01FE                |021D| 0|
0329|FFFF|VFPLift2   |0205                |021D| 0|
032A|FFFF|VFPLift3   |01FE                |021D| 0|
032B|FFFF|VFP_Spellst|02F2                |012A| 0|
032C|FFFF|VFP_seqpoin|0466                |0221| 0|
032D|FFFF|VFPSeqObj  |0466                |0112| 0|
032E|FFFF|VFP_DoorSwi|0204                |0220| 0|
032F|FFFF|VFP_LiftInd|020B 020C           |0220| 0|
0330|FFFF|VFP_RoundDo|0202                |00F4| 0|
0331|FFFF|VFP_Platfor|0201                |021F| 0|
0332|00D5|VFP_Platfor|04E4                |021F| 0|
0333|FFFF|VFP_Block1 |0467                |021E| 0|
0334|0165|VFP_LiftSwi|0203                |0111| 0|
0335|FFFF|VFP_Ladders|01FD                |021C| 0|
0336|FFFF|VFP_Maindoo|01F5                |012E| 0|
0337|0026|VFP_Doorloc|01FA                |0110| 0|
0338|0168|VFP_Maindoo|01F5                |012E| 0|
0339|FFFF|VFP_Door2  |01F6                |00F4| 0|
033A|FFFF|VFP_Door2B |01F6                |00C6| 0|
033B|02EB|VFP_statueb|01F9 01F7 01F8      |021A| 0|
033C|FFFF|VFP_MiniFir|01FB                |0218| 0|
033D|0389|VFP_LevelCo|0467                |0216| 0|changes music, applies heat effect
033E|0385|VFP_ObjCrea|0469                |0217| 0|
033F|0386|VFP_locksym|029B                |012A| 0|
0340|0387|VFP_Bobbing|01EF                |0112| 0|
0341|0388|VFP_Bobbing|01F0                |0112| 0|
0342|FFFF|LINKA_levco|045F                |0238| 0|spawns an object and puts you in an endless animation loop with weird music
0343|FFFF|OFPclawOnly|0214                |0110| 0|
0344|0384|DFP_PowerSl|0000                |023C| 0|
0345|FFFF|DFP_Lightni|0000                |023B| 0|
0346|FFFF|DFPSpSG    |0219                |012A| 0|
0347|FFFF|DFP_WaterHi|045F                |012A| 0|
0348|FFFF|DFP_FireHit|045F                |012A| 0|
0349|FFFF|DFP_InnerRi|0238                |012A| 0|
034A|0072|DFP_OuterRi|0239                |012A| 0|
034B|FFFF|DFP_PlacePl|023A                |012A| 0|
034C|026B|DFP_Water  |0237                |012A| 0|
034D|FFFF|DFP_Bridge |0236                |0110| 0|
034E|FFFF|VFP_SpellPl|045F                |0228| 0|
034F|FFFF|DFP_SmallBr|0235                |00F4| 0|
0350|FFFF|DFP_ToLinkD|021A                |00F4| 0|
0351|FFFF|DFP_PowerBo|0469                |012A| 0|
0352|FFFF|DFP_transla|0233                |00C6| 0|
0353|FFFF|DFP_Platfor|0234                |023A| 0|
0354|05A8|DFPSpPl    |045F                |0237| 0|
0355|FFFF|DFP_LaserBe|045F                |0236| 0|
0356|FFFF|DFP_blockwa|022C 022D 022E      |012A| 0|
0357|FFFF|DFP_TargetB|022B                |0235| 0|
0358|FFFF|DFP_TargetB|022A                |0235| 0|
0359|FFFF|DFP_PerchSw|0228                |0234| 0|
035A|0428|DFP_PerchBa|0227                |00F4| 0|
035B|FFFF|DFP_Statue1|0226                |0233| 0|
035C|FFFF|DFP_RotateP|0225                |0232| 0|
035D|FFFF|DFP_BigDoor|0221                |00F4| 0|
035E|05AA|DFP_BigDoor|0221                |00C6| 0|
035F|FFFF|DFP_SmallDo|0222                |00F4| 0|
0360|FFFF|DFP_SmallDo|0231                |00F4| 0|
0361|00C1|DFP_PitSwit|021F                |0111| 0|
0362|FFFF|DFP_ForceSw|0229                |0111| 0|
0363|00F9|DFP_ForceAw|0469                |0231| 0|
0364|FFFF|DFP_dish   |0223                |012A| 0|
0365|FFFF|DFP_PuzzleP|021E                |01FE| 2|
0366|FFFF|DFP_floorba|0218                |022F| 0|
0367|FFFF|DFP_wallbar|021B 021C 021D      |0230| 0|
0368|FFFF|DFP_Torch  |02DC                |022B| 0|
0369|003C|DFPSplstneA|02F3                |00C6| 0|
036A|0106|DFPSplstne |02F3                |012E| 0|
036B|03F2|VFPSplstneA|02F2                |00C6| 0|
036C|00EF|VFPSplstne |02F2                |012E| 0|
036D|0509|DFP_seqpoin|0466                |022D| 0|
036E|FFFF|DFP_DoorSwi|0220                |022E| 0|
036F|FFFF|DFPSpDA    |023B                |022E| 0|
0370|FFFF|DFPSpDB    |023C                |012A| 0|
0371|005E|DFP_Textblo|0232                |0239| 0|"no longer supported"
0372|FFFF|DFP_RoundDo|0215                |0110| 0|
0373|FFFF|DFP_SmallTu|022F                |00F4| 0|
0374|FFFF|DFP_LevelCo|0467                |0229| 0|sets music
0375|FFFF|DFP_ObjCrea|045F                |022A| 0|
0376|FFFF|WM_dummyfox|0001                |012A| 0|
0377|05A9|WM_MediumSc|00DD                |00ED| 0|
0378|FFFF|WM_krazoast|023D                |012A| 0|
0379|FFFF|WM_krazoast|023E                |012A| 0|
037A|03A9|WM_newcryst|0267                |0215| 0|
037B|03AA|WM_newcryst|0266                |0215| 0|
037C|03AB|WM_newcryst|0268                |0215| 0|
037D|03AC|WM_padlock |0163                |0111| 0|
037E|FFFF|WM_Explosiv|0305                |012E| 0|
037F|FFFF|WM_drape   |023F                |02AD| 0|
0380|FFFF|WM_Cloudrun|000E                |00C6| 0|
0381|FFFF|WM_TransTop|0269                |0214| 0|
0382|FFFF|WM_PlanetsS|045F                |0210| 0|
0383|FFFF|WM_Planets |045F                |0210| 0|
0384|04A4|WM_BossDrak|0000                |00C6| 0|
0385|FFFF|WM_Krazoa1 |0282                |00C6| 0|
0386|FFFF|WM_Krazoa2 |0282                |00C6| 0|
0387|FFFF|WM_thorntai|00D4                |00C6| 0|
0388|FFFF|WM_thorntai|000E                |00C6| 0|
0389|010A|WM_Spellboo|0000                |00C6| 0|
038A|010B|WM_BossDrak|0000                |00C6| 0|
038B|0108|WM_sun     |0259 025A 025B      |020E| 0|
038C|010D|WM_rock    |0258 0258           |012A| 0|
038D|0110|WM_fallenco|0256                |012A| 0|
038E|0111|WM_largeroc|0255                |012A| 0|
038F|02E9|WM_consoles|02F2                |00ED| 0|
0390|0115|WM_5pushcra|02C7                |00EF| 0|
0391|014B|WM_WallCraw|0024                |0211| 0|
0392|0113|WM_Walkway1|0252                |012A| 0|
0393|02F3|WM_Walkway2|0253                |012A| 0|
0394|FFFF|WM_NoPassVi|0250                |012A| 0|
0395|FFFF|WM_NoPassHo|0251                |012A| 0|
0396|FFFF|WM_MoonSeed|0044                |012A| 0|
0397|FFFF|WM_WarpPoin|02D1                |00F0| 0|
0398|FFFF|WM_Column_l|0246                |012A| 0|
0399|FFFF|WM_SpiritSe|0294                |020F| 0|
039A|FFFF|WM_Crystal |025D                |020E| 0|
039B|FFFF|WM_Planet  |045F                |012A| 0|
039C|02F1|WM_PlanetMo|045F                |012A| 0|
039D|0054|WM_VConsole|024F                |0213| 0|
039E|FFFF|WM_seqobjec|0466                |01FA| 0|
039F|FFFF|WM_Galleon |0097                |01F8| 0|
03A0|033C|WM_ObjCreat|0469                |01F9| 0|
03A1|05BE|WM_GeneralS|0050                |020A| 0|
03A2|05C0|WM_deaddino|000B                |028A| 1|
03A3|05C1|WM_LaserBea|045F                |01FC| 0|
03A4|FFFF|WM_Pressure|0240                |01FE| 2|
03A5|FFFF|WM_Door1   |0241                |0110| 0|
03A6|FFFF|WM_Door2   |0242                |00C6| 0|
03A7|0336|WM_Door3   |0243                |00C6| 0|
03A8|0339|WM_Door4   |0244                |00C6| 0|
03A9|033B|WM_PlanDoor|0261                |00F4| 0|
03AA|0338|WM_PlanDoor|0262                |00C6| 0|
03AB|033A|WM_PlanDoor|0263                |00C6| 0|
03AC|0337|WM_PlanDoor|0264                |00C6| 0|
03AD|FFFF|WM_Column  |0245                |0116| 1|
03AE|FFFF|WM_Column_T|0246                |0116| 3|
03AF|FFFF|WM_LaserTar|031B 031C           |01FD| 0|
03B0|0335|WM_WarpDoor|0247                |00F4| 0|
03B1|043D|WM_Animcolr|0248                |00C6| 1|
03B2|0478|WM_colrise |0248                |0201| 1|
03B3|FFFF|WM_spiritpl|025E                |020C| 0|
03B4|0438|WM_Animspir|025E                |00C6| 0|
03B5|FFFF|WM_seqpoint|0466                |020D| 0|
03B6|0445|WMFinalDoor|0257                |0110| 0|
03B7|0328|WM_Torch   |046A                |0204| 0|
03B8|0334|WM_Vein    |046A                |0205| 0|
03B9|0333|WM_WallTorc|024B                |0206| 0|
03BA|0331|WM_Worm    |046A                |0207| 0|
03BB|0330|WM_Wallpowe|045F                |0208| 0|
03BC|032E|WM_LevelCon|0467                |0209| 0|sets music/rain SFX, gives everything green tint
03BD|032C|SPitembeam |043E                |0289| 0|
03BE|036B|SPDrape    |0438                |0288| 0|
03BF|0329|SPScarab   |02B1 02B0           |0287| 0|
03C0|0327|SPSeqObj   |0466                |0112| 0|
03C1|02EA|SPDoor     |0437                |0110| 0|
03C2|0326|SPShop     |0467                |0285| 0|
03C3|FFFF|SPShopKeepe|0436                |0286| 0|
03C4|FFFF|SPFruitSmal|02BB                |0284| 0|spawning shop items outside the shop gives "pick up" but you can't. inside, you can buy them
03C5|00F5|SPEggSmall |02B9                |0284| 0|
03C6|0325|SPFruitLarg|043F                |0284| 0|
03C7|FFFF|SPEggLarge |0440                |0284| 0|
03C8|0323|SPBombSpore|0447                |0284| 0|
03C9|FFFF|SPMoonSeed |0161                |0284| 0|
03CA|02EC|SPLantern  |037F                |0284| 0|
03CB|0321|SPBlueArtef|0443                |0284| 0|
03CC|FFFF|SPBlueMushr|00D6                |0284| 0|
03CD|04E3|SPSwapGift |0444                |0284| 0|
03CE|0074|SPPda      |0445                |0284| 0|Would have been buyable PDA; appears as placeholder cube
03CF|04F7|SPBinocular|0446                |0284| 0|
03D0|014A|SPFireFly  |02EF                |0284| 0|
03D1|FFFF|SPFuelCell |00F6                |0284| 0|
03D2|FFFF|SPSidekickB|00B5                |0284| 0|
03D3|04EF|SPDusterHol|0441                |0284| 0|
03D4|04F0|SPStaffHitB|0442                |0284| 0|
03D5|04F1|SPMapTTH   |0439                |0284| 0|
03D6|04F2|SPMapMMP   |0439                |0284| 0|
03D7|FFFF|SPMapLF    |0439                |0284| 0|
03D8|0089|SPMapCRF   |0439                |0284| 0|
03D9|008A|SPMapDIM   |0439                |0284| 0|
03DA|008B|SPMapWC    |0439                |0284| 0|
03DB|008C|SPMapDR    |0439                |0284| 0|
03DC|008D|SPMapKP    |0439                |0284| 0|
03DD|008E|SPMapOFP   |0439                |0284| 0|
03DE|04F8|SPMapSW    |0439                |0284| 0|
03DF|05B8|SPMapVFP   |0439                |0284| 0|
03E0|008F|SPMapCC    |0439                |0284| 0|
03E1|FFFF|SPReplayDis|0467                |0284| 0|"Level control" object, draws fuel cell?
03E2|FFFF|SPWell     |0467                |01BD| 0|
03E3|0090|GM_TokenPic|0448                |00ED| 0|
03E4|0107|FElevContro|0467                |0142| 0|
03E5|0091|FEseqobject|0466                |0143| 0|
03E6|0278|Rarelogo   |02D3                |00C6| 0|
03E7|032F|N64logo    |02D1                |00C6| 0|
03E8|0092|FrontFox   |01D1                |02C0| 2|title screen Fox - steals camera focus
03E9|0494|FrontPeppy |01D2                |02C0| 0|
03EA|FFFF|FrontSlippy|01D3                |02C0| 0|
03EB|0489|FrontRob   |01D4                |02C0| 0|
03EC|FFFF|FrontFalco |01C4                |02C0| 0|
03ED|00B4|FrontPilots|01D6                |02C0| 0|pilot seat?
03EE|0401|FrontPilots|01D5                |02C0| 0|
03EF|FFFF|FrontPlanet|01C5                |02C0| 0|
03F0|050D|FrontSeq   |0466                |0112| 0|
03F1|FFFF|DFSH_seqpoi|0466                |0221| 0|
03F2|FFFF|DFSH_LaserB|045F                |017B| 0|
03F3|040C|DFSHcol    |0292                |012A| 0|
03F4|0093|DFSH_Shrine|0294                |0178| 0|
03F5|FFFF|DFSH_Spirit|0294                |02AD| 0|
03F6|0076|DFSH_ObjCre|045F                |0179| 0|
03F7|FFFF|DFSH_Spirit|0294                |00C6| 0|
03F8|FFFF|DFSH_Door1 |028A                |00F4| 0|
03F9|FFFF|DFSH_Door2 |028B                |00C6| 0|
03FA|FFFF|DFSH_Door3 |028C                |00C6| 0|
03FB|0055|DFSH_Door4 |028D                |00C6| 0|
03FC|FFFF|MMSH_seqpoi|0466                |0221| 0|
03FD|002A|MMSH_Shrine|0294                |018C| 0|
03FE|002F|MMSH_Scales|0050                |018D| 0|
03FF|02DB|SpiritPrize|0294                |017A| 0|
0400|FFFF|scalessword|0090                |012A| 0|
0401|FFFF|MMSH_WaterS|045F                |018E| 0|
0402|FFFF|AnimShadowH|0040                |00C6| 0|
0403|FFFF|AnimShadowC|001C                |00C6| 0|
0404|FFFF|ECSH_seqpoi|0466                |0221| 0|
0405|FFFF|ECSH_Shrine|0294                |018F| 0|
0406|FFFF|SpiritPrize|0294                |017A| 0|
0407|FFFF|ECSH_Spirit|0294                |017A| 0|
0408|FFFF|ECSH_Cup   |02A3                |0190| 0|
0409|FFFF|EC_Shrine_d|013E 013F           |0110| 0|
040A|0058|ECSH_LaserB|045F                |01FC| 0|
040B|0059|ECSH_Pressu|0240                |01FE| 2|
040C|FFFF|ECSH_Creato|0469                |0191| 0|
040D|FFFF|ECSHcol    |02A4                |012A| 0|
040E|FFFF|ECSH_Door1 |029F                |00F4| 0|
040F|FFFF|ECSH_Door2 |02A0                |00C6| 0|
0410|FFFF|ECSH_Door3 |02A1                |00C6| 0|
0411|FFFF|ECSH_Door4 |02A2                |00C6| 0|
0412|FFFF|GPSH_seqpoi|0466                |0221| 0|
0413|FFFF|GPSH_Shrine|0294                |0192| 0|
0414|0498|GPSH_ObjCre|045F                |0193| 0|
0415|FFFF|GPSH_Scene |045F                |0194| 0|
0416|0431|GPSHpickobj|02A5                |0116| 3|
0417|043F|GPSHpickobj|02A6                |0116| 3|
0418|0442|GPSHpickobj|02A7                |0116| 3|
0419|0433|GPSHpickobj|02A8                |0116| 3|
041A|FFFF|GPSHpickobj|02A9                |0116| 3|
041B|FFFF|GPSHpickobj|02AA                |0116| 3|
041C|FFFF|DBSH_seqpoi|0466                |0221| 0|
041D|FFFF|DBSH_PushCl|0016                |00C6| 0|
041E|FFFF|DBSH_Shrine|0294                |0195| 0|
041F|FFFF|DBSH_Symbol|02AB                |0196| 0|
0420|FFFF|NWSH_Seqobj|0466                |0112| 0|
0421|FFFF|NWSH_levcon|0467                |0198| 0|purple haze, chanting
0422|FFFF|Animscaless|007D                |00C6| 0|
0423|FFFF|DBprotectZo|0471                |012E| 0|
0424|FFFF|DBHoleContr|0471                |0243| 0|
0425|FFFF|DBstealerwo|0022                |0242| 0|
0426|FFFF|DBAnimThief|0022                |00C6| 0|
0427|003D|DB_egg     |0353                |023F|12|
0428|0443|DB_AnimEgg |0353                |00C6| 0|
0429|0444|DBAttractor|0466                |015F| 0|
042A|0436|DBPointMum |045F                |023D| 0|
042B|0369|KillerSpike|0363                |012E| 0|
042C|036D|DRProjectil|02C8                |00F9| 0|
042D|036F|DR_EarthSad|0355                |FFFF| 0|
042E|0372|DR_EarthDoo|0387                |0110| 0|
042F|FFFF|DR_BarrelGr|0360                |0283| 0|
0430|FFFF|DR_AnimEart|000D                |00C6| 0|
0431|0374|DR_EarthWar|000D                |0257| 0|
0432|0375|DRearthwalk|045F                |0111| 0|
0433|FFFF|DR_CloudRun|000E                |0258| 0|
0434|FFFF|DR_HighTop |0094                |0272| 0|
0435|0448|AnimHightop|0094                |00C6| 0|
0436|048B|GM_MazeWell|0467                |0263| 0|
0437|044A|GM_Token   |0448                |00C6| 0|
0438|0474|DR_LiftLeve|0325                |00C6| 0|
0439|FFFF|DR_Shield  |0385                |0110| 0|
043A|FFFF|DR_TowerSwi|0386                |02B6| 0|
043B|049A|DR_ShieldIn|0385                |00C6| 0|
043C|049B|DR_Tower   |0388                |012E| 0|
043D|02ED|DR_Platform|02D7                |00EF| 0|
043E|044E|DR_TestWall|03A3                |012E| 0|
043F|0450|DR_LaserCan|036A                |0261| 0|
0440|04C5|DR_Generato|036C                |026F| 0|
0441|044F|DR_WallGene|037E                |026F| 0|
0442|0368|DR_PushCart|0365                |0271| 0|
0443|04CB|DR_IonCanno|0375                |012E| 0|
0444|04E5|DRbullet   |04D7 04D6           |0262| 0|
0445|05BA|DR_Seqobj  |0466                |0112| 0|
0446|04E2|FireHoleCon|0466                |0112| 0|
0447|FFFF|DR_SeqobjBo|0466                |0112| 0|
0448|FFFF|DRBlastedWa|0376                |0159| 0|
0449|FFFF|DR_AnimColl|0356                |00C6| 0|
044A|FFFF|DRExplodePi|0377 0378 0379 037A…|0166| 0|
044B|FFFF|ExplodePlan|0170 0171 0172 0173…|0166| 0|
044C|FFFF|DRSmallExpl|0377 0378 0379 037A…|015A| 0|
044D|02DD|DRSmallExpl|0377 0378 0379 037A…|0166| 0|
044E|FFFF|DR_BigSkeet|0026                |0211| 0|
044F|FFFF|DR_SmallSke|0027                |0211| 0|
0450|FFFF|DR_Creator |045F                |0265| 0|
0451|04F3|DRHomingMis|04AA                |00DA| 0|
0452|04F4|DR_Vines   |0380                |026D| 0|
0453|04F5|DR_ExplodeD|0366                |0269| 0|
0454|FFFF|DR_Geezer  |045F                |026A| 0|
0455|FFFF|DR_Chimmey |045F                |026B| 0|
0456|FFFF|DR_CageCont|045F                |0268| 0|
0457|0033|DR_CageWith|0381                |026C| 0|
0458|0034|DR_CageNoRo|0382                |026C| 0|
0459|FFFF|DR_CageRope|0383                |026C| 0|
045A|FFFF|DR_CloudDoo|0373                |0110| 0|
045B|FFFF|DR_CloudDoo|0374                |00C6| 0|
045C|03C2|DRPerch    |031F                |FFFF| 4|
045D|03C3|DR_Shackle |0384                |026E| 0|
045E|03C4|DR_BigShack|0389                |026E| 0|
045F|03C5|DR_Rock    |038A                |0270| 0|
0460|03C6|DR_HighDoor|0370                |0110| 0|
0461|03C7|DR_HighDoor|0371                |00C6| 0|
0462|03C8|DR_James1Bo|035A                |0110| 0|
0463|03C9|DR_James2Bo|035B                |00C6| 0|
0464|03CA|FirePipe   |03A1                |0273| 0|
0465|03CB|ProximityMi|0309                |0260| 0|
0466|03CC|FireHole   |0362                |0273| 0|
0467|03D0|FlameMuzzle|036B                |0273| 0|
0468|03D1|IceHole    |0362                |0273| 0|
0469|03D5|SteamHoleNo|0362                |0273| 0|
046A|03E1|SteamHoleFi|0362                |0273| 0|
046B|0452|SteamHoleDe|0362                |0273| 0|
046C|0454|DR_pulley  |016C                |0274| 0|
046D|0453|DR_cradle  |016B                |0275| 0|
046E|FFFF|DR_TrackDoo|038E                |0112| 0|
046F|FFFF|DR_TrackDoo|038F                |012E| 0|
0470|0455|DR_DrakorDo|0372                |0110| 0|
0471|0456|DRCollapseP|037D                |0278| 0|
0472|0457|DRPlatformC|0393                |0278| 0|
0473|045A|DRPlatformC|0394                |0278| 0|
0474|045B|DR_EnergyDi|035F                |0279| 0|
0475|FFFF|DR_Collapse|0467                |027A| 0|
0476|03E2|DR_CaveIn  |0395 0396           |027B| 0|
0477|045C|DR_CaveInFl|0397 0398           |027B| 0|
0478|FFFF|DR_LightBea|0399                |027C| 0|
0479|0499|DR_LandSqua|038C                |00C6| 0|
047A|04BD|DRMusicCont|0467                |027E| 0|
047B|0052|DR_LightPol|039B                |0131| 0|
047C|045D|DR_LightHal|0467                |027F| 0|
047D|045E|DR_LightPol|039A                |027F| 0|
047E|03C1|DR_LightLam|00AF                |027F| 0|
047F|03BF|DR_LightLam|039C                |027F| 0|
0480|FFFF|DR_CloudPer|039D                |0280| 0|
0481|03C0|DR_EarthCal|039E                |0281| 0|
0482|0053|DR_CloudCal|03A0                |012E| 0|
0483|03BE|DieSeqObjec|0466                |012E| 0|
0484|0434|DieDuster  |02C6                |010E| 0|everything else goes black, game crashes
0485|FFFF|DieFox     |0001                |010E| 0|Fox death animation - you can still walk around
0486|045F|DieKrystal |04E8                |010E| 0|Krystal death animation
0487|0366|AnimFox    |0001                |00C6| 0|
0488|0367|AnimFoxLink|0001                |00C6| 0|
0489|0460|SB_Cloudrun|000E                |00C6| 0|
048A|0461|CampFire   |02DA                |0129| 0|
048B|FFFF|ExplodeWall|0377 0378 0379 037A…|015A| 0|
048C|0365|ExplodePlan|0170 0171 0172 0173…|015A| 0|
048D|0363|ExplodePlan|016E 016F           |0269| 0|
048E|02A8|Fall_Ladder|02BC 02BD 02BE 02BF |010A| 0|
048F|FFFF|sfxPlayer  |046A                |0133| 0|
0490|0361|InvisibleHi|031B                |00FA| 0|
0491|036E|curve      |0465                |0125| 0|debug object?
0492|0364|checkpoint4|0458                |00E8| 0|
0493|035D|setuppoint |02D9                |00E9| 0|
0494|035E|EndObject  |02D9                |0108| 0|
0495|035F|sideload   |02D9                |00EA| 0|
0496|FFFF|siderepel  |0010                |00EB| 0|
0497|035B|siderepelWi|0010                |00EB| 0|
0498|035C|PortalSpell|0364                |010D| 0|
0499|0355|PortalDoorA|0364                |00C6| 0|
049A|FFFF|FireFlyLant|037F                |010B| 0|
049B|0359|LanternFire|02EF                |010C| 0|
049C|035A|MagicCaveTo|04CF                |011F| 0|staff glows when nearby
049D|0362|MagicCaveBo|0466                |011E| 0|dark effect; press A to do an animation and crash
049E|FFFF|MCProjDoor |04D0 04D1           |0110| 0|
049F|04F9|MCUpgrade  |0466                |02B7| 0|
04A0|FFFF|MCUpgradeMa|0466                |02B8| 0|
04A1|046C|MCStaffEffe|0472                |02B9| 0|
04A2|046E|MCLightning|0472                |02BA| 0|
04A3|046F|Lightning  |0472                |0141| 0|
04A4|0464|PerchObject|046B                |FFFF| 0|
04A5|0430|KPLandingPa|0466                |0112| 0|
04A6|029C|CF_BobbingC|017A                |0112| 0|
04A7|046D|CFLandingPa|0466                |0112| 0|
04A8|04B9|LandingPad_|0466                |0112| 0|
04A9|FFFF|WC_LandingP|0466                |0112| 0|
04AA|02B3|SH_NT_Landi|0466                |0112| 0|
04AB|FFFF|Arwingsteps|04C3                |00C6| 0|
04AC|0040|Landed_Arwi|0480                |011B| 0|Arwing on ground; can use it
04AD|0024|Landed_Arwi|0480                |011B| 0|can't be used
04AE|03BD|texscroll  |046B                |0135| 0|
04AF|FFFF|texscroll2 |046B                |0134| 0|
04B0|FFFF|LFXEmitter |0479                |012D| 0|
04B1|029D|Transporter|045F                |012C| 0|warp pad (won't let you warp)
04B2|050C|WarpPoint  |02D1                |00F0| 0|
04B3|FFFF|RestartPoin|046C                |00F0| 0|
04B4|0471|CFDungeonBl|02D7                |00EF| 0|
04B5|0358|InvHit     |046A                |00F1| 0|
04B6|FFFF|iceblast   |046C                |00F2| 0|
04B7|0472|flameblast |046C                |00F3| 0|makes Tricky spew fire
04B8|0473|WaveAnimato|046D                |0136| 0|
04B9|002B|TexFrameAni|046D                |013F| 0|
04BA|0056|AlphaAnimat|046E                |0137| 0|
04BB|005A|GroundAnima|0471                |0138| 0|
04BC|0475|HitAnimator|0478                |0139| 0|
04BD|FFFF|VisAnimator|046F                |013A| 0|
04BE|002C|WallAnimato|0471                |013B| 0|makes Tricky think there's something near
04BF|0356|XYZAnimator|0471                |013C| 0|
04C0|0476|ExplodeAnim|0471                |013D| 0|
04C1|0057|PadlockKey |02D4                |00ED| 0|
04C2|FFFF|GuardPass  |02D1                |00ED| 0|
04C3|0354|Spellstone |02F2                |00ED| 0|
04C4|FFFF|TrickyWarp |02D9                |0100| 0|
04C5|FFFF|TrickyGuard|045F                |0101| 0|
04C6|FFFF|TrickyGuard|045F                |0120| 0|
04C7|0373|AnimCamera |045E                |00C6| 0|
04C8|0479|Override   |0476                |00C6| 0|
04C9|FFFF|DepthOfFiel|0476                |00C8| 0|
04CA|FFFF|DepthOfFiel|0476                |00C6| 0|
04CB|FFFF|VariableObj|045F                |00C6| 0|
04CC|FFFF|ExplodingWa|02CC                |00C6| 0|
04CD|FFFF|hologrambas|044B                |00C6| 0|
04CE|FFFF|hologramhaz|044C                |00C6| 0|
04CF|0490|hazesphere |044E                |00C6| 0|
04D0|FFFF|RipplePlain|044D                |00C6| 0|
04D1|FFFF|EffectBox  |02D5                |00EE| 0|
04D2|047A|xyzpoint   |0464                |FFFF| 0|
04D3|010C|StaticCamer|045E                |025A| 0|
04D4|0112|WalkCamera |045E                |FFFF| 0|
04D5|047B|Area       |0477                |00F6| 0|
04D6|047C|DigHole    |0471                |FFFF| 0|
04D7|003A|DigTunnel  |045F                |FFFF| 0|
04D8|FFFF|GroundQuake|046A                |FFFF| 0|a blue cube with FX written on it
04D9|FFFF|SidekickBal|00B5                |00F5| 0|Tricky's ball. you automatically pick it up. when thrown it flies off VERY fast and Tricky won't follow it.
04DA|0480|SidekickBal|00B5                |00C6| 0|Tricky's ball, instantly despawns
04DB|FFFF|LockIcon   |0451 0454 0455      |FFFF| 0|
04DC|FFFF|coldWaterCo|0467                |0119| 0|makes nearby water cold (you take damage being in it)
04DD|0342|animbubble |02FA                |00C6| 0|
04DE|0360|Duster     |02C6                |0118| 0|Bafomdad (1up); flies away to random spots
04DF|FFFF|BlueMushroo|00D6                |01A7| 0|
04E0|0357|BlueMushroo|00D6                |00C6| 0|
04E1|0087|EnergyEgg  |02B9                |00ED| 0|health pickup; frequently disappears, can't be collected
04E2|FFFF|EnergyEggAn|02B9                |00C6| 0|
04E3|FFFF|Apple      |02BB                |00ED| 0|small health pickup
04E4|FFFF|MoonSeedCol|0161                |00ED| 0|
04E5|0482|AppleAnim  |02BB                |00C6| 0|
04E6|0371|AppleOnTree|02BA 02BB           |0117| 0|
04E7|0477|MagicDustSm|02C5                |00FF| 0|
04E8|0449|MagicDustSm|02C5                |00C6| 0|
04E9|044C|MagicDustMi|02C5                |00FF| 0|
04EA|044D|MagicDustMi|02C5                |00C6| 0|
04EB|FFFF|MagicDustLa|02C5                |00FF| 0|
04EC|04B6|MagicDustLa|02C5                |00C6| 0|
04ED|FFFF|MagicDustHu|02C5                |00FF| 0|magic energy pickup
04EE|FFFF|MagicDustHu|02C5                |00C6| 0|
04EF|FFFF|GreenScarab|02B0                |0106| 0|
04F0|04B7|RedScarab  |02B0                |0106| 0|
04F1|FFFF|GoldScarab |02B0                |0106| 0|
04F2|FFFF|RainScarab |02B0                |0106| 0|
04F3|0484|GreenScarab|02B0                |00C7| 0|
04F4|047D|RedScarabAn|02AF                |00C6| 0|
04F5|FFFF|GoldScarabA|02B0                |00C6| 0|
04F6|FFFF|FireFly    |02EF                |020B| 0|
04F7|FFFF|SmallBasket|02AC                |0104| 3|
04F8|047F|LargeBasket|02AD                |0105| 3|empty
04F9|0353|LargeCrate |02B2                |0105| 3|
04FA|0351|LargeCrateL|02B3                |0105| 3|
04FB|FFFF|ReinforcedC|02B4                |0104| 3|
04FC|011A|TreasureChe|02B7                |011D| 3|
04FD|011C|MapTreasure|02B7 02B8           |011D| 3|"TREASURE CHEST"
04FE|011B|MagicPlant |02C3 02C4           |00FE| 0|plant that drops magic pickups
04FF|0119|StayPoint  |0467                |0102| 0|
0500|0117|ProjectileS|02C8 02C9           |00F9| 0|switch activated by fireball
0501|034F|GunPowderBa|016A                |0158| 0|
0502|FFFF|MetalBarrel|033E                |0158| 0|
0503|0088|BarrelGener|033C                |0282| 0|doesn't actually generate barrels
0504|0352|BarrelPad  |033D                |012F| 0|
0505|011D|MetalPushBl|02B5                |00EF| 0|
0506|0121|SpiritDoorL|033F                |0167| 0|
0507|FFFF|SpiritDoorS|0340                |0157| 0|
0508|FFFF|BurnableVin|00D9                |00E7| 0|
0509|0122|BombPlant  |00D8                |01A9| 0|
050A|0123|BombPlantSp|0447                |01AA| 0|bomb spore, collectible
050B|0124|BombPlantSp|0447                |00C6| 0|bomb spore, despawns
050C|0127|BombPlantin|045F                |01AB| 0|
050D|FFFF|HoloPoint  |0341                |0168| 0|crashes Dolphin
050E|0128|MSBush     |0165                |017F| 0|
050F|012A|MSVine     |0167                |017F| 0|
0510|012B|MSSeqObject|0466                |0112| 0|
0511|0135|animMoonSee|0161                |00C6| 0|
0512|FFFF|MSPlantingS|0161                |025B| 0|
0513|047E|InfoPoint  |02C1                |00EC| 0|press A to temporarily hide the HUD
0514|FFFF|GroundQuake|0131                |00FB| 0|
0515|013A|fuelCell   |00F6                |0123| 0|
0516|FFFF|ReStartMark|00F7                |00E6| 0|
0517|0263|deathGas   |045F                |0124| 0|
0518|0265|deathGasNoF|045F                |0124| 0|
0519|0266|fogControl |045F                |0140| 0|
051A|FFFF|TrigPnt    |0473 0461           |0126| 0|
051B|004A|TrigCyl    |0473                |0126| 0|
051C|0264|TrigPln    |0472 045A           |0126| 0|
051D|013C|TrigArea   |0474 0461           |0126| 0|
051E|FFFF|TrigTime   |0474 0461           |0126| 0|
051F|0125|TrigButt   |0474 0461           |0126| 0|
0520|0126|TriggSetp  |0474 0461           |0126| 0|
0521|0267|TrigBits   |0474 0461           |0126| 0|
0522|0133|TrigCrve   |0475 0461           |0126| 0|
0523|050E|ARWArwing  |0480                |029A| 0|
0524|050F|ARWArwingTa|04A3                |012E| 0|
0525|0255|ARWArwingLa|0487 0486           |029B| 0|
0526|0132|ARWArwingLa|0488                |029B| 0|
0527|0136|ARWArwingBo|048A                |029C| 0|
0528|0138|ARWArwingBo|0484                |029D| 0|
0529|0139|ARWArwingFo|0485                |00C6| 0|
052A|012C|ARWBombColl|0490 048F           |029F| 0|
052B|012D|ARWLaserCol|0490 0495           |029F| 0|
052C|012E|ARWContaine|0490                |029F| 0|
052D|012F|ARWSporeCol|0492                |029F| 0|
052E|FFFF|ARWDinoEggC|0494                |029F| 0|
052F|FFFF|ARWMoonSeed|0493                |029F| 0|
0530|FFFF|ARWGoldRing|0496 0497           |02A0| 0|
0531|FFFF|ARWSilverRi|0498 0499           |02A0| 0|
0532|FFFF|ARWBlocker |048B                |02A8| 0|
0533|FFFF|ARWBlockerS|048C                |02A8| 0|
0534|FFFF|ARWBlockerD|048D                |00C6| 0|
0535|FFFF|ARWBlockerD|048E                |00C6| 0|
0536|FFFF|ARWLevelCon|0467                |02A1| 0|
0537|FFFF|ARWSpeedStr|049A                |02A2| 0|
0538|FFFF|ARWArwingGu|049D 049F           |029D| 0|
0539|0425|ARWArwingGu|049E 04A0           |029D| 0|
053A|0262|ARWArwingRo|049B 049C           |029D| 0|
053B|0078|ARWSquadron|0467                |02A6| 0|
053C|0268|ARWBigAster|04A1                |02A6| 0|
053D|FFFF|ARWSmallAst|04A2                |02A6| 0|
053E|FFFF|ARWAnimSmal|04A2                |00C6| 0|
053F|032A|ARWAnimBigA|04A1                |00C6| 0|
0540|0320|ARWGenerato|0467                |02A5| 0|
0541|FFFF|ARWProximit|04A6                |02A7| 0|
0542|FFFF|ARWTimedMin|04A6                |02A7| 0|
0543|FFFF|ARWStaticGu|0467                |FFFF| 0|
0544|FFFF|ARWMobileGu|04A5                |02A6| 0|
0545|0201|ARWGroundGu|0467                |02A6| 0|
0546|031F|ARWShipFly |04A7                |02A6| 0|
0547|031E|ARWShipTwin|04A8                |02A6| 0|
0548|031D|ARWShipAnge|04A9                |02A6| 0|
0549|031C|ARWSeqObjec|0466                |0112| 0|
054A|031B|ARWStarFiel|01BA                |012E| 0|
054B|0511|ARWDarkIceP|019C                |00C6| 0|
054C|0512|ARWDragonRo|019A                |00C6| 0|
054D|FFFF|ARWCloudRun|0199                |00C6| 0|
054E|031A|ARWWalledCi|019B                |00C6| 0|
054F|FFFF|ARWDinoPlan|0198                |00C6| 0|
0550|FFFF|ARWAnimBomb|048A                |00C6| 0|
0551|0319|LGTPointLig|047C 047D           |02A9| 0|illuminates area
0552|FFFF|LGTDirectio|047E                |02AA| 0|debug light? doesn't seem to be visible
0553|FFFF|LGTProjecte|0467                |02AB| 0|
0554|FFFF|LGTControlL|0467                |02AC| 0|
0555|FFFF|SKYScapeDef|04AB                |FFFF| 0|clouds
0556|FFFF|SKYScapeSto|04AC                |FFFF| 0|
0557|FFFF|SKYScapeSto|04AD                |FFFF| 0|
0558|FFFF|SKYScapeSpa|01BA                |FFFF| 0|
0559|0318|SKYRingDefa|04AE                |FFFF| 0|
055A|0317|SKYRingStor|04AF                |FFFF| 0|
055B|FFFF|SKYMountain|04B0                |FFFF| 0|
055C|034E|SKYmountain|04B1                |FFFF| 0|
055D|FFFF|SKYmountain|04B2                |FFFF| 0|
055E|0048|SKYmountain|04B3                |FFFF| 0|
055F|0324|SKYSun     |04B4                |FFFF| 0|the sun
0560|FFFF|SKYMoon    |04B5                |FFFF| 0|the moon
0561|0383|SkyVortS   |04B6                |02B3| 0|
0562|0285|SkyVortC   |04B7                |02B3| 0|
0563|0286|WndLiftS   |04B8                |02B3| 0|
0564|0287|WndLiftC   |04B9                |02B3| 0|
0565|0288|DIM_PitVort|04BA                |02B3| 0|
0566|0289|DirectionSi|0102 0103           |012E| 0|
0567|028A|FirePole   |02EE                |012E| 0|
0568|FFFF|BlueFernFlo|0104                |02AD| 0|
0569|0382|WhiteFernFl|0105                |02AD| 0|
056A|0322|YellowFernF|0106                |02AD| 0|
056B|FFFF|AlienTreePa|0107                |011A| 0|
056C|0381|AlienSpore |0162                |02AD| 0|
056D|FFFF|BlueFlowerP|0108                |02AD| 0|
056E|04DD|BlueWhiteFl|0109                |02AD| 0|
056F|FFFF|RedYellowFl|010A                |02AD| 0|
0570|FFFF|YellowWhite|010B                |02AD| 0|
0571|FFFF|BullRush   |0113                |02AD| 0|
0572|04DC|BullRushClu|0114                |02AD| 0|
0573|048E|LongGrassCl|0115                |02AD| 0|
0574|00C6|DragonRockG|04D5                |02AD| 0|
0575|00D4|AlienGrassC|0119                |02AD| 0|
0576|FFFF|AlienGrassC|011A                |02AD| 0|
0577|00EB|FernTree   |010C                |02AF| 0|
0578|00EA|smallfern  |010E                |02AF| 0|
0579|00EC|tallpalmtre|010F                |02AF| 0|
057A|FFFF|JungleTree |0110 0111           |02AF| 0|
057B|FFFF|BrokenPipe |0357                |02B0| 0|
057C|009D|MetalRafter|0358                |02B0| 0|
057D|FFFF|BoulderOne |0116                |02B0| 0|
057E|FFFF|RedBoulder |0361                |02B0| 0|
057F|0109|ThornBush  |0359                |024E| 0|
0580|FFFF|Cactus     |04D3                |02B0| 0|
0581|02B2|DragonFly  |0117                |012E| 0|
0582|FFFF|CurveFish  |0118                |0103| 0|
0583|02BC|AnimFish   |0118                |00C6| 0|
0584|02BD|SnowGrass  |011B                |02AD| 0|
0585|02BE|RootPlant  |04D4                |02AF| 0|
0586|02BF|SnowBullrus|011C                |02AD| 0|
0587|02C0|SnowBoulder|011D                |02B0| 0|
0588|FFFF|SnowTree1  |011E                |02AF| 0|
0589|FFFF|SnowBush   |011F                |011A| 0|
058A|FFFF|SnowTree2  |0120                |02AF| 0|
058B|003E|SnowTree3  |0121                |02AF| 0|
058C|FFFF|SnowTree4  |0122                |02AF| 0|
058D|FFFF|SnowLog    |0123                |011A| 0|
058E|00EE|SnowFruitTr|0124                |02AF| 0|
058F|02C4|SulphurCrys|0125                |011A| 0|
0590|02C5|CraterFlame|045F                |01E7| 0|
0591|02C6|CobwebCorne|0126                |011A| 0|
0592|02C7|CobwebCeili|0127                |011A| 0|
0593|02C8|CobwebEdge |0128                |011A| 0|
0594|02C9|HangingPlan|0129                |02AD| 0|
0595|02CA|HangingPlan|012A                |02AD| 0|
0596|02CB|GrassWeedCl|012B                |011A| 0|
0597|02C1|GrassSupaCh|012C                |011A| 0|
0598|02C2|FernBush   |012D                |011A| 0|
0599|02BB|FernCurly  |012E                |011A| 0|
059A|02C3|WaterLillyL|012F                |02AD| 0|
059B|FFFF|WaterLillyF|0130                |011A| 0|
059C|02CC|CmbSrc     |0000                |02B1| 0|
059D|0045|CmbSrcTPole|014D                |02B1| 0|
059E|02CD|CmbSrcTWall|024C                |02B1| 0|
059F|00FA|ThusterSour|0000                |02B1| 0|
05A0|00FB|DustMoteSou|045F                |02B2| 0|
05A1|0300|TailLightSo|045F                |02B2| 0|
05A2|FFFF|FireWorkSou|045F                |02B2| 0|
05A3|0046|HangingLigh|014C                |0206| 0|
05A4|FFFF|PoleLight  |014E                |0206| 0|
05A5|FFFF|PoleFlame  |014D                |0206| 0|
05A6|0047|LightSource|045F                |0206| 0|burning sound effect?
05A7|FFFF|FXEmit     |0000                |012B| 0|
05A8|FFFF|AreaFXEmit |0000                |0130| 0|
05A9|FFFF|WaterFallSp|0000                |0132| 0|
05AA|FFFF|TAreaFXEmit|0000                |0130| 0|
05AB|FFFF|Sea        |0000                |FFFF| 0|
05AC|01FA|StaffAction|0467                |011C| 0|
05AD|01FB|StaffBoostP|0088                |011C| 0|
05AE|01FC|StaffBoulde|0089                |011C| 0|
05AF|FFFF|StaffBoulde|008A                |011C| 0|
05B0|04E8|StaffBoulde|008B                |011C| 0|
05B1|04EA|StaffLeverO|008C                |011C| 0|
05B2|04EC|StaffLeverT|008D                |011C| 0|
05B3|04EE|CommandMenu|04BC 04BD 04BE      |FFFF| 0|the C stick menu from the HUD
05B4|FFFF|CommandMenu|04BF                |FFFF| 0|
05B5|FFFF|CommandMenu|04C0                |FFFF| 0|
05B6|FFFF|CommandMenu|04C1 04C2           |FFFF| 0|
05B7|0062|FrontEndLog|04C4                |FFFF| 0|
05B8|0063|Blue_bean  |02BB                |0106| 0|
05B9|0064|EnergyGem1 |046A                |012A| 0|another FX cube
05BA|04DA|beanAnim   |02BB                |00C6| 0|
05BB|FFFF|DFropenode |0000                |0175| 0|
05BC|0424|DFsequences|0466                |0112| 0|
05BD|FFFF|DFSH_Door1S|028E                |0176| 0|
05BE|FFFF|DFSH_Door2S|028F                |0177| 0|
05BF|FFFF|WM_PureMagi|02EB                |00ED| 0|
05C0|0429|DFSH_Door3S|0290                |0177| 0|
05C1|0426|DFSH_Door4S|0291                |0177| 0|
05C2|FFFF|Footstep   |0002                |FFFF| 0|a light yellow cylinder
05C3|02E1|WarpCigar  |04E6                |00C6| 0|
05C4|02E2|WarpGasCyli|04E7                |00C6| 0|
