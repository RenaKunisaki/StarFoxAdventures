/** This script plays back a gameplay video along with the recorded debug log,
 *  and displays state information extracted from the log.
 */
import {E} from './Element.js';
import {get, getDlls, getBits, finishLoading, setupMain, hex} from './Util.js';
import Struct from '/js/Struct.js';

const NUM_DLLS     = 0x2C2;
const NUM_GAMEBITS = 0xF58;
const IGNORE_LINE_PREFIXES = [
    "Bit 0A7F", //toggled constantly
    "Bit 0CBB", //toggled constantly
    '>> ', //handled specially
    't=',
    "hitstate ",
    "FAIL ON NOT ON GROUND",
    'LOADED OBJECT ', //for now
];

let DLLs, gameBits;
let logFollowTimer=null;
let logLines, logView, dllView, gameBitView, player, eDebug, eDprint, eInfo;
let dllLogView, bitLogView;
//let timeOffset = (2*60)+53; //seconds (ep1) - XXX make this a URL param
let timeOffset = 128; //seconds

function fmtTime(time) {
    const secs = Math.floor( time         % 60).toString().padStart(2, '0');
    const mins = Math.floor((time /   60) % 60).toString().padStart(2, '0');
    const hrs  = Math.floor( time / 3600)      .toString().padStart(2, '0');
    return `${hrs}:${mins}:${secs}`;
}

let prevLine      = null;
let dprintLines   = {};
let dllRefCounts  = {};
let gameBitValues = {};
let dllElems      = [];
let gameBitElems  = [];

function getBit(id) {
    let bit = gameBits.getElementById('0x'+hex(id, 4));
    if(!bit) {
        console.error("Not found bit", id);
        return null;
    }
    return {
        id:          id,
        name:        bit.getAttribute('name'),
        table:       bit.getAttribute('table'),
        unk2:        bit.getAttribute('unk2'),
        unk1:        bit.getAttribute('unk1'),
        max:         bit.getAttribute('max'),
        description: bit.textContent,
    };
}

function getDll(id) {
    let dll = DLLs.getElementById('0x'+hex(id, 4));
    if(!dll) {
        console.error("Not found DLL", id);
        return null;
    }
    return {
        id:        id,
        name:      dll.getAttribute('name'),
        address:   dll.getAttribute('address'),
        dolOffs:   dll.getAttribute('dolOffs'),
        interface: dll.getAttribute('interface'),
        unk08:     dll.getAttribute('unk08'),
        unk0E:     dll.getAttribute('unk0E'),
    };
}

function parseLine(line) {
    if(line.msg.indexOf('load   DLL') >= 0) {
        //xxxxxxxx load   DLL 004A param 00000004 rc=0
        //the extra spaces aren't visible in the browser UI
        let p = line.msg.indexOf('rc=');
        let dll = parseInt(line.msg.substr(20, 4), 16);
        //message is before refcount is updated
        return {
            event: 'loadDll',
            id:    dll,
            rc:    parseInt(line.msg.substr(p+3)) + 1, //ref count
            param: line.msg.substr(31, 8),
            dll:   getDll(dll),
        };
    }
    else if(line.msg.indexOf('unload DLL') >= 0) {
        //xxxxxxxx unload DLL 01F5 addr 80327EF0 rc=1
        let p = line.msg.indexOf('rc=');
        let dll = parseInt(line.msg.substr(20, 4), 16);
        return {
            event: 'unloadDll',
            id:    dll,
            rc:    parseInt(line.msg.substr(p+3)) - 1,
            dll:   getDll(dll),
        };
    }
    else if(line.msg.indexOf('Bit ') >= 0 && line.msg.indexOf(' -> ') >= 0) {
        //8139501c Bit 076E 0 -> 1
        let fields = line.msg.trim().split(' ');
        let id = parseInt(fields[1], 16);
        let bit = getBit(id);
        if(bit == null) console.error("Not found bit for msg", line);
        return {
            event:  'setBit',
            id:     id,
            oldVal: parseInt(fields[2], 16),
            val:    parseInt(fields[4], 16),
            bit:    bit,
        };
    }
    else return null;
}

