import { clearElement, E } from "../../../lib/Element.js";

export default class LayerChooser {
    /** Widget that lets you choose what to display. */
    constructor(mapViewer) {
        this._nextId   = 0;
        this.mapViewer = mapViewer;
        this.game      = mapViewer.game;
        this.app       = mapViewer.game.app;
        this.element   = E.details('map-layers', {open:'open'},
            E.summary(null, "Layers"),
            this._makeElems(),
        );
    }

    refresh() {
        const objCounts = []; //act# => obj count
        for(let i=0; i<16; i++) objCounts.push(0);
        let objs = this.mapViewer.map.romList;
        if(objs) objs = objs.entries;
        if(!objs) objs = [];
        for(let obj of objs) {
            for(let i=1; i<16; i++) {
                if(obj.acts[i]) objCounts[i]++;
            }
        }

        const oldAct = this.eActList.value;
        clearElement(this.eActList);
        this.eActList.append(E.option(null, "None", {value:0}));
        for(let i=1; i<16; i++) {
            this.eActList.append(E.option(null,
                `Act ${i} (${objCounts[i]})`, {value:i}));
        }
        this.eActList.append(E.option(null,
            `All Acts (${objs.length})`, {value:-1}));

        //when list is empty, value is the empty string
        if(oldAct !== '') this.eActList.value = oldAct;
    }

    _makeCheck(name, cbFunc, title=null, checked=false) {
        /** Make a checkbox.
         *  @param {string} name The name text to display in the label.
         *  @param {function} cbFunc The function to call when clicked.
         *  @param {string} title The tooltip text.
         *  @returns {HtmlDivElement} The element.
         */
        const id   = 'mapview-layers-check-' + (this._nextId++);
        const eBox = E.input(null, {type:'checkbox', id:id});
        const eLbl = E.label(null, {'for':id}, name);
        const eDiv = E.div('checkbox', eBox, eLbl);
        if(title != null) eDiv.setAttribute('title', title);
        if(checked) eBox.checked = true;
        eBox.addEventListener('change', cbFunc);
        return eDiv;
    }

    _makeElems() {
        /** Build the elements for this widget. */
        const L = this.mapViewer.layers;

        this.eActList = E.select();
        this.eActList.addEventListener('change', e => {
            this.mapViewer.layers.actNo = this.eActList.value;
            this.mapViewer.redraw();
        });

        return E.div(
            this._makeCheck("Main Geometry",
                e => this._toggleMainGeometry(),
                "Non-translucent, non-reflective polygons", L.mainGeometry),
            this._makeCheck("Water Geometry",
                e => this._toggleWaterGeometry(),
                "Translucent polygons", L.waterGeometry),
            this._makeCheck("Reflective Geometry",
                e => this._toggleReflectiveGeometry(),
                "Opaque reflective polygons", L.reflectiveGeometry),
            this._makeCheck("Hidden Geometry",
                e => this._toggleHiddenGeometry(),
                "Polygons normally not shown in-game", L.hiddenGeometry),
            E.div(null, "Objects:", this.eActList),
            this._makeCheck("Hidden Objects",
                e => this._toggleHiddenObjects(),
                "Objects normally not shown in-game", L.hiddenObjects),
        );
    }

    _toggleMainGeometry() {
        this.mapViewer.layers.mainGeometry = !this.mapViewer.layers.mainGeometry;
        this.mapViewer.redraw();
    }
    _toggleWaterGeometry() {
        this.mapViewer.layers.waterGeometry = !this.mapViewer.layers.waterGeometry;
        this.mapViewer.redraw();
    }
    _toggleReflectiveGeometry() {
        this.mapViewer.layers.reflectiveGeometry = !this.mapViewer.layers.reflectiveGeometry;
        this.mapViewer.redraw();
    }
    _toggleHiddenGeometry() {
        this.mapViewer.layers.hiddenGeometry = !this.mapViewer.layers.hiddenGeometry;
        this.mapViewer.redraw();
    }
    _toggleHiddenObjects() {
        this.mapViewer.layers.hiddenObjects = !this.mapViewer.layers.hiddenObjects;
        this.mapViewer.redraw();
    }
}
