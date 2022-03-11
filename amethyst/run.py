#!/usr/bin/env python3
"""Build amethyst.arg file and launch emulator."""
import subprocess
import argparse
import struct

def tInt(n):
    """use as arg type to enable hex input"""
    return int(n, 0)

def addFlags(tbl):
    for arg in tbl:
        parser.add_argument('--'+arg[0], action='store_true', help=arg[2])

def getFlags(tbl):
    result = 0
    varg = vars(args)
    for arg in tbl:
        if varg[arg[0].replace('-', '_')]:
            result |= (1 << arg[1])
    return result

parser = argparse.ArgumentParser(
    description='Run game in emulator with arguments.')

parser.add_argument('-s', '--save-slot', type=int, default=0, choices=(0,1,2,3),
    help="Which save slot to load (1-3, 0=none)")

parser.add_argument('-c', '--coords', type=str, default=None,
    help="Coords to spawn at (X,Y,Z,layer)")

parser.add_argument('-w', '--warp', type=tInt, default=None, choices=range(0x80),
    help="Spawn at specified warp index")

debugPrint = (
    # arg, bit, description
    ('dprint-tricky',        0, "Show Tricky debug text"),
    ('dprint-player-pos',    1, "Show player coords debug text"),
    ('dprint-camera-pos',    2, "Show camera coords debug text"),
    ('dprint-restart-pos',   3, "Show restart point debug text"),
    ('dprint-memory',        4, "Show memory info debug text"),
    ('dprint-interact-obj',  5, "Show interact object debug text"),
    ('dprint-heap',          6, "Show heap debug text"),
    ('dprint-gamebits',      7, "Show gamebit changelog debug text"),
    ('dprint-player-state',  8, "Show player state debug text"),
    ('dprint-perf',          9, "Show performance debug text"),
    ('dprint-fps',          10, "Show FPS meter"),
    ('dprint-rng',          11, "Show RNG debug text"),
    ('dprint-streams',      12, "Show audio stream debug text"),
    ('dprint-sfx',          13, "Show sound effect debug text"),
    ('dprint-env',          14, "Show environment debug text"),
    ('dprint-objseq',       15, "Show objseq debug text"),
)
debugRender = (
    ('render-map-grid',       0, "Show map grid on screen"),
    ('render-debug-objs',     1, "Show debug objects"),
    ('hide-disabled-objs',    9, "Hide disabled debug objects"),
    ('render-perf',           2, "Show performance meters"),
    ('render-rng',            3, "Show RNG graph"),
    ('render-hitboxes',       4, "Show object hitboxes"),
    ('render-attach-points',  5, "Show object attach points"),
    ('render-focus-points',   6, "Show object focus points"),
    ('render-unk-points',     7, "Show object unknown points"),
    ('render-player-vel',     8, "Show player velocity vector"),
)
cheats = (
    ('inf-hp',          0, "Enable infinite HP cheat"),
    ('inf-mp',          1, "Enable infinite MP cheat"),
    ('inf-money',       2, "Enable infinite money cheat"),
    ('inf-lives',       3, "Enable infinite lives cheat"),
    ('frozen-baddies',  4, "Enable frozen baddies cheat"),
    ('inf-food',        5, "Enable infinite Tricky energy cheat"),
    ('ten-rings',       6, "Enable always 10 rings cheat (Arwing)"),
    ('inf-bombs',       7, "Enable infinite bombs cheat (Arwing)"),
)

addFlags(debugPrint)
addFlags(debugRender)
addFlags(cheats)

loadMapLayer, loadPosX, loadPosY, loadPosZ = 0x7F, 0, 0, 0
args = parser.parse_args()
if args.warp is not None:
    with open('discroot/files/WARPTAB.bin', 'rb') as warptab:
        warptab.seek(args.warp * 16)
        loadPosX, loadPosY, loadPosZ, loadMapLayer = struct.unpack(
            '>fffh', warptab.read(14))
elif args.coords is not None:
    coords = args.coords.split(',')
    loadPosX, loadPosY, loadPosZ = float(coords[0]), float(coords[1]), float(coords[2])
    loadMapLayer = int(coords[3])

with open('amethyst.arg', 'wb') as file:
    file.write(struct.pack('>IIIIbbbbfff',
        1, # version
        getFlags(cheats), getFlags(debugPrint), getFlags(debugRender),
        args.save_slot-1, loadMapLayer,
        0, 0, # padding
        loadPosX, loadPosY, loadPosZ,
    ))

subprocess.run(['./build.sh', 'run'])
# build.sh will move the arg file to the appropriate place
# and remove it after running so it doesn't sneak into a release
# or hang around and cause trouble later
