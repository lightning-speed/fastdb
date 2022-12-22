class Manager {
  static async loadJSFile(path, onload, err) {
    let scriptEle = document.createElement("script");
    scriptEle.setAttribute("src", path);
    document.body.appendChild(scriptEle);
    if (onload != null) {
      scriptEle.addEventListener("load", () => {
        onload();
      });
    }
    if (err != null) {
      scriptEle.addEventListener("error", (ev) => {
        err(ev);
      });
    }
    return scriptEle;
  }
  static init(theme, view) {
    this.views = new Array();
    Frontend.init(theme);
    this.openView(view, null);
  }
  static rv() {
    for (let i = 0; i < this.views.length; i++)
      if (this.views[i].view == view) {
        this.views.splice(i, 1);
        break;
      }
  }

  static async openView(view, data) {
    mainView.innerHTML = "";
    this.currentView = view;
    view.start(data);
  }

  static isScreenOrientationLandscape() {
    return window.screen.orientation.type.includes("landscape");
  }
}
