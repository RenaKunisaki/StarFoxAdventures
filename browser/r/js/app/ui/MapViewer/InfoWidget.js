import { clearElement, E } from "../../../lib/Element.js";
import { hex } from "../../../Util.js";

export default class InfoWidget {
    /** Widget displaying info about selected object. */
    constructor(mapViewer) {
        this.mapViewer = mapViewer;
        this.game      = mapViewer.game;
        this.app       = mapViewer.game.app;
        this._tbl      = E.table();
        this.element   = E.details('obj-info', {open:'open'},
            E.summary(null, "Selection"),
            this._tbl,
        );
        this.show(null);
    }

    show(info) {
        /** Display info about an object.
         *  @param {object} obj What to show. Can be null.
         */
        clearElement(this._tbl);
        if(info == null) {
            this._tbl.append(E.tr(
                E.th(null, "Nothing selected"),
            ));
            return;
        }

        console.log("show", info);
        switch(info.type) {
            case 'blockHit':      this._showBlockHit(info);       break;
            case 'collisionMesh': this._showBlockCollision(info); break;
            case 'mapBlockDlist': this._showBlockDlist(info);     break;
            case 'object':        this._showObject(info);         break;
            case 'polyGroup':     this._showBlockPolyGroup(info); break;
            case 'warp':          this._showWarp(info);           break;
            default:
                this._tbl.append(E.tr(
                    E.th('error', `Unknown object type: "${info.type}"`),
                ));
        }
    }

    _showBlockCollision(info) {
        /** Display info about a map block collision triangle.
         *  @param {object} info The block info.
         */
        const block = info.block;
        const poly  = info.poly;
        const rows = [
            E.tr(E.th(null, `Hit Tri #${info.idx}`, {colspan:2})),
            E.tr( E.th(null, "Block"), E.td('string', block.header.name) ),
            E.tr(
                E.th(null, "Vtxs"),
                E.td('string', `${poly.vtxs[0]}, ${poly.vtxs[1]}, ${poly.vtxs[2]}`),
            ),
            E.tr(
                E.th(null, "ID"),
                E.td('hex', `0x${hex(poly._06, 4)}`),
            ),
            E.tr(E.th(null, `Group (#${poly.groupIdx})`, {colspan:2})),
            ...this._makeBlockPolyGroupInfo(block, poly.group),
        ];
        this._tbl.append(...rows);
    }

    _showBlockDlist(info) {
        /** Display info about a map block display list.
         *  @param {object} info The block info.
         */
        const block = info.block;
        const rows = [
            E.tr(E.th(null, `Block "${block.header.name}"`, {colspan:2})),
            E.tr(
                E.th(null, "Y range"),
                E.td('float', `${block.header.yMin}, ${block.header.yMax}`),
            ),
            E.tr(
                E.th(null, "Y offs"),
                E.td('float', block.header.yOffset),
            ),
            E.tr(
                E.th(null, "Stream"),
                E.td('string', info.stream),
            ),
            E.tr(
                E.th(null, "Polygons"),
                E.td('int', block.header.nPolygons),
            ),
            E.tr(
                E.th(null, "PolyGroups"),
                E.td('int', block.header.nPolyGroups),
            ),
            E.tr(
                E.th(null, "Dlists"),
                E.td('int', block.header.nDlists),
            ),
            E.tr(
                E.th(null, "Shaders"),
                E.td('int', block.header.nShaders),
            ),
            E.tr(
                E.th(null, "Textures"),
                E.td('int', block.header.nTextures),
            ),
            E.tr(
                E.th(null, "Vertices"),
                E.td('int', block.header.nVtxs),
            ),
            E.tr(
                E.th(null, "Flags 04"),
                E.td('hex', hex(block.header.flags_0x4, 2)),
            ),
        ];
        this._tbl.append(...rows);
    }

    _showBlockHit(info) {
        /** Display info about a HITS.bin entry.
         *  @param {object} info The block info.
         */
        const hit = info.hit;
        const sx  = Math.abs(hit.x1 - hit.x2);
        const sy  = Math.abs(hit.y1 - hit.y2);
        const sz  = Math.abs(hit.z1 - hit.z2);
        const rows = [
            E.tr(E.th(null, `Hit #${info.idx}`, {colspan:2})),
            E.tr( E.th(null, "Block"), E.td('string', info.block.header.name) ),
            E.tr( E.th(null, "X"), E.td('float', `${hit.x1} - ${hit.x2} (${sx})`) ),
            E.tr( E.th(null, "Y"), E.td('float', `${hit.y1} - ${hit.y2} (${sy})`) ),
            E.tr( E.th(null, "Z"), E.td('float', `${hit.z1} - ${hit.z2} (${sz})`) ),
            E.tr( E.th(null, "Offset"), E.td('hex', `0x${hex(info.offset,6)}`) ),
            E.tr( E.th(null, "Unk0C"), E.td('hex', `0x${hex(hit._0C,2)}`) ),
            E.tr( E.th(null, "Unk0D"), E.td('hex', `0x${hex(hit._0D,2)}`) ),
            E.tr( E.th(null, "Unk0E"), E.td('hex', `0x${hex(hit.flags0E,2)}`) ),
            E.tr( E.th(null, "Unk0F"), E.td('hex', `0x${hex(hit.flags,2)}`) ),
            E.tr( E.th(null, "Points"), E.td('int', `${hit.iPoint[0]}, ${hit.iPoint[1]}`) ),
        ];
        this._tbl.append(...rows);
    }

