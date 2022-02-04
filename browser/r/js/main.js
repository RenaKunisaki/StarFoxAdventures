import App from './app/App.js';

async function main() {
    window.app = new App();
    await window.app.run();
}

if(document.readyState === "complete"
|| document.readyState === "interactive") {
    setTimeout(main, 1); //call on next available tick
} else {
    document.addEventListener("DOMContentLoaded", main);
}
