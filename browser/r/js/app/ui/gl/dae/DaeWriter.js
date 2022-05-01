import { E } from "../../../../lib/Element.js";

const NS = {
    dae:   'http://www.collada.org/2005/11/COLLADASchema',
    html:  'http://www.w3.org/1999/xhtml',
    svg:   'http://www.w3.org/2000/svg',
    xlink: 'http://www.w3.org/1999/xlink',
    xml:   'http://www.w3.org/XML/1998/namespace',
    xmlns: 'http://www.w3.org/2000/xmlns/',
};

export default class DaeWriter {
    /** Writes DAE files. */
    constructor() {
        this.cameras       = {}; //library_cameras
        this.lights        = {}; //library_lights
        this.effects       = {}; //library_effects
        this.images        = {}; //library_images
        this.materials     = {}; //library_materials
        this.geometries    = {}; //library_geometries
        this.visual_scenes = {}; //library_visual_scenes
    }

    addGeometry(id, geom) {
        this.geometries[id] = geom;
    }

    toXml() {
        this.xml = document.implementation.createDocument(NS.dae, "COLLADA");
        this.xml.documentElement.setAttribute('version', "1.4.1");
        this.xml.documentElement.setAttributeNS(NS.xmlns, 'xmlns:xsi',
            "http://www.w3.org/2001/XMLSchema-instance");

        const scene = E.visual_scene({id:'Scene', name:'Scene'});

        //this.xml.documentElement.appendChild(E.library_cameras(this.cameras));
        //this.xml.documentElement.appendChild(E.library_lights(this.lights));
        //this.xml.documentElement.appendChild(E.library_effects(this.effects));
        //this.xml.documentElement.appendChild(E.library_images(this.images));
        //this.xml.documentElement.appendChild(E.library_materials(this.materials));

        const eGeometries = E.library_geometries();
        for(const [id, geom] of Object.entries(this.geometries)) {
            eGeometries.append(E.geometry({
                id:   id+'.geometry',
                name: id,
            }, geom));
            scene.append(E.node(
                {id:id, name:id, type:'NODE'},
                E.matrix(null, {sid:'transform'},
                    "1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1"
                ),
                E.instance_geometry(null, {url:'#'+id+'.geometry'}),
                //XXX bind_material
            ));
        }
        this.xml.documentElement.appendChild(eGeometries);
        this.xml.documentElement.appendChild(E.library_visual_scenes(scene));
        this.xml.documentElement.appendChild(E.scene(
            E.instance_visual_scene({url:'#Scene'}),
        ));

        return this.xml;
    }
}
