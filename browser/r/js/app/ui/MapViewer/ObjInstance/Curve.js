import { ObjInstance } from "./ObjInstance.js";
import { hsv2rgb } from "../../../../Util.js";
import RenderBatch from "../../gl/gx/RenderBatch.js";
import Box from "../../gl/Model/Box.js";

export default class Curve extends ObjInstance {
    _makeColor(entry) {
        const [r,g,b] = hsv2rgb(
            //XXX confirm max type
            (entry.params.type.value.value / 0x3F) * 360, 1, 1);
        return [r*255, g*255, b*255, 0xC0];
    }
    _renderPoint(id, obj, batch) {
        batch.addFunction(Box.fromLine(this.gx,
            [this.entry.position.x, this.entry.position.y, this.entry.position.z],
            [obj.position.x, obj.position.y, obj.position.z],
            [0.25, 0.25, 0.25])
            .setId(id).setColors([this._makeColor(obj)]).batch);
    }

    render(id) {
        const batch  = new RenderBatch(this.gx);
        const entry  = this.entry;

        //render lines between the points
        const idNext = entry.params.idNext.value.value; //ugh
        const idPrev = entry.params.idPrev.value.value;
        const id24   = entry.params._24.value.value;
        const id28   = entry.params._28.value.value;
        if(idNext > 0) {
            const next = this.map.romList.objsByUniqueId[idNext];
            if(!next) {
                console.warn("Curve's next point doesn't exist", entry);
            }
            else this._renderPoint(id, next, batch);
        }

        const others = [idPrev, id24, id28];
        for(let idOther of others) {
            if(idOther > 0) {
                const that = this.map.romList.objsByUniqueId[idOther];
                if(that && that.params.idNext.value.value != entry.id) {
                    this._renderPoint(id, that, batch);
                }
            }
        }

        //render the point
        const x = this.entry.position.x;
        const y = this.entry.position.y;
        const z = this.entry.position.z;
        const s = 5;
        batch.addFunction((new Box(this.gx,
            [-0.5, -0.5, -0.5],
            [ 0.5,  0.5,  0.5],
        )).setPos(x,y,z).setScale(5).setId(id).setColors(
            [this._makeColor(entry)]).batch);

        return batch;
    }
};
