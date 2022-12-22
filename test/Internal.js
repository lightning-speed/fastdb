class Internal {
  static async Init() {
    this.lastMsgI = (await info("msg")).size;
    if (this.lastMsgI == null) {
      this.lastMsgI = 0;
    } else this.lastMsgI = Math.max(0, this.lastMsgI - 10);
    setInterval(async function () {
      let msg = await read("msg/" + Internal.lastMsgI);
      while (msg != null && msg.response == 200) {
        const md = JSON.parse(msg.data);
        Manager.currentView.createMessage(md.msg, md.username, md.time);
        Internal.lastMsgI++;
        msg = await read("msg/" + Internal.lastMsgI);
        Manager.currentView.MsgDiv.element.scrollTop =
          Manager.currentView.MsgDiv.element.scrollHeight;
      }
    }, 1000);
  }
  static async send() {
    let lp = (await info("msg")).size;
    if (lp == null) {
      lp = 0;
    }
    const res = await write(
      "msg/" + lp,
      JSON.stringify({
        username: this.username,
        msg: mbox.value,
        time: new Date().toString,
      })
    );
    console.log(res);
    mbox.value = "";
  }
}