    _showBlockPolyGroup(info) {
        /** Display info about a map block collision group.
         *  @param {object} info The block info.
         */
        const block = info.block;
        const group = info.group;
        const rows = [
            E.tr(E.th(null, `Hit Group #${info.idx}`, {colspan:2})),
            ...this._makeBlockPolyGroupInfo(block, group),
        ];
        this._tbl.append(...rows);
    }

    _makeBlockPolyGroupInfo(block, group) {
        const sx    = Math.abs(group.x1 - group.x2);
        const sy    = Math.abs(group.y1 - group.y2);
        const sz    = Math.abs(group.z1 - group.z2);
        return [
            E.tr( E.th(null, "Block"), E.td('string', block.header.name) ),
            E.tr(
                E.th(null, "ID"),
                E.td('hex', `0x${hex(group.id, 2)}`),
            ),
            E.tr(
                E.th(null, "1stPoly"),
                E.td('int', group.firstPolygon),
            ),
            E.tr( E.th(null, "X"), E.td('float', `${group.x1} - ${group.x2} (${sx})`) ),
            E.tr( E.th(null, "Y"), E.td('float', `${group.y1} - ${group.y2} (${sy})`) ),
            E.tr( E.th(null, "Z"), E.td('float', `${group.z1} - ${group.z2} (${sz})`) ),
            E.tr(
                E.th(null, "Flags"),
                E.td('hex', `0x${hex(group.flags, 4)}`),
            ),
            E.tr(
                E.th(null, "Unk0E"),
                E.td('hex', `0x${hex(group._0E, 2)}${hex(group._0F, 2)}`),
            ),
            E.tr(
                E.th(null, "Unk11"),
                E.td('hex', `0x${hex(group._11, 2)}`),
            ),
        ];
    }

    _showObject(info) {
        /** Display info about a game object instance.
         *  @param {object} info The object info.
         *  @note `info.obj` should be a RomListEntry.
         */
        const entry = info.obj;
        const dll   = entry.object.dll;
        const rows = [
            E.tr(E.th(null, `Object ID 0x${hex(entry.id, 8)}`, {colspan:2})),
            E.tr(
                E.th(null, "Name"),
                E.td('string objName', entry.object.header.name),
            ),
            E.tr(
                E.th(null, "Entry#"),
                E.td('int', entry.idx),
            ),
            E.tr(
                E.th(null, "Def#"),
                E.td('hex objDefNo', `0x${hex(entry.objDef, 4)}`),
            ),
            E.tr(
                E.th(null, "Position"),
                E.td('vec3f objPos', `${entry.position.x.toFixed(2)}, ${entry.position.y.toFixed(2)}, ${entry.position.z.toFixed(2)}`),
            ),
            E.tr(
                E.th(null, "DLL"),
                E.td('dllId', `#0x${hex(entry.object.header.dll_id, 4)} `+
                    (dll ? dll.name : '-')),
            ),
        ];
        let params = entry.params;
        if(params == null) params = {};
        for(const [name, param] of Object.entries(params)) {
            rows.push(E.tr(
                E.th('objparam', name),
                E.td(null, param.value.display),
            ))
        }
        this._tbl.append(...rows);
    }

    _showWarp(info) {
        /** Display info about a WARPTAB entry.
         *  @param {object} info The warp info.
         */
        const warp = info.warp;
        const rows = [
            E.tr(E.th(null, `Warp #0x${hex(info.idx,2)}`, {colspan:2})),
            E.tr( E.th(null, "X"), E.td('float', warp.pos.x) ),
            E.tr( E.th(null, "Y"), E.td('float', warp.pos.y) ),
            E.tr( E.th(null, "Z"), E.td('float', warp.pos.z) ),
            E.tr(
                E.th(null, "Angle"),
                E.td('int',
                    `${Math.round((warp.xRot / 65536)*360)}\u00B0 (0x${hex(warp.xRot,4)})`),
            ),
        ];
        this._tbl.append(...rows);
    }
}
