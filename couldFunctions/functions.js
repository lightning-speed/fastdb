
const cloud = require("./cloud.js");
const io = require("./io.js");
class functions{

 static async test(){
  	let buf = "a";
  	buf = await io.read("com","shit");
  	console.log(buf);
  }
}

cloud.functions.add("test",functions.test);

cloud.start();

