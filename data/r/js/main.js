/** Main script to run the SFA data browser.
 */
import App from './App.js';

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
