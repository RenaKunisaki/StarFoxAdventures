export function createElement(name, ...args) {
    let elem;
    if(name instanceof Array) { //holy shit this is stupid
        elem = document.createElementNS(name[0], name[1]);
    }
    else elem = document.createElement(name);
    let haveClass = false;
    while(args.length > 0) {
        const arg = args.shift();
        if(typeof(arg) == 'string') {
            if(!haveClass) {
                elem.className = arg;
                haveClass = true;
            }
            else elem.append(arg);
        }
        else if(typeof(arg) == 'number') {
            elem.append(arg.toString());
        }
        else if((arg === null || arg === undefined) && !haveClass) haveClass = true;
        else if(arg instanceof Element) elem.append(arg);
        else if(Array.isArray(arg)) {
            for(const item of arg) elem.append(item);
        }
        else if(typeof(arg) == 'object') {
            if(arg === null) elem.append("null");
            else if(arg === undefined) elem.append("undefined");
            else if(arg.element instanceof Element) elem.append(arg.element);
            else {
                for(const k in arg) {
                    //elem[k] = options[k];
                    const v = arg[k];
                    if(k in elem) elem[k] = v;
                    else elem.setAttribute(k, v);
                    if(k == 'className') haveClass = true;
                }
            }
        }
    }
    return elem;
}

//Convenience function for creating elements.
//eg: elem = E.div('someClass');
const handler = {
    get: function(obj, prop) {
        if(prop in obj) return obj[prop];
        return (...args) => createElement(prop, ...args);
    }
};
export const E = new Proxy({}, handler);

export function clearElement(elem) {
    while(elem.firstChild) {
        elem.removeChild(elem.firstChild);
    }
    return elem;
}

export function isVisible(e) {
    //copied from jQuery
    return !!( e.offsetWidth || e.offsetHeight || e.getClientRects().length );
}

export function connectEvents(elem, events) {
    for(const event in events) {
        elem.addEventListener(event, events[event]);
    }
}
export function disconnectEvents(elem, events) {
    for(const event in events) {
        elem.removeEventListener(event, events[event]);
    }
}
