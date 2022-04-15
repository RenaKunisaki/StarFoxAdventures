const PI_OVER_180 = Math.PI / 180.0; //rad = deg * PI_OVER_180
//const PI_UNDER_180 = 180.0 / Math.PI; //deg = rad * PI_UNDER_180

export default class EventHandler {
    /** Handles events for the MapViewer canvas. */
    constructor(mapViewer) {
        this.mapViewer      = mapViewer;
        this.canvas         = this.mapViewer.canvas;
        this._prevMousePos  = [0, 0];
        this._mouseStartPos = null;

        this.canvas.addEventListener('mousemove', e => this._onMouseMove (e));
        this.canvas.addEventListener('mousedown', e => this._onMouseDown (e));
        this.canvas.addEventListener('mouseup',   e => this._onMouseUp   (e));
        this.canvas.addEventListener('wheel',     e => this._onMouseWheel(e));
        //must disable context menu to be able to right-drag
        this.canvas.addEventListener('contextmenu', e => e.preventDefault());
    }

    _onMouseDown(event) {
        if(event.buttons == 1) {
            const obj = this.mapViewer._getObjAt(event.clientX, event.clientY);
            this.mapViewer.infoWidget.show(obj);
        }
        else {
            event.preventDefault();
            this._mouseStartView = null;
        }
    }
    _onMouseUp(event) {
        event.preventDefault();
        this._mouseStartView = null;
    }

    _onMouseMove(event) {
        //buttons are bitflag: 1=left 2=right 4=mid 8=back 16=fwd
        //viewController.set() will redraw the scene.
        if(event.buttons == 4) { //rotate
            if(this._mouseStartView) {
                this.mapViewer.viewController.set({
                    rot: {
                        x: this._mouseStartView.rot.x + (event.y - this._mouseStartPos[1]),
                        y: this._mouseStartView.rot.y + (event.x - this._mouseStartPos[0]),
                    },
                });
            }
            else {
                this._mouseStartView = this.mapViewer.viewController.get();
                this._mouseStartPos  = [event.x, event.y];
            }
        }
        else if(event.buttons == 2) { //move
            if(this._mouseStartView) {
                this._doMouseCamMove(event);
            }
            else {
                this._mouseStartView = this.mapViewer.viewController.get();
                this._mouseStartPos  = [event.x, event.y];
            }
        }
        else {
            this._mouseStartView = null;
        }
        this._prevMousePos = [event.x, event.y];
    }

    _doMouseCamMove(event) {
        const scale = 1;
        const dx = (event.x - this._mouseStartPos[0]) * scale;
        const dz = (event.y - this._mouseStartPos[1]) * scale;

        const view = this.mapViewer.viewController.get();
        const ry = ((view.rot.y % 360) - 180) * PI_OVER_180;
        //const rz = view.rot.z * PI_OVER_180;
        const sinRX = Math.sin(ry);
        const cosRX = Math.cos(ry);
        const x = this._mouseStartView.pos.x + ((dx * cosRX) - (dz * sinRX));
        const z = this._mouseStartView.pos.z + ((dx * sinRX) + (dz * cosRX));

        this.mapViewer.viewController.set({pos: {x:x, z:z}});
    }

    _onMouseWheel(event) {
        this.mapViewer.viewController.adjust({pos:{y: event.deltaY}});
    }
}
