import { clearElement, E } from "../../../lib/Element.js";

export default class LayerChooser {
    /** Widget that lets you choose what to display. */
    constructor(mapViewer) {
        this._nextId   = 0;
        this.mapViewer = mapViewer;
        this.game      = mapViewer.game;
        this.app       = mapViewer.game.app;
        this.element   = E.details('map-layers',
            E.summary(null, "Layers"),
            this._makeElems(),
        );
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
        return E.div(
            this._makeCheck("Main Geometry",
                e => this._toggleMainGeometry(),
                "Non-translucent, non-reflective polygons", true),
            this._makeCheck("Water Geometry",
                e => this._toggleWaterGeometry(),
                "Translucent polygons"),
            this._makeCheck("Reflective Geometry",
                e => this._toggleReflectiveGeometry(),
                "Opaque reflective polygons"),
            this._makeCheck("Hidden Geometry",
                e => this._toggleHiddenGeometry(),
                "Polygons normally not shown in-game"),
            this._makeCheck("Objects",
                e => this._toggleObjects(),
                "Object positions"),
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
    _toggleObjects() {
        this.mapViewer.layers.objects = !this.mapViewer.layers.objects;
        this.mapViewer.redraw();
    }
}
