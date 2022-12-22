const mainView = document.getElementById("main_ui_forever");
const DARK_THEME = 1;
const LIGHT_THEME = 2;

const DEFAULT_DARK_BACK_THEME = "rgb(60,60,70)";
const DEFAULT_LIGHT_BACK_THEME = "rgb(220,220,240)";
const DEFAULT_DARK_TEXT_THEME = "rgb(225,225,225)";
const DEFAULT_LIGHT_TEXT_THEME = "rgb(60,60,65)";

const DEFAULT_DARK_BUT_THEME = "rgb(120,120,130)";
const DEFAULT_LIGHT_BUT_THEME = "rgb(200,200,210)";

class Frontend {
  static init(theme) {
    this.setUITheme(theme);
  }
  static setUITheme(theme) {
    this.theme = theme;
    if (theme == DARK_THEME) {
      mainView.style.backgroundColor = DEFAULT_DARK_BACK_THEME;
    } else {
      mainView.style.backgroundColor = DEFAULT_LIGHT_BACK_THEME;
    }
  }
  static async ask(question) {
    ask_dialog.children[0].innerHTML = question;
    ask_dialog_cover.style.top =
      (document.body.offsetHeight - ask_dialog_cover.offsetHeight) / 1.95 +
      "px";
    return new Promise((resolve, reject) => {
      let askButtonResult = 0;
      ask_dialog.children[2].onclick = () => {
        askButtonResult = 1;
      };
      ask_dialog.children[3].onclick = () => {
        askButtonResult = 2;
      };
      const inter = setInterval(() => {
        if (askButtonResult != 0) {
          resolve(askButtonResult == 1 ? true : false);
          askButtonResult = 0;
          clearInterval(inter);
          ask_dialog_cover.style.top = "-100%";
        }
      }, 200);
    });
  }
  static async alert(text) {
    ask_dialog.children[3].hidden = true;
    await this.ask(text);
    setTimeout(() => {
      ask_dialog.children[3].hidden = false;
    }, 200);
  }
  static setTitle(title) {
    document.title = title;
  }
}

Frontend.init();
