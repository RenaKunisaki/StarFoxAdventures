import { E, clearElement } from "../../lib/Element.js";
import { fileSize, hex } from "../../Util.js";

export default class FileList {
    /** Displays list of files in ISO.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-fileList');
        this.app.onIsoLoaded(iso => this.refresh(iso));
    }

    refresh(iso) {
        //console.log("files", iso.files);
        //make the actual file list element
        let [eList, iFile] = this._makeElemForDir(iso, 0);

        //make the right pane to show file info
        this.eRightPane = E.div('rightPane', "");

        const elem = E.div('fileList',
            E.div('list', E.h1(null, "Files"), eList),
            this.eRightPane);
        this.element.replaceWith(elem);
        this.element = elem;
    }

    _showFile(file) {
        clearElement(this.eRightPane);
        const eView = E.button(null, "View in New Window");
        this.eRightPane.append(
            E.h1(null, file.name),
            E.table('fileInfo',
                E.tr(E.th(null, "Index"), E.td('index int', file.idx.toLocaleString())),
                E.tr(E.th(null, "Size"), E.td('fileSize int', file.size.toLocaleString())),
                E.tr(E.th(null, "Offset"), E.td('offset hex', `0x${hex(file.bufferOffs)}`)),
            ),
            eView,
        );

        eView.addEventListener('click', async e => {
            const win = await this.app.openChildWindow();
            console.log("Opened window", win);
            if(win) win.app.showFile(file);
        })
    }

    _makeElemForDir(iso, iFile, _depth=0) {
        if(_depth > 10) throw new Error("Maximum depth exceeded");
        const parent = iso.files[iFile++];
        const list   = E.ul('dir');
        let elem;
        if(_depth == 0) elem = list;
        else elem = E.details('dir',
            E.summary('name', parent.name),
            list,
        );
        while(iso.files[iFile]) {
            let file = iso.files[iFile];
            if(file.parent != parent) break;
            if(file.isDir) {
                let [eFile, nextIdx] = this._makeElemForDir(iso, iFile, _depth+1);
                list.append(E.li('dir', eFile));
                iFile = nextIdx;
            }
            else {
                const eFile = E.li('file',
                    E.span('name', file.name),
                    E.span('fileSize int', fileSize(file.size), {title:file.size}),
                );
                eFile.addEventListener('click', e => this._showFile(file));
                list.append(eFile);
                iFile++;
            }
        }
        return [elem, iFile];
    }
}
