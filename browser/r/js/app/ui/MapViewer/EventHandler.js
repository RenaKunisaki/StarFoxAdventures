const PI_OVER_180 = Math.PI / 180.0; //rad = deg * PI_OVER_180
//const PI_UNDER_180 = 180.0 / Math.PI; //deg = rad * PI_UNDER_180

export default class EventHandler {
    /** Handles events for the MapViewer canvas. */
    constructor(mapViewer) {
        this.mapViewer = mapViewer;
        this.canvas = this.mapViewer.canvas;
        this._prevMousePos = [0, 0];
        this._mouseStartPos = null;
        this._moveSpeedModifier = 1.0;
        this._deltaTick = 1000.0 / 30.0; // 30 fps
        this._keyPressMap = {};

        const canvas = this.canvas;
        canvas.addEventListener('mousemove', e => this._onMouseMove(e));
        canvas.addEventListener('mousedown', e => this._onMouseDown(e));
        canvas.addEventListener('mouseup', e => this._onMouseUp(e));
        canvas.addEventListener('wheel', e => this._onMouseWheel(e));
        //must disable context menu to be able to right-drag.
        //can still use alt+right to open it.
        canvas.addEventListener('contextmenu', e => {
            if (!e.altKey) e.preventDefault();
        });

        //XXX move some of this to a generic class?
        canvas.addEventListener('keydown', e => this._onKey(e, true));
        canvas.addEventListener('keyup', e => this._onKey(e, false));

        var t = this;
        setInterval(function () { t._tick(); }, this._deltaTick);
    }

    _tick() {
        if (!this.mapViewer || !this.mapViewer.viewController) {
            return;
        }

        var deltaTime = this._deltaTick / 1000.0
        var baseSpeed = 350.0;
        var speed = baseSpeed * this._moveSpeedModifier;

        if (this._keyPressMap[' ']) {
            this.mapViewer.viewController.adjust({ pos: { y: speed * deltaTime } });
        }

        var movement = { x: 0.0, y: 0.0 };

        if (this._keyPressMap["ArrowLeft"] || this._keyPressMap["a"]) {
            movement.x = speed * deltaTime;
        }
        if (this._keyPressMap["ArrowRight"] || this._keyPressMap["d"]) {
            movement.x = -speed * deltaTime;
        }
        if (this._keyPressMap["ArrowUp"] || this._keyPressMap["w"]) {
            movement.y = speed * deltaTime;
        }
        if (this._keyPressMap["ArrowDown"] || this._keyPressMap["s"]) {
            movement.y = -speed * deltaTime;
        }

        if (movement.x != 0.0 || movement.y != 0.0) {
            this._moveByVector(movement);
        }
    }

    _moveByVector(vec) {
        const view = this.mapViewer.viewController.get();
        const rx = ((view.rot.x % 360) - 180) * PI_OVER_180;
        const ry = ((view.rot.y % 360) - 180) * PI_OVER_180;

        const sinRX = Math.sin(rx);
        const cosRX = Math.cos(rx);
        const sinRY = Math.sin(ry);
        const cosRY = Math.cos(ry);

        const deltaX = ((vec.x * cosRY) - (vec.y * sinRY));
        const deltaY = vec.y * sinRX;
        const deltaZ = ((vec.x * sinRY) + (vec.y * cosRY));

        this.mapViewer.viewController.adjust({ pos: { x: deltaX, y: deltaY, z: deltaZ } });
    }

    async _onMouseDown(event) {
        if (event.buttons == 1) {
            const obj = await this.mapViewer._getObjAt(event.clientX, event.clientY);
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
        if (event.buttons == 2) { //rotate
            if (this._mouseStartView) {
                this.mapViewer.viewController.set({
                    rot: {
                        x: this._mouseStartView.rot.x + (event.y - this._mouseStartPos[1]),
                        y: this._mouseStartView.rot.y + (event.x - this._mouseStartPos[0]),
                    },
                });
            }
            else {
                this._mouseStartView = this.mapViewer.viewController.get();
                this._mouseStartPos = [event.x, event.y];
            }
        }
        else if (event.buttons == 4) { //move
            if (this._mouseStartView) {
                this._doMouseCamMove(event);
            }
            else {
                this._mouseStartView = this.mapViewer.viewController.get();
                this._mouseStartPos = [event.x, event.y];
            }
        }
        else {
            this._mouseStartView = null;
            //const obj = this.mapViewer._getObjAt(event.clientX, event.clientY);
            //this.mapViewer.infoWidget.show(obj);
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

        this.mapViewer.viewController.set({ pos: { x: x, z: z } });
    }

    _onMouseWheel(event) {
        event.preventDefault();
        const clamp = (num, min, max) => Math.min(Math.max(num, min), max);
        var deltaSpeed = - event.deltaY / 500.0;
        this._moveSpeedModifier = clamp(this._moveSpeedModifier + deltaSpeed, 0.25, 5.0)
    }

    _onKey(event, isDown) {
        const locations = {
            [KeyboardEvent.DOM_KEY_LOCATION_STANDARD]: '',
            [KeyboardEvent.DOM_KEY_LOCATION_LEFT]: 'L_',
            [KeyboardEvent.DOM_KEY_LOCATION_RIGHT]: 'R_',
            [KeyboardEvent.DOM_KEY_LOCATION_NUMPAD]: 'KP_',
        };
        const code = [
            locations[event.location],
            event.key,
            event.shiftKey ? '_Shift' : '',
            event.ctrlKey ? '_Ctrl' : '',
            event.altKey ? '_Alt' : '',
            event.metaKey ? '_Meta' : '',
            isDown ? '_Press' : '_Release'].join('');
        console.log("KEY EVENT", code, event);

        this._keyPressMap[event.key] = isDown;

        const C = this.mapViewer.layerChooser;
        switch (code) {
            case 'KP_8_Press':
                //put camera at top looking down or something
                break;
            case 'g_Press':
                C.toggleLayer('waterGeometry');
                break;
            case 'b_Press':
                C.toggleLayer('blockBounds');
                break;
            case 'h_Press':
                C.toggleLayer('hiddenGeometry');
                break;
            case 'm_Press':
                C.toggleLayer('mainGeometry');
                break;
            case 't_Press':
                C.toggleLayer('reflectiveGeometry');
                break;
            case 'p_Press':
                C.toggleLayer('warps');
                break;
        }
    }
}
