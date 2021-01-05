import Struct from '/r/js/Struct.js';
/** Define structures used in SFA models.
 */

export const ModelHeader = Struct({
    //0x00: ref count, set on load
    'flags':         ['H', 0x02],
    //'modelId':       ['H', 0x04], // set on load
    'fileSize':      ['I', 0x0C],
    'unk18':         ['I', 0x18],
    'extraAmapSize': ['I', 0x1C],
    'textures':      ['I', 0x20], // -> texture IDs (u32)
    'flags24':       ['B', 0x24],
    'unk25':         ['B', 0x25], // relates to lighting
    'vtxs':          ['I', 0x28], // -> vec3s
    'normals':       ['I', 0x2C], // -> vec3s
    'colors':        ['I', 0x30], // -> ??? (how many?)
    'texCoords':     ['I', 0x34], // -> vec2s
    'shaders':       ['I', 0x38], // -> Shader
    'bones':         ['I', 0x3C], // -> Bone
    'boneQuats':     ['I', 0x40], // -> Quaternion
    'unk44':         ['3I',0x44], // 0xFFC00000 x3; never read
    'unk50':         ['I', 0x50],
    'vtxGroups':     ['I', 0x54], // -> ModelVtxGroup
    'hitboxes':      ['I', 0x58], // -> HitboxStruct
    'pAltIndBuf':    ['I', 0x64], // -> ???
    'pAnimBuf':      ['I', 0x68], // -> ???
    'pModelIndBuf':  ['I', 0x6C], // -> array of u16 anim IDs - null in file, loaded from MODANIM.TAB
    'animIdxs':      ['8h',0x70], // related to animation - FFxx disables walk anim - 0x0, 0xFD, 0x112, 0x197, 0x218, 0x29B, 0x0, 0x0
    'amapTabEntry':  ['I', 0x80],
    'posFineSkinningConfig': ['I', 0x84],
    'posFineSkinningPieces': ['I', 0xA4], // ObjInstance*
        // field_88.nVtxs = how many
    'posFineSkinningWeights': ['I', 0xA8],
    'nrmFineSkinningWeights': ['I', 0xAC],
    'dlists':        ['I', 0xD0], // -> DisplayListPtr
    'renderInstrs':  ['I', 0xD4], // -> bit-packed instruction code
    'nRenderInstrs': ['H', 0xD8], // num bytes
    'offsDC':        ['I', 0xDC], // -> pointers
    'unkE0':         ['H', 0xE0],
    'flagsE2':       ['H', 0xE2], // related to textures; # texcoords?
    'nVtxs':         ['H', 0xE4],
    'nNormals':      ['H', 0xE6],
    'nColors':       ['H', 0xE8],
    'nTexCoords':    ['H', 0xEA],
    'nAnimations':   ['H', 0xEC],
    'nTextures':     ['B', 0xF2],
    'nBones':        ['B', 0xF3],
    'nVtxGroups':    ['B', 0xF4],
    'nDlists':       ['B', 0xF5],
    'unkF6':         ['B', 0xF6],
    'nHitBoxes':     ['B', 0xF7],
    'nShaders':      ['B', 0xF8],
    'nPtrsDC':       ['B', 0xF9], // #ptrs at field DC
    'nTexMtxs':      ['B', 0xFA],
    'unkFB':         ['B', 0xFB],
});

export const Bone = Struct({
    'parent': ['B',  0x00], // high bit is a flag?
    'unk01':  ['B',  0x01], // idx to write to?
    'unk02':  ['B',  0x02], // idx -> something 0x40 bytes (Mtx44?)
    'unk03':  ['B',  0x03], // idx -> something 0x20 bytes
    'head':   ['3f', 0x04],
    'tail':   ['3f', 0x10],
});

export const ModelVtxGroup = Struct({
    'bone0':  ['B', 0x00],
    'bone1':  ['B', 0x01], // weight is 1.0 - bone0 weight
    'weight': ['B', 0x02], // for bone0
    'unk03':  ['B', 0x03], // always 0; padding?
});
