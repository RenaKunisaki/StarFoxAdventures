import { E } from "../../lib/Element.js";

export class TaskCancelled extends Error {

}

export default class TaskProgress {
    /** A popup that shows progress of a task. */
    constructor() {
        this.numSteps   = 0;
        this.stepsDone  = 0;
        this.taskText   = "";
        this.subText    = "";
        this._cancelled = false;

        this.eBar       = E.progress();
        this.eText      = E.h1('taskText');
        this.eSubText   = E.h2('subText');
        this.eStepsDone = E.span('number stepsDone');
        this.eNumSteps  = E.span('number numSteps');
        this.btnCancel  = E.button('cancel', "Cancel");
        this.element    = E.div('taskProgress',
            this.eText, this.eSubText,
            E.div(null, this.eStepsDone, " of ", this.eNumSteps),
            this.eBar, this.btnCancel,
        );
        this.element.style.display = 'none';
        document.body.append(this.element);

        this.btnCancel.addEventListener('click', e => {this._cancelled = true});
    }

    show() {
        this.element.style.display = '';
    }

    hide() {
        this.element.style.display = 'none';
    }

    update(params) {
        if(this._cancelled) {
            this._cancelled = false;
            this.hide();
            throw new TaskCancelled();
        }
        if(params.taskText  != undefined) this.taskText  = params.taskText;
        if(params.subText   != undefined) this.subText   = params.subText;
        if(params.stepsDone != undefined) this.stepsDone = params.stepsDone;
        if(params.numSteps  != undefined) this.numSteps  = params.numSteps;
        this.eText     .innerText = this.taskText;
        this.eSubText  .innerText = this.subText;
        this.eStepsDone.innerText = this.stepsDone;
        this.eNumSteps .innerText = this.numSteps;
        this.eBar.setAttribute('max',   this.numSteps);
        this.eBar.setAttribute('value', this.stepsDone);
        this.show();
        //wait for next frame so we can let other tasks process
        return new Promise(window.requestAnimationFrame);
    }
}
