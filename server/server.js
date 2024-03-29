var count = 0;
var ready = false;
var fs = require('fs');
var sys = require('sys');
var currentRes;
var searchQuery = "";
var html = '<html><body><a href="http://52.10.132.16:8080">OSTRTA</a><form method="post" action="./">Search: <input style="display:inline-block;" type="text" name="a" value="'+searchQuery+'" autofocus/><input style="display:inline-block;" type="submit" value="Submit" /></form></body>';

var http = require('http');

var server = http.createServer(function(req, res) {
      if (req.method == 'POST') {
        //console.log("POST");
        var body = '';
        req.on('data', function (data) {
            body += data;
            //console.log("Partial body: " + body);
        });
        req.on('end', function () {
            //console.log("Body: " + body);
            body = body.substring(2).split('+').join(" ");
            searchQuery = body;
            sys.puts(body);
            fs.appendFileSync('queries.txt', body + "\n");
        });
        res.writeHead(200, {'Content-Type': 'text/html'});
        currentRes = res;
    }
    else
    {
        //console.log("GET");
        //var html = fs.readFileSync('index.html');
        res.writeHead(200, {'Content-Type': 'text/html'});
        res.end(html);
    }
});
server.listen(3000);

fs.appendFileSync('server-log-raw.txt', "\nServer starting at timestamp: " + Date.now() + "\n");
process.stdin.on('data', function (input) {
  if (count == 1) {
    ready = true;
  }
  fs.appendFileSync('server-log-raw.txt', count);
  fs.appendFileSync('server-log-raw.txt', ": " + input.toString());
  if(ready) {
    searchReady = input.toString();
    if(currentRes) {
    var html = '<html><body><a href="http://52.10.132.16:8080">OSTRTA</a><form method="post" action="./">Search: <input style="display:inline-block;" type="text" name="a" value="'+searchQuery+'" autofocus /><input style="display:inline-block;" type="submit" value="Submit" /></form></body>';
    var resultBuilder = html;
    var pages = input.toString().split(" ");
    if (pages[0] == "not") {
      resultBuilder += '<p>'+searchQuery+' was not found in any documents</p>';
    } else {
	    var page;
	    resultBuilder += '<p>Found results on '+(pages.length-1)+' pages</p>';
	
      for (var i = 0; i < pages.length-1;i++) {
	    page = pages[i];
	    resultBuilder += '<p><a href="http://52.10.132.16:8080/documents/cranfield00'+(page>9 ? "" + page:"0"+page)+'.html">'+page+'<br/>cranfield00'+(page>9 ? "" + page:"0"+page)+'</a></p>';
      }
    }
    
    currentRes.end(resultBuilder);
    currentRes=false;
    }
  }
  count++;
});
