import App from './app/App.js';

async function main() {
    let parent = null;
    if(window.opener && window.opener.app) {
        parent = window.opener.app;
    }
    window.app = new App(parent);
    await window.app.run();
}

if(document.readyState === "complete"
|| document.readyState === "interactive") {
    setTimeout(main, 1); //call on next available tick
} else {
    document.addEventListener("DOMContentLoaded", main);
}
