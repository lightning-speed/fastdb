


class func_table {
	constructor(){
		this.list = new Array();
	}
	add(name,func){
		this.list.push({
			name:name,
			func: func
		});
	}
	call(func_name,arugments){
		for(let i = 0;i<this.list.length;i++){
			if(this.list[i].name == func_name){
				this.list[i].func();
				return 0;
			}
		}
		console.log("Invalid function name");
		return -1;
	}
}

const registered_functions = new func_table();

function main(){
 if(process.argv.length<=2){
 	console.log("Function name not provided");
 	return -1;
 }
 const arugments = new Array();
 for(let i =3;i<process.argv.length;i++){
 	arugments.push(process.argv[i]); 
 }
 registered_functions.call(process.argv[2],arugments);
}
module.exports = {start: main, functions: registered_functions};