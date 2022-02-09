import { E } from "../../lib/Element.js";
import { hex, int, Percent, fileSize } from "../../Util.js";
import Table from "./Table.js";

export default class FileList {
    /** Displays list of files in ISO.
     */
    constructor(app) {
        this.app = app;
        this.element = document.getElementById('tab-fileList');
        this.app.onIsoLoaded(iso => this.refresh(iso));
    }

    refresh(iso) {
        console.log("files", iso.files);
        let [eList, iFile] = this._makeElemForDir(iso, 0);
        const elem = E.div('fileList', eList);
        this.element.replaceWith(elem);
        this.element = elem;
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
                list.append(E.li('file', eFile));
                iFile = nextIdx;
            }
            else {
                list.append(E.li('file',
                    E.span('name', file.name),
                    E.span('fileSize int', fileSize(file.size), {title:file.size}),
                ));
                iFile++;
            }
        }
        return [elem, iFile];
    }
}
