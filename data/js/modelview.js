/** Main script for model viewer.
 */
import {E} from './Element.js';
import {get, getObjects, getGameTexts, getMaps, getBits, finishLoading, setupMain} from './Util.js';
import Context from './gl/Context.js';
import ViewControl from './gl/ViewControl.js';
import LightControl from './gl/LightControl.js';
import Picker from './gl/Picker.js';
import AssetLoader from './gl/sfa/AssetLoader.js';
import ModelRenderer from './gl/sfa/model/ModelRenderer.js';
//import ModelDataView from './ModelDataView.js';
import TextureView from './gl/TextureView.js';

let gl; //Context

class Renderer {
    constructor(gx, model) {
        this.gx    = gx;
        this.model = model;
        this._renderer = new ModelRenderer(gx, model);
    }

    render() {
        this._renderer.render();
        this._renderer.renderBones();
    }

    renderPickBuffer() {
        this._renderer.render();
    }
}

class ModelViewer {
    /** Handles loading and displaying SFA models.
     */
    constructor() {
        this.gl = new Context('#glCanvas');
        this.controls = document.getElementById('controls');

        this.viewControl = new ViewControl(this.gl);
        this.controls.append(this.viewControl.element);

        this.lightControl = new LightControl(this.gl);
        this.controls.append(this.lightControl.element);

        this.picker = new Picker(this.gl);
        this.controls.append(this.picker.element);

        this.textureView = new TextureView(this.gl);
        this.controls.append(this.textureView.element);
    }

    async init() {
        await this.gl.loadPrograms();
    }

    async showModel(model) {
        /** Display SFA model.
         */
        this._renderer = new Renderer(this.gl.gx, model);
        this.gl.setRenderer(this._renderer);
        this.gl.redraw();
        this.gl.gx.printStats();
        console.log("Model", model);
        this.textureView.setModel(model);
        //this.dataView = new ModelDataView(model);
        //this.controls.append(this.dataView.element);
        //this.dataView.refresh();
        /* for(let i=0; i<model.header.nBones; i++) {
            const bone = model.bones[i];
            console.log("Bone %d parent=%d unk=%d,%d,%d head=%f,%f,%f tail=%f,%f,%f ->",
                i, bone.parent, bone.unk01, bone.unk02, bone.unk03,
                bone.head[0], bone.head[1], bone.head[2],
                bone.tail[0], bone.tail[1], bone.tail[2],
                model.calcBonePos(bone, true),
            );
        } */
        /* for(let i=0; i<model.header.nVtxGroups; i++) {
            const group = model.vtxGroups[i];
            console.log("Vgrp[%d]: bone=%d,%d wgt=%d (%f / %f) ?=%d", i,
                group.bone0, group.bone1, group.weight,
                group.weight/4, 1-(group.weight/4),
                group.unk03);
        } */
        /* const buf = new DataBuffer(model.rawVtxs);
        for(let i=0; i<model.header.nVtxs; i++) {
            const offs = i*6;
            const x    = buf.nextS16();
            const y    = buf.nextS16();
            const z    = buf.nextS16();
            console.log("V[%d @0x%s]: %d, %d, %d", i, offs.toString(16),x,y,z);
        } */
    }
}

function main() {
    const viewer = new ModelViewer();
    viewer.init().then(() => {
        let id = window.location.hash;
        if(id == null || id.length < 1) id = 0x4E8;
        else id = parseInt(id.substr(1), 16);
        const loader = new AssetLoader(viewer.gl.gx);
        loader.loadModel(id).then(model => {
            viewer.showModel(model);
        })
    });
    //gl.showModel('krystalmodel.bin');
    //gl.showModel('model0.bin');
}
setupMain(main);
