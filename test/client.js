const hostedSiteUrl = window.location.origin;
let seqKey;
async function signup(username, password) {
  const resT = /^[a-z0-9_\.]+$/.exec(username);
  const pt = /^[a-z0-9_\.]+$/.exec(password);

  const valid = !!resT;
  if (!valid) {
    alert("Inappropriate Username");
    return -1;
  }
  if (!!!pt || password.length < 6) {
    alert("Inappropriate Password");
    return -1;
  }
  let res = await httpGet(
    hostedSiteUrl + "/?&aus&" + username + "&" + password
  );
  res = JSON.parse(res);
  if (res.response != 200) {
    alert("Invalid Login Details");
    return -1;
  }
  seqKey = res.data;
  return res;
}
async function register(username, password) {
  const resT = /^[a-z0-9_\.]+$/.exec(username);
  const pt = /^[a-z0-9_\.]+$/.exec(password);

  const valid = !!resT;
  if (!valid) {
    alert("Inappropriate Username");
    return -1;
  }
  if (!!!pt || password.length < 6) {
    alert("Inappropriate Password");
    return -1;
  }
  let res = await httpGet(
    hostedSiteUrl + "/?&reg&" + username + "&" + password
  );
  res = JSON.parse(res);
  seqKey = res.data;
  return res;
}
function encode(str) {
  str = str.replaceAll(" ", "(-)20)");
  str = str.replaceAll('"', "(-)22)");
  str = str.replaceAll("<", "(-)3C)");
  str = str.replaceAll(">", "(-)3E)");
  str = str.replaceAll("\t", "(-)T4)");
  str = str.replaceAll("$", "(-)T6)");
  str = str.replaceAll(":", "(-)T7)");
  str = str.replaceAll(";", "(-)T8)");
  str = str.replaceAll("=", "(-)T9)");
  str = str.replaceAll("_", "(-)7A)");
  return str;
}
function decode(str) {
  str = str.replaceAll("(-)20)", " ");
  str = str.replaceAll("(-)3C)", "<");
  str = str.replaceAll("(-)22)", '"');
  str = str.replaceAll("(-)3E)", ">");
  str = str.replaceAll("(-)T4)", "\t");
  str = str.replaceAll("(-)T6)", "$");
  str = str.replaceAll("(-)T7)", ":");
  str = str.replaceAll("(-)T8)", ";");
  str = str.replaceAll("(-)T9)", "=");
  str = str.replaceAll("(-)7A)", "_");
  return str;
}

async function write(node, content) {
  node = node.replaceAll("/", ".");
  content = encode(content);
  const res = await httpGet(
    hostedSiteUrl + "/?&write&" + node + "&" + seqKey + "&" + content
  );
  return res;
}
async function read(node) {
  node = node.replaceAll("/", ".");
  const res = JSON.parse(
    await httpGet(hostedSiteUrl + "/?&get&" + node + "&" + seqKey)
  );
  if (res.data != null) res.data = decode(res.data);
  return res;
}

async function info(node) {
  node = node.replaceAll("/", ".");
  const res = await httpGet(hostedSiteUrl + "/?&info&" + node + "&" + seqKey);
  return JSON.parse(res);
}
async function remove(node) {
  node = node.replaceAll("/", ".");
  const res = await httpGet(hostedSiteUrl + "/?&delete&" + node + "&" + seqKey);
  return JSON.parse(res);
}

async function httpGet(theUrl) {
  var xmlHttp = new XMLHttpRequest();
  await xmlHttp.open("GET", theUrl, false);
  await xmlHttp.send(null);
  return xmlHttp.responseText;
}
