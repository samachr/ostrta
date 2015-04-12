var count = 0;
var ready = false;
var fs = require('fs');
var sys = require('sys');
var currentRes;
var searchReady = "this is a search";
var html = '<html><body><form method="post" action="./">Search: <input type="text" name="a" /><input type="submit" value="Submit" /></form></body>';

var html2 = '<!DOCTYPE html><html><head><link rel="stylesheet" type="text/css" href="myCSS.css"><meta charset="UTF-8"><title>Osterta</title></head><body><!--<p class="a"><span class="box">osterta</span></p>--><div class="middle"><img class="c" src="Osterta_Title.jpg" alt="Title box"/><div class="centered" ><input type="text" value=""></div><div class="smCent" ><button style="width:402px; height:26px; font-family: futura; font-size: 16px;">Search</button></div><div><footer><p class="b">One Search To Rule Them All<br>Created by Nathan Baggins and Samwise Christensen</p></footer></body>';


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

  
  
 // if (req.method == 'POST') {
  //  req.on('data'), function (data) {
 // }
  //sys.puts("the");
  //res.writeHead(200);
  //currentRes = res;
  //res.end(searchReady);
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
    currentRes.end(html + input.toString());
    currentRes=false;
    }
  }
  count++;
});
