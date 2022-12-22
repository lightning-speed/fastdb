var ComponentCounterI1I = 1;

class Component {
  constructor(type, parent) {
    if (parent == null) {
      parent = mainView;
    }
    this.element = document.createElement(type);
    this.id = "Componenet73D8" + ComponentCounterI1I++;
    this.parent = parent;
    this.type = type;
    this.element.style.color =
      Frontend.theme == DARK_THEME
        ? DEFAULT_DARK_TEXT_THEME
        : DEFAULT_LIGHT_TEXT_THEME;
    if (!this.type.includes("div") && !this.type.includes("span"))
      this.element.style.backgroundColor =
        Frontend.theme == DARK_THEME
          ? DEFAULT_DARK_BUT_THEME
          : DEFAULT_LIGHT_BUT_THEME;
    parent.appendChild(this.element);
    this.children = new Array();
  }
  setText(text) {
    this.element.innerHTML = text;
    return this;
  }
  addClass(className) {
    this.element.className += className + " ";
    return this;
  }
  destroy() {
    this.element.remove();
  }
  add(Component) {
    this.children.push(Component);
    this.element.appendChild(Component.element);
    return this;
  }
  set(attribute, value) {
    this.element.setAttribute(attribute, value);
    return this;
  }
  setSize(width, height) {
    this.element.style.width = width;
    this.element.style.height = height;
    return this;
  }
  break() {
    this.add(new Component("br"));
    return this;
  }
}
