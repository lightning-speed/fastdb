/*const https = require("https");

// Sample URL
for (let i = 0; i < 100; i++) {
  const url = "http://127.0.0.1:9001?&get&msegs." + i;
  const request = https.request(url, (response) => {
    let data = "";
    response.on("data", (chunk) => {
      data = data + chunk.toString();
    });

    response.on("end", () => {
      console.log(data);
    });
  });

  request.on("error", (error) => {
    console.log("An error", error);
  });

  request.end();
}
*/

const https = require("https");
async function a() {
  https
    .get("https://pocketchat.fireship.app", function (res) {
      console.log(res.statusCode);
      res.setEncoding("utf8");
      res.on("data", function (data) {
        console.log(data.charAt(0));
      });
    })
    .on("error", function (err) {
      console.log(err);
    });
}
setInterval(a, 50);
