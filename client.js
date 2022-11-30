
const hostedSiteUrl = "http://localhost:12913";

function encode(str){
    str = str.replaceAll(" ","(-)%20)");
    str = str.replaceAll("<","(-)%3C)");
    str = str.replaceAll(">","(-)%3E)");
   str =  str.replaceAll("\t","(-)%T4)");
    str = str.replaceAll("%","(-)%T5)");
    str = str.replaceAll("$","(-)%T6)");
    str = str.replaceAll(":","(-)%T7)");
    str = str.replaceAll(";","(-)%T8)");
    str = str.replaceAll("=","(-)%T9)");
   str =  str.replaceAll("_","(-)%7A)");
   return str;
}
function decode(str){
    str = str.replaceAll("(-)%20)"," ");
    str = str.replaceAll("(-)%3C)","<");
    str = str.replaceAll("(-)%3E)",">");
    str = str.replaceAll("(-)%T4)","\t");
    str = str.replaceAll("(-)%T5)","%");
    str = str.replaceAll("(-)%T6)","$");
    str = str.replaceAll("(-)%T7)",":");
    str = str.replaceAll("(-)%T8)",";");
    str = str.replaceAll("(-)%T9)","=");
    str = str.replaceAll("(-)%7A)","_");
    return str;
}


async function write(node,content){
    node = node.replaceAll("/",".");
    content = encode(content);
    const res = await httpGet(hostedSiteUrl+"/?write="+node+"&"+content);
    return res;
}
async function read(node){
    node = node.replaceAll("/",".");
    const res = JSON.parse(await httpGet(hostedSiteUrl+"/?get="+node));
        res.data = decode(res.data);
    return res;
}

    

async function httpGet(theUrl)
{
    var xmlHttp = new XMLHttpRequest();
    await xmlHttp.open( "GET", theUrl, false ); n
    await xmlHttp.send( null );
    return xmlHttp.responseText;
}