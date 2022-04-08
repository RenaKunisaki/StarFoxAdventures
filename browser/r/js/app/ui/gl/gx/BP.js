//XXX do we need this?
import { hex } from "../../../../Util";

const RegNames = [
    'GEN_MODE',        'COPY_FILTER0',    'COPY_FILTER1',   'COPY_FILTER2',    //00
    'COPY_FILTER3',    null,              'IND_MTXA0',      'IND_MTXB0',       //04
    'IND_MTXC0',       'IND_MTXA1',       'IND_MTXB1',      'IND_MTXC1',       //08
    'IND_MTXA2',       'IND_MTXB2',       'IND_MTXC2',      'IND_IMASK',       //0C
    'IND_CMD0',        'IND_CMD1',        'IND_CMD2',       'IND_CMD3',        //10
    'IND_CMD4',        'IND_CMD5',        'IND_CMD6',       'IND_CMD7',        //14
    'IND_CMD8',        'IND_CMD9',        'IND_CMDA',       'IND_CMDB',        //18
    'IND_CMDC',        'IND_CMDD',        'IND_CMDE',       'IND_CMDF',        //1C
    'SU_SCIS0',        'SU_SCIS1',        'SU_LPSIZE',      'SU_COUNTER',      //20
    'RAS_COUNTER',     'RAS1_SS0',        'RAS1_SS1',       'RAS1_IREF',       //24
    'RAS1_TREF0',      'RAS1_TREF1',      'RAS1_TREF2',     'RAS1_TREF3',      //28
    'RAS1_TREF4',      'RAS1_TREF5',      'RAS1_TREF6',     'RAS1_TREF7',      //2C
    'SU_SSIZE0',       'SU_TSIZE0',       'SU_SSIZE1',      'SU_TSIZE1',       //30
    'SU_SSIZE2',       'SU_TSIZE2',       'SU_SSIZE3',      'SU_TSIZE3',       //34
    'SU_SSIZE4',       'SU_TSIZE4',       'SU_SSIZE5',      'SU_TSIZE5',       //38
    'SU_SSIZE6',       'SU_TSIZE6',       'SU_SSIZE7',      'SU_TSIZE7',       //3C
    'PE_ZMODE',        'PE_CMODE0',       'PE_CMODE1',      'PE_CONTROL',      //40
    'FIELD_MASK',      'PE_DONE',         null,             'PE_TOKEN',        //44
    'PE_TOKEN_INT',    'EFB_TOPLEFT',     'EFB_SIZE',       'XFB_TARGET',      //48
    null,              null,              'DISPCOPYYSCALE', 'PE_COPY_CLEAR_AR',//4C
    'PE_COPY_CLEAR_GB','PE_COPY_CLEAR_Z', 'PE_COPY_EXEC',   'PE_COPY_FILTER53',//50
    'PE_COPY_FILTER54','PE_BOUNDBOX55',   'PE_BOUNDBOX56',  null,              //54
    null,              'PE_SCISSOR_BOX_OFFS', null,         null,              //58
    null,              null,              null,             null,              //5C
    null,              null,              null,             null,              //60
    'TX_LOADTLUT0',    'TX_LOADTLUT1',    null,             null,              //64
    null,              null,              null,             null,              //68
    null,              null,              null,             null,              //6C
    null,              null,              null,             null,              //70
    null,              null,              null,             null,              //74
    null,              null,              null,             null,              //78
    null,              null,              null,             null,              //7C
    'TX_SETMODE0_I0',  'TX_SETMODE0_I1',  'TX_SETMODE0_I2',  'TX_SETMODE0_I3', //80
    'TX_SETMODE1_I0',  'TX_SETMODE1_I1',  'TX_SETMODE1_I2',  'TX_SETMODE1_I3', //84
    'TX_SETIMAGE0_I0', 'TX_SETIMAGE0_I1', 'TX_SETIMAGE0_I2', 'TX_SETIMAGE0_I3',//88
    'TX_SETIMAGE1_I0', 'TX_SETIMAGE1_I1', 'TX_SETIMAGE1_I2', 'TX_SETIMAGE1_I3',//8C
    'TX_SETIMAGE2_I0', 'TX_SETIMAGE2_I1', 'TX_SETIMAGE2_I2', 'TX_SETIMAGE2_I3',//90
    'TX_SETIMAGE3_I0', 'TX_SETIMAGE3_I1', 'TX_SETIMAGE3_I2', 'TX_SETIMAGE3_I3',//94
    'TX_LOADTLUT0',    'TX_LOADTLUT1',    'TX_LOADTLUT2',    'TX_LOADTLUT3',   //98
    null,              null,              null,             null,              //9C
    'TX_SETMODE0_I4',  'TX_SETMODE0_I5',  'TX_SETMODE0_I6',  'TX_SETMODE0_I7', //A0
    'TX_SETMODE1_I4',  'TX_SETMODE1_I5',  'TX_SETMODE1_I6',  'TX_SETMODE1_I7', //A4
    'TX_SETIMAGE0_I4', 'TX_SETIMAGE0_I5', 'TX_SETIMAGE0_I6', 'TX_SETIMAGE0_I7',//A8
    'TX_SETIMAGE1_I4', 'TX_SETIMAGE1_I5', 'TX_SETIMAGE1_I6', 'TX_SETIMAGE1_I7',//AC
    'TX_SETIMAGE2_I4', 'TX_SETIMAGE2_I5', 'TX_SETIMAGE2_I6', 'TX_SETIMAGE2_I7',//B0
    'TX_SETIMAGE3_I4', 'TX_SETIMAGE3_I5', 'TX_SETIMAGE3_I6', 'TX_SETIMAGE3_I7',//B4
    'TX_SETTLUT_I4',   'TX_SETTLUT_I5',   'TX_SETTLUT_I6',   'TX_SETTLUT_I7',  //B8
    null,              null,              null,             null,              //BC
    'TEV_COLOR_ENV_0', 'TEV_ALPHA_ENV_0', 'TEV_COLOR_ENV_1', 'TEV_ALPHA_ENV_1',//C0
    'TEV_COLOR_ENV_2', 'TEV_ALPHA_ENV_2', 'TEV_COLOR_ENV_3', 'TEV_ALPHA_ENV_3',//C4
    'TEV_COLOR_ENV_4', 'TEV_ALPHA_ENV_4', 'TEV_COLOR_ENV_5', 'TEV_ALPHA_ENV_5',//C8
    'TEV_COLOR_ENV_6', 'TEV_ALPHA_ENV_6', 'TEV_COLOR_ENV_7', 'TEV_ALPHA_ENV_7',//CC
    'TEV_COLOR_ENV_8', 'TEV_ALPHA_ENV_8', 'TEV_COLOR_ENV_9', 'TEV_ALPHA_ENV_9',//D0
    'TEV_COLOR_ENV_A', 'TEV_ALPHA_ENV_A', 'TEV_COLOR_ENV_B', 'TEV_ALPHA_ENV_B',//D4
    'TEV_COLOR_ENV_C', 'TEV_ALPHA_ENV_C', 'TEV_COLOR_ENV_D', 'TEV_ALPHA_ENV_D',//D8
    'TEV_COLOR_ENV_E', 'TEV_ALPHA_ENV_E', 'TEV_COLOR_ENV_F', 'TEV_ALPHA_ENV_F',//DC
    'TEV_REGISTERL_0', 'TEV_REGISTERH_0', 'TEV_REGISTERL_1', 'TEV_REGISTERH_1',//E0
    'TEV_REGISTERL_2', 'TEV_REGISTERH_2', 'TEV_REGISTERL_3', 'TEV_REGISTERH_3',//E4
    'FOG_RANGE',       null,              null,              null,             //E8
    'TEV_RANGE_ADJ_C', 'TEV_RANGE_ADJ_K', 'TEV_FOG_PARAM_0', 'TEV_FOG_PARAM_1',//EF
    'TEV_FOG_PARAM_2', 'TEV_FOG_PARAM_3', 'TEV_FOG_COLOR',   'TEV_ALPHAFUNC',  //F0
    'TEV_Z_ENV_0',     'TEV_Z_ENV_1',     'TEV_KSEL_0',      'TEV_KSEL_1',     //F4
    'TEV_KSEL_2',      'TEV_KSEL_3',      'TEV_KSEL_4',      'TEV_KSEL_5',     //F8
    'TEV_KSEL_6',      'TEV_KSEL_7',      'SS_MASK',         null,             //FC
];

