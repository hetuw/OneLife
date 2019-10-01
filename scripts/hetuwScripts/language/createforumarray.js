// https://onehouronelife.com/forums/viewtopic.php?id=6315

const fs = require('fs');

var filename = "vowelsAndSyllables.txt"

function main() {
	let contents = fs.readFileSync(filename, 'utf8');	
	let lines = contents.split('\n');

	var data = [];

	for (var i = 0; i < lines.length; i++) {
		for (var k = 0; k < lines[i].length; k++) {
			let c = lines[i][k];
			if (c === " ") continue;
			if (c === "\t") continue;
			if (!data[i]) data[i] = {};
			if (lines[i].charCodeAt(k) >= '0'.charCodeAt(0) && lines[i].charCodeAt(k) <= '9'.charCodeAt(0)) {
				if (!data[i].strNum) data[i].strNum = "";
				data[i].strNum += c;
				continue;
			}
			if (!data[i].text) data[i].text = "";
			data[i].text += c;
		}
	}

	let longestTextLength = 0;
	let highestNum = 0;
	for (var i = 0; i < data.length; i++) {
		console.log(i+". "+data[i].text+", "+data[i].strNum);
		if (data[i].text.length > longestTextLength) longestTextLength = data[i].text.length;
		data[i].num = parseInt(data[i].strNum);
		if (data[i].num > highestNum) highestNum = data[i].num;
	}

	console.log("");
	console.log("longestTextLength: "+longestTextLength+", highestNum: "+highestNum);
	console.log("");

	longestTextLength++;
	let array = "static constexpr char languageArray["+data.length+"]["+longestTextLength+"] = { ";

	for (var i = highestNum; i >= 0; i--) {
		for (var d = 0; d < data.length; d++) {
			if (data[d].num !== i) continue;
			array += '"'+data[d].text.toUpperCase()+'", ';
		}
	}

	array = array.substring(0, array.length-2);
	array += "};";
	
	console.log(array);
}

main();
