class Login extends View {
  async start(data) {
    new Component("div")
      .addClass("loginDiv")
      .add(new Component("br"))
      .add(new Component("br"))

      .add(
        new Component("input")
          .set("type", "text")
          .set("placeholder", "Username")
          .set("id", "usna_field")
      )
      .add(
        new Component("input")
          .set("type", "password")
          .set("placeholder", "Password")
          .set("id", "pswd_field")
      )
      .add(new Component("br"))

      .add(
        new Component("button")
          .setText("Register")
          .set("onclick", "Login.register()")
      )
      .add(
        new Component("button").setText("Login").set("onclick", "Login.login()")
      );
  }
  static async register() {
    const ret = await register(usna_field.value, pswd_field.value);
    if (ret == -1) return;
    localStorage.setItem(
      "login",
      JSON.stringify({ username: usna_field.value, password: pswd_field.value })
    );
    Manager.openView(new App());
  }
  static async login() {
    const ret = await signup(usna_field.value, pswd_field.value);
    if (ret == -1) return;
    localStorage.setItem(
      "login",
      JSON.stringify({ username: usna_field.value, password: pswd_field.value })
    );
    Manager.openView(new App());
  }
  async finish() {}
}
