class App extends View {
  async start(data) {
    let res = localStorage.getItem("login");
    if (res == null) {
      Manager.openView(new Login(), null);
    }
    res = JSON.parse(res);
    Internal.username = res.username;
    res = await signup(res.username, res.password);
    console.log(res);

    const MainDiv = new Component("div")
      .add(
        new Component("input")
          .addClass("m_box")
          .set("id", "mbox")
          .set("placeholder", "Type Message Here")
      )
      .add(
        new Component("button")
          .addClass("s_but")
          .set("onclick", "Internal.send()")
          .setText("☝")
      )
      .set("style", "height:100%");
    this.MsgDiv = new Component("div", MainDiv.element);
    this.MsgDiv.addClass("art");
    await Internal.Init();
  }

  async finish() {}
  async createMessage(msg, user, time) {
    if (user == Internal.username) {
      user = "You";
    }

    this.MsgDiv.add(new Component("br")).add(
      new Component("div")
        .addClass("msg")
        .add(new Component("span").setText("From: " + user))
        .add(new Component("hr"))
        .add(new Component("span").setText(msg))
        .set(
          "style",
          `background-color:rgb(${Math.random() * 120},${Math.random() * 100},${
            Math.random() * 140
          });`
        )
    );
  }
}