export const Reg = {};
for(let i=0; i<256; i++) {
    const name = RegNames[i];
    if(name) Reg[name] = i;
}

export default class BP {
    /** "BP" subsystem for GX.
     */
    constructor(gx) {
        this.gx = gx;
        this._rawVals = {};
        this.reset();
    }

    reset() {
        /** Reset all state back to default.
         */
        this.scissor = {x0:0, y0:0, x1:0, y1:0, xOffs:0, yOffs:0};
        for(let i=0; i<256; i++) this.setReg(i, 0);
    }

    getReg(reg) {
        return this._rawVals[reg];
    }

    setReg(reg, val) {
        /** Set BP register.
         *  @param {int} reg Register ID.
         *  @param {int} val Value, which should be a 24-bit integer.
         */
        this._rawVals[reg] = val;
        switch(reg) { //XXX verify these...
            case Reg.GEN_MODE:
                this._GEN_MODE = {
                    ZFREEZE:   (val >> 19) & 0x01,
                    NBMP:      (val >> 16) & 0x07,
                    REJECT_EN: (val >> 14) & 0x03,
                    NTEV:      (val >> 10) & 0x1F,
                    MS_EN:     (val >>  9) & 0x01,
                    NCOL:      (val >>  4) & 0x1F,
                    NTEX:      (val >>  0) & 0x0F,
                };
                break;
            case Reg.IND_MTXA0: case Reg.IND_MTXA1: case Reg.IND_MTXA2:
                this['IND_MTXA'+(reg - Reg.IND_MTXA0)] = {
                    S:  (val >> 22) & 0x0003,
                    MB: (val >> 11) & 0x07FF,
                    MA: (val >>  0) & 0x07FF,
                };
                break;
            case Reg.IND_MTXB0: case Reg.IND_MTXB1: case Reg.IND_MTXB2:
                this['IND_MTXB'+(reg - Reg.IND_MTXB0)] = {
                    S:  (val >> 22) & 0x0003,
                    MD: (val >> 11) & 0x07FF,
                    MC: (val >>  0) & 0x07FF,
                };
                break;
            case Reg.IND_MTXC0: case Reg.IND_MTXC1: case Reg.IND_MTXC2:
                this['IND_MTXC'+(reg - Reg.IND_MTXC0)] = {
                    S:  (val >> 22) & 0x0003,
                    MF: (val >> 11) & 0x07FF,
                    ME: (val >>  0) & 0x07FF,
                };
                break;
            case Reg.IND_CMD0: case Reg.IND_CMD1: case Reg.IND_CMD2:
            case Reg.IND_CMD3: case Reg.IND_CMD4: case Reg.IND_CMD5:
            case Reg.IND_CMD6: case Reg.IND_CMD7: case Reg.IND_CMD8:
            case Reg.IND_CMD9: case Reg.IND_CMDA: case Reg.IND_CMDB:
            case Reg.IND_CMDC: case Reg.IND_CMDD: case Reg.IND_CMDE:
            case Reg.IND_CMDF:
                this['IND_CMD'+hex(reg-Reg.IND_CMD0)] = {
                    FB:   (val >> 20) & 0x01, //addprev
                    LB:   (val >> 19) & 0x01, //utclod
                    TW:   (val >> 16) & 0x07, //Wrap T
                    SW:   (val >> 13) & 0x07, //Wrap S
                    M:    (val >>  9) & 0x0F, //Matrix ID
                    BS:   (val >>  7) & 0x03, //Alpha Selection
                    BIAS: (val >>  4) & 0x07,
                    FMT:  (val >>  2) & 0x03, //Format
                    BT:   (val >>  0) & 0x03, //Indirect Tex Stage ID
                };
                break;
            case Reg.SU_SCIS0:
                this.scissor.x0 = (val >> 12) & 0x0FFF;
                this.scissor.y0 = (val >>  0) & 0x0FFF;
                break;
            case Reg.SU_SCIS1:
                this.scissor.x1 = (val >> 12) & 0x0FFF;
                this.scissor.y1 = (val >>  0) & 0x0FFF;
                break;
            case Reg.SU_LPSIZE:
                this.SU_LPSIZE = {
                    LINEASPECT: (val >> 22) & 0x01,
                    PTOFF:      (val >> 19) & 0x07,
                    LTOFF:      (val >> 16) & 0x07,
                    PSIZE:      (val >>  8) & 0xFF,
                    LSIZE:      (val >>  0) & 0xFF,
                };
                break;
            case Reg.RAS1_SS0: //ind tex coord scale 0
            case Reg.RAS1_SS1: //ind tex coord scale 1
                this['RAS1_SS'+(reg-Reg.RAS1_SS0)] = {
                    TS1: (val >> 12) & 0x0F, SS1: (val >>  8) & 0x0F,
                    TS0: (val >>  4) & 0x0F, SS0: (val >>  0) & 0x0F,
                }
                break;
            case Reg.RAS1_IREF:
                this.RAS1_IREF = {
                    //IndTex Stage 3 NTexCoord, IndTex Stage 3 NTexMap
                    BC3: (val >> 21) & 0x07, BI3: (val >> 18) & 0x07,
                    BC2: (val >> 15) & 0x07, BI2: (val >> 12) & 0x07,
                    BC1: (val >>  9) & 0x07, BI1: (val >>  6) & 0x07,
                    BC0: (val >>  3) & 0x07, BI0: (val >>  0) & 0x07,
                };
                break;
            case Reg.RAS1_TREF0:
            case Reg.RAS1_TREF1:
            case Reg.RAS1_TREF2:
            case Reg.RAS1_TREF3:
            case Reg.RAS1_TREF4:
            case Reg.RAS1_TREF5:
            case Reg.RAS1_TREF6:
            case Reg.RAS1_TREF7:
                this['RAS1_TREF'+(reg-Reg.RAS1_TREF0)] = {
                    //3 bits padding
                    CC1:  (val >> 19) & 0x07, //Ind Tex Stage 1 Channel ID
                    TE1:  (val >> 18) & 0x01, //Ind Tex Stage 1 TexMap Enable
                    TC1:  (val >> 15) & 0x07, //Ind Tex Stage 1 TexCoord
                    TI1:  (val >> 12) & 0x07, //Ind Tex Stage 1 TexMap
                    //3 bits padding
                    CC0:  (val >>  7) & 0x07, //Ind Tex Stage 0 ...
                    TE0:  (val >>  6) & 0x01,
                    TC0:  (val >>  3) & 0x07,
                    TI0:  (val >>  0) & 0x07,
                };
                break;
            case Reg.SU_SSIZE0: case Reg.SU_SSIZE1: case Reg.SU_SSIZE2:
            case Reg.SU_SSIZE3: case Reg.SU_SSIZE4: case Reg.SU_SSIZE5:
            case Reg.SU_SSIZE6: case Reg.SU_SSIZE7:
                this['SU_SSIZE'+(reg-Reg.SU_SSIZE0)] = {
                    PF:    (val >> 19) & 0x0001, //texcoord offset for points enable
                    LF:    (val >> 18) & 0x0001, //texcoord offset for lines enable
                    WS:    (val >> 17) & 0x0001, //s-cylindrical texcoord wrapping enable
                    BS:    (val >> 18) & 0x0001, //s-range bias enable
                    SSIZE: (val >>  0) & 0xFFFF, //s-scale value -1 (U16)
                };
                break;
            case Reg.SU_TSIZE0: case Reg.SU_TSIZE1: case Reg.SU_TSIZE2:
            case Reg.SU_TSIZE3: case Reg.SU_TSIZE4: case Reg.SU_TSIZE5:
            case Reg.SU_TSIZE6: case Reg.SU_TSIZE7:
                this['SU_TSIZE'+(reg-Reg.SU_TSIZE0)] = {
                    WT:    (val >> 17) & 0x0001, //t-cylindrical texcoord wrapping enable
                    BT:    (val >> 18) & 0x0001, //t-range bias enable
                    TSIZE: (val >>  0) & 0xFFFF, //t-scale value -1 (U16)
                };
                break;
            case Reg.PE_ZMODE:
                this.PE_ZMODE = {
                    MASK:   (val >> 4) & 0x01, //update enable
                    FUNC:   (val >> 1) & 0x07, //Z-buffer compare function
                    ENABLE: (val >> 0) & 0x01, //Z-buffer enable
                };
                break;
            case Reg.PE_CMODE0:
                this.PE_CMODE0 = {
                    LOGICOP:        (val >> 12) & 0x0F,
                    BLENDOP:        (val >> 11) & 0x01,
                    SFACTOR:        (val >>  8) & 0x07,
                    DFACTOR:        (val >>  5) & 0x07,
                    ALPHA_MASK:     (val >>  4) & 0x01,
                    COLOR_MASK:     (val >>  3) & 0x01,
                    DITHER_ENABLE:  (val >>  2) & 0x01,
                    LOGICOP_ENABLE: (val >>  1) & 0x01,
                    BLEND_ENABLE:   (val >>  0) & 0x01,
                };
                break;
            case Reg.PE_CMODE1:
                this.PE_CMODE1 = {
                    CONSTANT_ALPHA_ENABLE: (val >> 8) & 0x01,
                    CONSTANT_ALPHA:        (val >> 0) & 0xFF,
                };
                break;
            case Reg.PE_CONTROL:
                this.PE_CONTROL = {
                    ZCOMPLOC: (val >> 6) & 0x01, //1:before tex
                    ZFMT:     (val >> 3) & 0x07,
                    PIXFMT:   (val >> 0) & 0x07,
                };
                break;
            case Reg.PE_SCISSOR_BOX_OFFS:
                this.scissor.yOffs = (val >> 10) & 0x03FF;
                this.scissor.xOffs = (val >>  0) & 0x03FF;
                break;
            case Reg.TX_SETMODE0_I0:
            case Reg.TX_SETMODE0_I1:
            case Reg.TX_SETMODE0_I2:
            case Reg.TX_SETMODE0_I3:
            case Reg.TX_SETMODE0_I4:
            case Reg.TX_SETMODE0_I5:
            case Reg.TX_SETMODE0_I6:
            case Reg.TX_SETMODE0_I7:
                this['TX_SETMODE0_I'+(reg-Reg.TX_SETMODE0_I0)] = {
                    LODCLAMP: (val >> 21) & 0x0001, //"LODCLAMP/BIASCLAMP"
                    MAXANSO:  (val >> 19) & 0x0003, //1, 2, 4, unused (2,4 require edge LOD)
                    LODBIAS:  (val >>  9) & 0x03FF, //"s2.5"
                    DIAGLOD:  (val >>  8) & 0x0001, //"DIAGLOAD" probably typo
                    MINFILTER:(val >>  5) & 0x0007, //near, near-mip-near, near-mip-in, unused, linear, lin-mip-near, lin-mip-lin, unused
                    MAGFILTER:(val >>  4) & 0x0001, //near, linear
                    WRAP_T:   (val >>  2) & 0x0003, //clamp, repeat, mirror, unused
                    WRAP_S:   (val >>  0) & 0x0003,
                }
                //repeat/mirrir requires the texture size to be a power of two.
                //(wrapping is implemented by a logical AND (SIZE-1))
                break;
            case Reg.TX_SETMODE1_I0: case Reg.TX_SETMODE1_I1:
            case Reg.TX_SETMODE1_I2: case Reg.TX_SETMODE1_I3:
            case Reg.TX_SETMODE1_I4: case Reg.TX_SETMODE1_I5:
            case Reg.TX_SETMODE1_I6: case Reg.TX_SETMODE1_I7:
                this['TX_SETMODE1_I'+(reg-Reg.TX_SETMODE1_I0)] = {
                    MAX_LOD: (val >>  8) & 0xFF, //"U4.4"
                    MIN_LOD: (val >>  0) & 0xFF, //"U4.4"
                };
                break;
            case Reg.TX_SETIMAGE0_I0: case Reg.TX_SETIMAGE0_I1:
            case Reg.TX_SETIMAGE0_I2: case Reg.TX_SETIMAGE0_I3:
            case Reg.TX_SETIMAGE0_I4: case Reg.TX_SETIMAGE0_I5:
            case Reg.TX_SETIMAGE0_I6: case Reg.TX_SETIMAGE0_I7:
                this['TX_SETIMAGE0_I'+(reg-Reg.TX_SETIMAGE0_I0)] = {
                    FMT:    (val >> 20) & 0x000F,
                    HEIGHT: (val >> 10) & 0x03FF,
                    WIDTH:  (val >>  0) & 0x03FF,
                };
                break;
            case Reg.TEV_COLOR_ENV_0: case Reg.TEV_COLOR_ENV_1:
            case Reg.TEV_COLOR_ENV_2: case Reg.TEV_COLOR_ENV_3:
            case Reg.TEV_COLOR_ENV_4: case Reg.TEV_COLOR_ENV_5:
            case Reg.TEV_COLOR_ENV_6: case Reg.TEV_COLOR_ENV_7:
            case Reg.TEV_COLOR_ENV_8: case Reg.TEV_COLOR_ENV_9:
            case Reg.TEV_COLOR_ENV_A: case Reg.TEV_COLOR_ENV_B:
            case Reg.TEV_COLOR_ENV_C: case Reg.TEV_COLOR_ENV_D:
            case Reg.TEV_COLOR_ENV_E: case Reg.TEV_COLOR_ENV_F:
                this['TEV_COLOR_ENV_'+hex(reg-Reg.TEV_COLOR_ENV_0)] = {
                    DEST:  (val >> 22) & 0x03,
                    SHIFT: (val >> 20) & 0x03,
                    CLAMP: (val >> 19) & 0x01,
                    SUB:   (val >> 18) & 0x01,
                    BIAS:  (val >> 16) & 0x03,
                    SELA:  (val >> 12) & 0x0F,
                    SELB:  (val >>  8) & 0x0F,
                    SELC:  (val >>  4) & 0x0F,
                    SELD:  (val >>  0) & 0x0F,
                };
                break;
            case Reg.TEV_ALPHA_ENV_0: case Reg.TEV_ALPHA_ENV_1:
            case Reg.TEV_ALPHA_ENV_2: case Reg.TEV_ALPHA_ENV_3:
            case Reg.TEV_ALPHA_ENV_4: case Reg.TEV_ALPHA_ENV_5:
            case Reg.TEV_ALPHA_ENV_6: case Reg.TEV_ALPHA_ENV_7:
            case Reg.TEV_ALPHA_ENV_8: case Reg.TEV_ALPHA_ENV_9:
            case Reg.TEV_ALPHA_ENV_A: case Reg.TEV_ALPHA_ENV_B:
            case Reg.TEV_ALPHA_ENV_C: case Reg.TEV_ALPHA_ENV_D:
            case Reg.TEV_ALPHA_ENV_E: case Reg.TEV_ALPHA_ENV_F:
                this['TEV_ALPHA_ENV_'+hex(reg-Reg.TEV_ALPHA_ENV_0)] = {
                    DEST:  (val >> 22) & 0x03,
                    SHIFT: (val >> 20) & 0x03,
                    CLAMP: (val >> 19) & 0x01,
                    SUB:   (val >> 18) & 0x01,
                    BIAS:  (val >> 16) & 0x03,
                    SELA:  (val >> 13) & 0x07,
                    SELB:  (val >> 10) & 0x07,
                    SELC:  (val >>  7) & 0x07,
                    SELD:  (val >>  4) & 0x07,
                    TSWAP: (val >>  2) & 0x03,
                    RSWAP: (val >>  0) & 0x03,
                };
                break;
            case Reg.TEV_REGISTERL_0: case Reg.TEV_REGISTERL_1:
            case Reg.TEV_REGISTERL_2: case Reg.TEV_REGISTERL_3:
                this['TEV_REGISTERL_'+(reg-Reg.TEV_REGISTERL_0)] = {
                    TYPE: (val >> 23) & 0x0001,
                    A:    (val >> 12) & 0x0FFF,
                    R:    (val >>  0) & 0x0FFF,
                };
                break;
            case Reg.TEV_REGISTERH_0: case Reg.TEV_REGISTERH_1:
            case Reg.TEV_REGISTERH_2: case Reg.TEV_REGISTERH_3:
                this['TEV_REGISTERH_'+(reg-Reg.TEV_REGISTERH_0)] = {
                    TYPE: (val >> 23) & 0x0001,
                    G:    (val >> 12) & 0x0FFF,
                    B:    (val >>  0) & 0x0FFF,
                };
                break;
            case Reg.TEV_FOG_PARAM_0: //"a" parameter of the screen to eye space conversion function
                this.TEV_FOG_PARAM_0 = {
                    A_SIGN_SHIFT: (val >> 19) & 0x001F,
                    A_EXPN:       (val >> 11) & 0x01FF,
                    A_MANT:       (val >>  0) & 0x01FF,
                }
                break;
            //TEV_FOG_PARAM_1, 2 are a single number
            case Reg.TEV_FOG_PARAM_3: //fog type
                this.TEV_FOG_PARAM_3 = {
                    FSEL:   (val >> 21) & 0x0007,
                    PROJ:   (val >> 20) & 0x0001,
                    C_SIGN: (val >> 19) & 0x0001,
                    C_EXPN: (val >> 11) & 0x00FF,
                    C_MANT: (val >>  0) & 0x07FF,
                };
                break;
            case Reg.TEV_FOG_COLOR:
                this.TEV_FOG_COLOR = {
                    R: (val >> 16) & 0xFF,
                    G: (val >>  8) & 0xFF,
                    B: (val >>  0) & 0xFF,
                };
                break;
            case Reg.TEV_ALPHAFUNC:
                this.TEV_ALPHAFUNC = {
                    LOGIC: (val >> 22) & 0x03,
                    OP1:   (val >> 19) & 0x07,
                    OP0:   (val >> 16) & 0x07,
                    A1:    (val >>  8) & 0xFF,
                    A0:    (val >>  0) & 0xFF,
                };
                break;
            //TEV_Z_ENV_0 is a single number
            case Reg.TEV_Z_ENV_1:
                this.TEV_Z_ENV_1 = {
                    OP:   (val >> 2) & 0x03, //disable, add, replace
                    TYPE: (val >> 0) & 0x03, //u8, u16, u24
                };
                break;
            case Reg.TEV_KSEL_0: case Reg.TEV_KSEL_1: case Reg.TEV_KSEL_2:
            case Reg.TEV_KSEL_3: case Reg.TEV_KSEL_4: case Reg.TEV_KSEL_5:
            case Reg.TEV_KSEL_6: case Reg.TEV_KSEL_7:
                this['TEV_KSEL_'+(reg-Reg.TEV_KSEL_0)] = {
                    KASEL1: (val >> 19) & 0x1F,
                    KCSEL1: (val >> 14) & 0x1F,
                    KASEL0: (val >>  9) & 0x1F,
                    KCSEL0: (val >>  4) & 0x1F,
                    XGA:    (val >>  2) & 0x03,
                    XRB:    (val >>  0) & 0x03,
                };
                break;
            //default: do nothing, only store the raw value
        }
    }
}
