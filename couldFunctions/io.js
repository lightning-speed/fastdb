const { spawn } = require('child_process');



async function read(node){
const wc = await spawn('../fastdb', ['../db.dat','-read',node]);
let d = null;
 await  wc.stdout.on('data', (data) => {
   d = data;
  });
 return new Promise((res,err) =>{
   let inr = setInterval(()=>{
    if(d!=null){
      clearInterval(inr);
      res(`${d}`);
    }
   },50);
 });
}

module.exports = {read: read};