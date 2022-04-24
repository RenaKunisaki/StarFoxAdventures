import { NotImplementedError } from "../../../errors.js";
import RenderBatch from "../gx/RenderBatch.js";

const PI_OVER_180 = Math.PI / 180;

export default class Model {
    /** Base class for a 3D model that has its own orientation
     *  and location in space.
     *  This isn't necessarily used for everything we render;
     *  mainly it's for our built-in "helper" models.
     */
    constructor(gx, pos=[0,0,0], rot=[0,0,0], scale=[1,1,1]) {
        /** Construct Model.
         *  @param {GX} gx GX instance to use.
         *  @param {Array} pos Model position in 3D environment.
         *  @param {Array} rot Model rotation on X, Y, Z axes, in radians.
         *  @param {Array} scale Model scale on X, Y, Z axes.
         */
        this.gx      = gx;
        this.gl      = gx.gl;
        this.pos     = vec3.fromValues(...pos);
        this.rot     = vec3.fromValues(...rot);
        this.scale   = vec3.fromValues(...scale);
        this.opacity = 1;
        this.mtx     = null;
        this.id      = -1;
        this._needsUpdate = true;
    }

    get batch() {
        if(this._needsUpdate) this._update();
        this._needsUpdate = false;
        return this._batch;
    }

    setMatrix(m) {
        /** Set the transformation matrix.
         *  @param {mat4} m The matrix to use, or null for none.
         */
        this.mtx = m;
        this._needsUpdate = true;
        return this;
    }
    setPos(x, y, z) {
        this.pos[0] = x; this.pos[1] = y; this.pos[2] = z;
        this._needsUpdate = true;
        return this;
    }
    moveBy(x, y, z) {
        this.pos[0] += x; this.pos[1] += y; this.pos[2] += z;
        this._needsUpdate = true;
        return this;
    }
    setRot(x, y, z) {
        /** Set the model's rotation.
         *  @param {number} x Model rotation on X axis, in radians.
         *  @param {number} y Model rotation on Y axis, in radians.
         *  @param {number} z Model rotation on Z axis, in radians.
         *  @returns {Model} this.
         */
        this.rot[0] = x; this.rot[1] = y; this.rot[2] = z;
        this._needsUpdate = true;
        return this;
    }
    setRotDegrees(x, y, z) {
        /** Set the model's rotation using degrees.
         *  @param {number} x Model rotation on X axis, in degrees.
         *  @param {number} y Model rotation on Y axis, in degrees.
         *  @param {number} z Model rotation on Z axis, in degrees.
         *  @returns {Model} this.
         *  @note This is equivalent to `setRot(x * (Math.PI/180)),
         *   y * (Math.PI/180)), z * (Math.PI/180)))`.
         */
        this.rot[0] = x * PI_OVER_180;
        this.rot[1] = y * PI_OVER_180;
        this.rot[2] = z * PI_OVER_180;
        this._needsUpdate = true;
        return this;
    }
    setScale(x, y, z) {
        /** Set the model's scale.
         *  @param {number} x Model scale on X axis.
         *  @param {number} y Model scale on Y axis.
         *  @param {number} z Model scale on Z axis.
         *  @returns {Model} this.
         */
        this.scale[0] = x; this.scale[1] = y; this.scale[2] = z;
        this._needsUpdate = true;
        return this;
    }
    setOpacity(opacity) {
        /** Set the model's opacity.
         *  @param {number} Opacity, from 0 (invisible) to 1 (opaque).
         */
        this.opacity = opacity;
        this._needsUpdate = true;
        return this;
    }
    setId(id) {
        /** Set the model's picker ID.
         *  @param {integer} id Vertex ID value for picker.
         *  @returns {Model} this.
         */
        this.id = id;
        this._needsUpdate = true;
        return this;
    }

    render() {
        /** Render the model. */
        if(this._needsUpdate) this._update();
        this._needsUpdate = false;
        this.gx.executeBatch(this._batch);
    }

    _update() {
        /** Recalculate buffers. Called after geometry changes. */
        //this._batch = new RenderBatch(gx);
        //subclass should replace this method
        return this;
    }
}
