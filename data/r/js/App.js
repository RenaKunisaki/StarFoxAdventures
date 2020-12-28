import Game from './Game.js';
import Index from './Index.js';
import DllInfo from './DllInfo.js';
import DllList from './DllList.js';
import ObjInfo from './ObjInfo.js';
import ObjList from './ObjList.js';
import GameTextList from './GameTextList.js';
import GameBitList from './GameBitList.js';
import MapInfo from './MapInfo.js';
import MapList from './MapList.js';
import MapView from './MapView.js';

export default class App {
    /** The application.
     *  This class is responsible for deciding which class(es) to invoke
     *  to render the page.
     */
    constructor() {
        const params = new URLSearchParams(window.location.search);
        this.params = params;
        let ver = params.get('v');
        if(ver == null) ver='U0';
        this.game = new Game(ver);
    }

    async run() {
        await this.game.downloadInfo();

        const pages = {
            dll:      DllInfo,
            dlls:     DllList,
            obj:      ObjInfo,
            objs:     ObjList,
            gametext: GameTextList,
            gamebits: GameBitList,
            map:      MapInfo,
            maps:     MapList,
            mapview:  MapView,
        };
        const p = this.params.get('p');
        const cls = (p == null) ? Index : pages[p];

        if(cls) {
            this.page = new cls(this);
            document.body.append(this.page.element)
            await this.page.run();
            document.title = `${this.page.getTitle()} - SFA`
        }
        else document.body.append("Page not found");
        document.getElementById('loading').remove();
    }
}
