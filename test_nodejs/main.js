//
// SEEDA03 向け、クライアントサンプル
//
// command-line: node main.js
//
"use strict";

var http = require("http"),
    url  = require("url"),
    server;

server = http.createServer(function (request, response) {
  request.on("end", function() {
    var urlObj;

    switch (request.method) {
      case "POST":
        urlObj = url.parse(request.url, true);

        switch (urlObj.pathname) {
          case "/api/":
            console.log(urlObj.query.val);
            response.end();
            break;
        }
        break;
    }
  }).resume();
}).listen(3000);
