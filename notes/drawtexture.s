# this draws a texture containing some text
# but it doesn't appear to be the actual font
# it's like "CONTIUEGameSav" and some shit
# that does appear to be how all the text is stored...
test_TX1: .float 0.0
test_TY1: .float 0.0
test_TX2: .float 1.0
test_TY2: .float 1.0
LOADW   r3, 0x803dc9ec
lwz     r3, 0x14(r3)
li      r4, 0
CALL    0x8004c2e4
#CALL    0x800799c0
#CALL    0x800795e8
#CALL    0x80078fa4
CALL    0x80079804
#CALL    0x80078a7c
lfs     f1, (test_TX1 - mainLoop)(r14)
lfs     f2, (test_TY1 - mainLoop)(r14)
lfs     f3, (test_TX2 - mainLoop)(r14)
lfs     f4, (test_TY2 - mainLoop)(r14)
li      r3, 20
li      r4, 20
li      r5, 1600
li      r6, 1600
CALL    0x80075e8c


# this works for defining a custom GameTextBox
# may be the easiest way to achieve drawing fixed width
# unless we can work out the textures
# or just use the debug font? should be able to do that even
# if debug print isn't enabled.
lfs      f1, (textPos - mainLoop)(r14)
lfs      f2, (textPos+4 - mainLoop)(r14)
lfs      f3, (lineHeight - mainLoop)(r14)
lwz      r3, 0(r21) # get title string
add      r3, r3, r14
addi     r4, r14, textBox - mainLoop
li       r5, 0
CALL     0x800174d0

textBox:
.short  0
.short  0
.short  0
.short  0
.short	320 # width
.short	240 # height
.float	1   # textScale
.byte	3   # justify
.byte   0
.byte	0   # curJustify
.byte	7   # textureId
.short	100 # x
.short	100 # y
.byte   0
.byte   0
.byte   0
.byte   0
.byte   0
.byte   0
.byte   0
.byte   0
textPos: .float 10, 10
lineHeight: .float 16
