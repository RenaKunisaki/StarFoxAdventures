export default class Block {
    /** One block of a map.
     */
    constructor(app, map, x, z, val) {
        this.app  =  app;
        this.map  =  map;
        this.x    =  x;
        this.z    =  z;
        this.unk1 =  val >> 31;
        this.mod  = (val >> 23) & 0x00FF;
        this.sub  = (val >> 17) & 0x003F;
        this.unk2 =  val        & 0x01FF;
    }
}
