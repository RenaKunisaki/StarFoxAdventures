import { E } from "../../lib/Element.js";

export default class TabBar {
    /** A container for multiple elements that can be switched between
     *  using a tab bar at the top.
     */
    constructor(items) {
        this.tabs = {}; //label => {'eTab':tab elem, 'eBody':body elem}
        this._eTabBar = E.div('tabs');
        this.element = E.div('tabBar', this._eTabBar);
        if(items != undefined) {
            for(const [label, elem] of Object.entries(items)) {
                this.add(label, elem);
            }
        }
    }

    add(label, elem) {
        //add the element
        const isFirst = Object.keys(this.tabs).length == 0;
        //elem = E.div('tabContainer', elem);
        this.element.append(elem);
        if(!isFirst) elem.style.display = 'none';

        //build a tab for the element
        let tab = E.div('tab', label);
        this._eTabBar.append(tab);
        tab.addEventListener('click', e => this.showTab(label));
        if(isFirst) tab.classList.add('active');

        this.tabs[label] = {
            eTab:  tab,
            eBody: elem,
        };
    }

    showTab(label) {
        for(let [lbl, tab] of Object.entries(this.tabs)) {
            if(lbl == label) {
                tab.eTab.classList.add('active');
                tab.eBody.style.display = 'block';
            }
            else {
                tab.eTab.classList.remove('active');
                tab.eBody.style.display = 'none';
            }
        }
    }
}
