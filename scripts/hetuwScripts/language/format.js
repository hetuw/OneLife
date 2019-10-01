
const fs = require('fs');

var out_file = "formatted.txt";

// args[0] path of nodejs
// args[1] path of script
var args = process.argv.slice(0);
var in_folder = args[2];

let isWin = process.platform === "win32";
let fileSeperator = '/';
if (isWin) fileSeperator = '\\';

function formatFile(file) {
	console.log("file: "+file);

	let contents = fs.readFileSync(file, 'utf8');	
	let lines = contents.split('\n');
	let out_str = "";

	for (var i = 0; i < lines.length; i++) {
		let lineIsEmpty = true;
		let str = "";
		let lastCharWasSpace = false;
		for (var k = 0; k < lines[i].length; k++) {
			if (lines[i][k] === '/') break;
			if (lines[i][k] === ' ') {
				if (!lastCharWasSpace) {
					lastCharWasSpace = true;
					str += " ";
				}
				continue;
			}
			lastCharWasSpace = false;
			lineIsEmpty = false;
			if (lines[i][k] === '{') {
				str += '[';
			} else if (lines[i][k] === '}') {
				str += ']';
			} else {
				str += lines[i][k];
			}
		}
		if (lineIsEmpty) out_str += "\n";
		else out_str += str;
	}
	out_str += "\n";

	fs.appendFileSync(out_file, out_str);
}

function main() {

	if (!fs.existsSync(in_folder)) {
		console.log("Error could not find folder: "+folder);
		console.log("Start this script with a folder as argument");
		return;
	}

	fs.readdirSync(in_folder).forEach( (file) => {
		formatFile(in_folder+fileSeperator+file);
	});
}

main();
