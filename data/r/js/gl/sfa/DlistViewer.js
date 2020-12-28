import {E, clearElement} from '/r/js/Element.js';
import HexViewer from '../HexViewer.js';

export default class DlistViewer {
    /** A UI widget for browsing display lists.
     */
    constructor(gx) {
        this.gx    = gx;
        this.model = null;
        this._createUi();
    }

    setModel(model) {
        this.model = model;
        //this.refresh();
    }

    _createUi() {
        /** Create the UI widgets.
         */
        this.summary = E.summary(null, "Display Lists");
        this.element = E.details('dlist-viewer', this.summary);
        this.element.addEventListener('toggle', e => this._onMainToggle(e));
        this.hexView = new HexViewer();
        document.body.append(this.hexView.element);
    }

    _onMainToggle(event) {
        /** Called when the box is opened or closed.
         */
        if(!this.element.open) return;
        clearElement(this.element);
        this.element.append(this.summary);

        for(let i=0; i<this.model.dlists.length; i++) {
            const dlist = this.model.dlists[i];
            const ptr   = this.model.dlistPtrs[i];
            const idx   = i.toString(16).padStart(2, '0').toUpperCase();
            const offs  = ptr.offset.toString(16).padStart(6, '0').toUpperCase();
            const len   = ptr.size.toString(16).padStart(4, '0').toUpperCase();
            const eSum  = E.summary('dlist', `List 0x${idx} @0x${offs} (0x${len})`);
            const eDet  = E.details('dlist', eSum);
            this.element.append(eDet);
            eDet.addEventListener('toggle', e => this._onListToggle(e, i, eSum, eDet));
        }
    }

    _onListToggle(event, idx, eSum, eDet) {
        /** Called when a dlist box is opened or closed.
         */
        if(!eDet.open) return;
        //const text = eSum.firstChild.textContent;
        clearElement(eDet);
        eDet.append(eSum);

        const list = this.model.dlists[idx];
        const ptr  = this.model.dlistPtrs[idx];
        const unk12 = ptr.unk12.toString(16).padStart(2, '0').toUpperCase();
        const unk16 = ptr.unk16.toString(16).padStart(4, '0').toUpperCase();
        const unk18 = ptr.unk18.toString(16).padStart(8, '0').toUpperCase();
        const specialBitAddr = this._makeOffsetLink(ptr.specialBitAddr);
        const bbox  = `(${ptr.bbox[0]},${ptr.bbox[1]},${ptr.bbox[2]}) (${ptr.bbox[3]},${ptr.bbox[4]},${ptr.bbox[5]})`;
        const ul = E.ul(null,
            E.li(null, `Shader: ${ptr.shaderId}`),
            E.li(null, 'specialBitAddr: ', specialBitAddr),
            E.li(null, `bbox: ${bbox}`),
            E.li(null, `Unk: ${unk12} ${unk16} ${unk18}`),
        );
        for(const op of list.parsedOps) {
            const offs = op.offset.toString(16).padStart(4, '0').toUpperCase();
            let params = [];
            if(op.params) {
                for(const [k,v] of Object.entries(op.params)) {
                    params.push(`${k}:${v}`);
                }
            }
            params = params.join(' ');
            ul.append(E.li('opcode',
                `${offs}: ${op.op} ${params}`,
            ));
        }
        eDet.append(ul);
    }

    _onOffsetClick(event, offset) {
        this.hexView.setData(this.model.rawData, offset).show();
    }

    _makeOffsetLink(offs) {
        const a = E.a('offset',
            '0x'+(offs.toString(16).toUpperCase().padStart(6, '0')));
        a.addEventListener('click', e => this._onOffsetClick(e, offs));
        return a;
    }
}