function handleMsg(line) {
    if(line.elem) line.elem.classList.remove('future');
    const event = parseLine(line);
    if(event == null) return;
    switch(event.event) {
        case 'loadDll':
        case 'unloadDll': {
            dllRefCounts[event.id] = event.rc;
            const elem = dllElems[event.id];
            if(elem) elem.classList.add('loaded');
            else console.error("No element for DLL", `dll${dll}`, line);
            break;
        }

        case 'setBit': {
            gameBitValues[event.id] = event.val;
            //console.log("GameBit", event.bit, "set to", event.val, event);
            const elem = gameBitElems[event.id];
            if(elem) {
                if(event.val == 0) elem.classList.remove('nonzero');
                else elem.classList.add('nonzero');
                elem.classList.add('defined');
            }
            else console.error("No element for GameBit", event);
            break;
        }
    }
}


function updateState() {
    /** Update ALL displayed state.
     *  This is slow, so it's only done after jumping around.
     */
    dllRefCounts = {};
    const t = player.currentTime;
    const secs = Math.floor(t);

    //reset all lines' highlight
    for(let [k, lines] of Object.entries(logLines)) {
        for(let line of lines) {
            if(line.elem) line.elem.classList.add('future');
        }
    }

    for(let i=0; i<=secs; i++) {
        if(!logLines[i]) continue;
        for(let line of logLines[i]) {
            //if(line.time >= (t + 0.17)) break;
            handleMsg(line);
        }
    }
}

function pauseLogFollowing() {
    console.log("stop log following");
    clearInterval(logFollowTimer);
    logFollowTimer = null;
    eDebug.innerText = "⏸ " + eDebug.innerText;
}

function resumeLogFollowing() {
    console.log("start log following");
    if(logFollowTimer) return;
    logFollowTimer = setInterval(() => {
        let cur = null;
        let lastDprint = null;
        const t = player.currentTime;
        const k = Math.floor(t);
        if(logLines[k]) {
            for(let line of logLines[k]) {
                if(line.time >= t) {
                    if(line.time >= (t + 0.17)) break;
                    if(line.msg.startsWith('>> ')) {
                        dprintLines[line.msg[3]] = line;
                        lastDprint = line;
                    }
                    else {
                        cur = line;
                        handleMsg(line);
                    }
                }
            }
        }
        if(cur && cur.elem) {
            cur.elem.scrollIntoView();
            eDebug.innerText = `${fmtTime(t)}; ${fmtTime(cur.time)}: ${cur.raw}`;
            prevLine = cur;
        }
        else if(prevLine) {
            eDebug.innerText = `${fmtTime(t)}; ${fmtTime(prevLine.time)}: ${prevLine.raw}`;
        }
        else eDebug.innerText = fmtTime(t);

        if(lastDprint) {
            let lines = [];
            for(let [k, line] of Object.entries(dprintLines)) {
                lines.push(line.msg);
            }
            eDprint.innerText = lines.join('\n');
        }

        //if(dllChanged) updateState();
    }, 100);
}

function jumpToTime(line) {
    console.log("jump to time", line.time, line);
    player.currentTime = line.time;
    updateState();
}


async function downloadLog(ver, name) {
    const logData = (await get({
        path:`${ver}/${name}.log`,
        mimeType:'text/plain; charset=utf-8'
    })).responseText;

    logLines = {}; //split up by second
    for(let line of logData.split('\n')) {
        //lol nope of course it doesn't work the sane way
        //the number limits how many items are returned but not how many splits
        //let [time, addr, msg] = line.trim().split(' ', 3);
        let raw = line;
        line     = line.trim();
        let sp   = line.indexOf(' ')
        let time = (parseInt(line.substring(0, sp)) / 1000) - timeOffset;
        let sp2  = line.indexOf(' ', sp+1);
        let addr = line.substring(sp+1, sp2);
        let key  = Math.floor(time);
        if(logLines[key] == undefined) logLines[key] = [];

        let entry = {
            time:  time,
            addr:  addr,
            msg:   line.substring(sp2+1),
            dTime: fmtTime(time),
            raw:   raw,
        };
        let evt = parseLine(entry);
        entry.event = evt;
        //if(key < 120) console.log("parsed", entry, evt);
        if(evt) switch(evt.event) {
            case 'loadDll': {
                if(evt.dll) entry.dispMsg = `Load   DLL ${hex(evt.id, 4)} (${evt.dll.name}) param ${evt.param}`;
                break;
            }
            case 'unloadDll': {
                if(evt.dll) entry.dispMsg = `Unload DLL ${hex(evt.id, 4)} (${evt.dll.name})`;
                break;
            }
            case 'setBit': {
                if(evt.bit) entry.dispMsg = `Bit ${hex(evt.id, 4)} (${evt.bit.name}) ${hex(evt.oldVal)} -> ${hex(evt.val)}`;
                break;
            }
        }
        logLines[key].push(entry);
    }
    //console.log("LOG", logLines);
}


