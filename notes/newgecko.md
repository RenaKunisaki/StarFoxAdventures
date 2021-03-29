Since our new Gecko adapter thingy is probably way better than the original we should design a better protocol for it too.

Note, SPI transfer size is 16 bits. should tweak the command formats to account for this. it'll be easier on the game side if it can do 8/16-bit immediate reads to get the command, then DMA as needed.

We could start implementing the game side of it in Dolphin already.

# Basic Commands
- Dolphin OS stuff? eg list threads, RTC
- Video stuff, eg screen grab
    - GPU commands? use breakpoints to log dlists?
- Checksum memory, to check for changes?
- Stack trace (can just be done using memory read)
- Cheat stuff (actual Gecko codes?)
- Controller I/O? (eg spoof/read controller inputs, control motors)

(there is a good chance most of this will never be needed or implemented)

The exact command bytes may vary but I imagine something like:

Memory:
    10ccaaaaaaaa: read
        cc: count, in bytes (or shorts, since 16bit transfer?)
        aa: address
    11ccccccaaaaaaaa: batch read - uses DMA
    12ddccccccccaaaaaaaaccccpp...: dump to external device
        dd: 0=memcard A, 1=SD card, 2=USB
        cc: path length, pp: path/name
        if cc is 0 then pp is 4 bytes sector number
    13: reserved
    14ccaaaaaaaadd...: write; dd=data
    15ccccccaaaaaaaadd...: batch write
    16ddccccccccaaaaaaaaccccpp...: read from external device
        dd: 0=memcard A, 1=SD card, 2=USB, 3=disc
            - if dd&0x80, send the data out over EXI instead of to memory
        cc: path length, pp: path/name
    17: reserved
    18ccaaaaaaaadd...: search
        cc: data length
        aa: start addr
        dd: data
        returns next matching address
    19ccccccaaaaaaaabbbbbbbb: copy cc bytes from aa to bb
    1Accccccaaaaaaaabbbbbbbb: fill cc bytes at aa with bb
    1Bccccaaaaaaaa: flush cache for cc bytes at aa
    maybe even have dump/load memory to/from memory card/SD card/USB device?
Code:
    20aaaaaaaaccdd...ccff...: Call func
        aa: address
        cc: number of params
        dd: param data...
        cc: number of float params
        ff: float param data...
        should automatically work out the stack frame stuff for extra params.
    21ccccdd...: execute code
        cc: number of words (instructions)
        dd: code...
    22ccffaaaaaaaa: set/clear breakpoints
        cc: number of bytes
            - the breakpoint handler would check if it's in the right range and ignore if not
        ff: flags (read/write/exec; unset all to remove)
            01: read
            02: write
            04: exec
            08: pause (else, just log)
            10: ignore this address (eg to ignore code that's commonly touching a var)
        aa: start address
Registers:
    30ccccnnnn: read
        cc: number of regs
        nn: first reg
        0-31: CPU regs; 32-63: FPU regs; 64+: SPR
    31ccccnnnndd...: write
Misc:
    - these are commands the MCU itself responds to.
    - putting them at the end of the list reduces the size of the command table in the game.
    08: freeze
    09: resume
    0A: advance frame
    0B: single-step instruction
        - can probably do it by putting breakpoints on the next opcode, but jumps...?
    FD: reboot?
    FE: return some version info
    FFxx...: game-specific command; xx=command, rest varies...
        00cccccccc: alloc
        01aaaaaaaa: free

let's try to rework that for 16 bits.

# PC -> Game
each command is preceded by u16 length (bytes, big endian).

00cc: misc 1-byte commands, cc=command
    00: ping (response: FF00)
        FF as in GAME_CMD_ACK, 00 as in PC_CMD_MISC
    01: pause
    02: resume
    03: advance frame
    04: single step
    05: query; response is u8 bitflags xxxxxxxr
        r: running
        x: reserved (always 0)
    06: grab screenshot
        response: 0009 wwww hhhh vvvv...
    FF: reboot
01cc aaaa aaaa: read memory, cc words, address aa
    if cc=0: 0100 cccc cccc aaaa aaaa
        response: 0100 cccc cccc aaaa aaaa vvvv...
    response: 01cc aaaa aaaa vvvv...
02cc aaaa aaaa vvvv...: write memory, cc words, address aa, values vv...
    0200 cccc cccc aaaa aaaa vvvv...
03cc aaaa: read CPU regs, cc words, starting from reg aa
    00-1F: CPU regs
    20-3F: FPU regs
    40+:   SPR
    response: 03cc aaaa vvvv...
04cc aaaa vvvv...: write CPU regs
05cc aaaa aaaa: flush cache from aa to aa+cc
    0500 cccc cccc aaaa aaaa - we probably don't need that
06cc aaaa aaaa bbbb bbbb: copy memory from aa to bb
    0600 cccc cccc aaaa aaaa bbbb bbbb
07cc aaaa aaaa vvvv: fill memory at aa
    0700 cccc cccc aaaa aaaa vvvv
08cc aaaa aaaa vvvv...: search memory for vv starting at aa
    response: 08cc aaaa aaaa (0=no match)
    we probably don't need 0800 cccc cccc...
09cc ffrr xx00 aaaa aaaa vvvv...: call function at aa
    vv: params (r3+)
    ff: float params
    rr: return params
    xx: return float params
    response: 09cc aaaa aaaa vvvv...
        cc: total return values (count of u32)
    maybe this would be easier with some multipart command like:
        - load words into FPRs if needed
        - write words into buffer
        - call func using params from buffer
        - read result from buffer
0Acc vvvv vvvv...: execute code
    0A00 cccc cccc vvvv vvvv...
0Bcc ffff aaaa aaaa: set breakpoints from aa to aa+cc
    0B00 cccc cccc ffff aaaa aaaa
    ffff: flags
        0001 read
        0002 write
        0004 exec
        0008 ignore
        0010 break (else only log)
        0020 one-shot

# Game -> PC
each command is preceded by u16 length (bytes, big endian).

01: response for cmd 01
03: response for cmd 03
08: response for cmd 08
09: response for cmd 09
FB: exception (need to figure out specifics)
FCff aaaa aaaa: breakpoint hit, ff=flags
FDcc nnnn vvvv...: debug log message
    cc: channel (0=gecko itself, 1=OSReport, 2=OSPanic, ...more?)
    nn: length (words, pad msg with 0)
FEcc xxxx: NAK command cc, xxxx = error code
FFcc: ACK command cc (default response for commands that don't have one)


# Game-Specific Commands
Some command like 0xFF followed by the game-specific command byte.
eg: list objects, spawn object, game bits...


# USB Control Commands
The Teensy itself is controlled by sending USB Setup packets to endpoint 0:
0x03 (USBCDC_REQ_GET_COMM_FEATURE): read debug port
0x11 (USBCDC_REQ_SET_HOOK_STATE): set mode, wIndex:
    0: disable debug interface
    1: enable debug interface
    2: enable Gecko interface
0x3A (USBCDC_REQ_GET_PROFILE): query; response:
    u8 status bitflags: ......gd
        .: reserved (always 0)
        g: Gecko mode enabled
        d: debug interface enabled
    u8 version major, u8 version minor, u8 revision