function buildLogView() {
    logView    = E.ul('log-view', {id:'mainLogView'});
    dllLogView = E.ul('log-view', {id:'dllLogView'});
    bitLogView = E.ul('log-view', {id:'bitLogView'});
    for(let [k, lines] of Object.entries(logLines)) {
        let prev = null;
        for(let line of lines) {
            if(line.msg.indexOf('Previous message repeated') >= 0) {
                //00000000 Previous message repeated 2 times
                if(prev) prev.append(E.span('repeat',
                    line.msg.substring(26, line.msg.indexOf(' ', 26))));
            }
            else {
                let ignore = false;
                for(let prefix of IGNORE_LINE_PREFIXES) {
                    if(line.msg.startsWith(prefix)) {
                        ignore = true;
                        break;
                    }
                }
                if(!ignore) {
                    let msg = line.dispMsg;
                    if(msg == undefined) msg = line.msg;
                    const li = E.li('log-item future', `${line.dTime} ${line.addr} ${msg}`);
                    li.addEventListener('click', e => jumpToTime(line));
                    line.elem = li;
                    let evt = line.event ? line.event.event : null;
                    switch(evt) {
                        case 'loadDll':
                        case 'unloadDll':
                            dllLogView.append(li);
                            break;

                        case 'setBit':
                            bitLogView.append(li);
                            break;

                        default: logView.append(li);
                    }
                    prev = li;
                }
            }
        }
    }
    document.body.append(logView);
    document.body.append(dllLogView);
    document.body.append(bitLogView);
}


function buildDllView() {
    dllView = E.table('dll-view',
        E.tr(E.th(null, "DLLs", {colspan:32})));
    let id=0;
    for(let i=0; i<NUM_DLLS; i += 32) {
        const tr = E.tr();
        for(let j=0; j<32; j++) {
            const td = E.td({id: `dll${id}`});
            id++;
            tr.append(td);
            dllElems.push(td);
            td.addEventListener('mousemove', e => {
                let id = parseInt(e.target.id.substr(3));
                let dll = getDll(id);
                let name = dll ? dll.name : '?';
                eInfo.innerText = `DLL ${hex(id,4)} (${name}), rc=${dllRefCounts[id]}`;
            });
        }
        dllView.append(tr);
    }
    document.body.append(dllView);
}

function buildBitView() {
    gameBitView = E.table('gamebit-view',
        E.tr(E.th(null, "GameBits", {colspan:64})));
    let id=0;
    for(let i=0; i<NUM_GAMEBITS; i += 64) {
        const tr = E.tr();
        for(let j=0; j<64; j++) {
            const td = E.td({id: `bit${id}`});
            id++;
            tr.append(td);
            gameBitElems.push(td);
            td.addEventListener('mousemove', e => {
                let id = parseInt(e.target.id.substr(3));
                let bit = getBit(id);
                let name = bit ? bit.name : '?';
                eInfo.innerText = `Bit ${hex(id,4)} (${name}): ${gameBitValues[id]}`;
            });
        }
        gameBitView.append(tr);
    }
    document.body.append(gameBitView);
}


function buildPlayer(vid) {
    player = E.video({
            controls: 'controls',
            autoplay: 'autoplay',
            muted:    'true', //required for autoplay
        },
        E.source({
            type: 'video/mp4',
            src: `/video/${vid}`,
        }),
    );
    player.addEventListener('play',   e => resumeLogFollowing());
    player.addEventListener('pause',  e => pauseLogFollowing());
    player.addEventListener('seeked', e => updateState());
    document.body.append(player);

    eDebug = E.div('video-debug', '...');
    document.body.append(eDebug);

    //container for debug messages normally shown on-screen in game
    eDprint = E.div('video-dprint', '...');
    document.body.append(eDprint);

    //container for info about selected bit/DLL
    eInfo = E.div('video-info', '...');
    document.body.append(eInfo);

    document.addEventListener('keydown', e => {
        switch(e.code) {
            case 'Space':
                if(player.paused) player.play();
                else player.pause();
                e.preventDefault();
                break;

            //default: console.log(e.code);
        }
    });
}


async function main() {
    const params = new URLSearchParams(window.location.search);
    let ver = params.get('ver');
    if(ver == null) ver='U0';

    const vid = params.get('v');
    const log = params.get('l');
    gameBits   = await getBits(ver);
    DLLs       = await getDlls(ver);
    await downloadLog(ver, log);
    buildLogView();
    buildDllView();
    buildBitView();
    buildPlayer(vid);
    finishLoading();
}
setupMain(main);
