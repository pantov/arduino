/**************************websocket_example.js*************************************************/

var bodyParser = require("body-parser");
const express = require('express'); //express framework to have a higher level of methods
const app = express(); //assign app variable the express class/method
var http = require('http');
var path = require("path");
// подключённые клиенты
var clients = {};
//var id=1;
var serverid="server";
//console.log(http);
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
const server = http.createServer(app);//create a server


//***************this snippet gets the local ip of the node.js server. copy this ip to the client side code and add ':3000' *****
//****************exmpl. 192.168.56.1---> var sock =new WebSocket("ws://192.168.56.1:3000");*************************************
require('dns').lookup(require('os').hostname(), function (err, add, fam) {
  console.log('addr: '+add);
})

/**********************websocket setup**************************************************************************************/
//var expressWs = require('express-ws')(app,server);
const WebSocket = require('ws');
const s = new WebSocket.Server({ server });

//webSockets = {} // userID: webSocket

//when browser sends get request, send html file to browser
// viewed at http://localhost:30000
app.get('/', function(req, res) {
res.sendFile(path.join(__dirname + '/index.html'));
});
app.get('/server', function(req, res) {
res.sendFile(path.join(__dirname + '/index.html'));
});
app.get('/cli', function(req, res) {
res.sendFile(path.join(__dirname + '/index.html'));
});


//*************************************************************************************************************************
//***************************ws chat server********************************************************************************

//app.ws('/echo', function(ws, req) {
s.on('connection',function(ws,req){
//
//  var userID = parseInt(ws.upgradeReq.url.substr(1), 10)
//  webSockets[userID] = ws
//  console.log('connected: ' + userID + ' in ' + Object.getOwnPropertyNames(webSockets))
//const location = url.parse(ws.upgradeReq.url, true);
var id = Math.random(); 
//if (req.url=="/server") {
//  serverid=id;
//  } 
// clients[id] = ws;
// id=id+1;
ws.url=req.url;
if (req.url.indexOf(serverid) !== -1) {
ws.tip="server"
}  else {
ws.tip="client"
}
// console.log("req.url ====");
// console.log(ws.url);
// console.log("req.url ====");
// console.log("location " + location);
console.log("------------------------");
s.clients.forEach(function(client){ //broadcast incoming message to all clients (s.clients)
//if(client!=ws && client.readyState ){ //except to the same client (ws) that sent this message
//if(client.readyState ){ //except to the same client (ws) that sent this message
//client.send("broadcast: " +message);
console.log(client.url);
});
console.log("=========================");
//ws.hereMyCustomParameter = parameters.query.myCustomParam;
//var clientID = ws._socket._handle.fd;
//console.log('Client.ID: '+clientID);
//           console.log('Client.ID: ' + ws._socket._handle.fd);
//
/******* when server receives messsage from client trigger function with argument message *****/
ws.on('message',function(message){

console.log("Received from: "+ws.tip+"   "+message);

s.clients.forEach(function(client){ //broadcast incoming message to all clients (s.clients)
if(client!=ws && client.readyState ){ //except to the same client (ws) that sent this message
//if(client.readyState ){ //except to the same client (ws) that sent this message
//client.send("broadcast: " +message);

if (ws.tip=="server") {
client.send("broadcast: " +message);
} else {

if (client.tip=="server") {
client.send("broadcast: " +message);
}

}



}
});

// ws.send("From Server only to sender: "+ message); //send to client where message is from
for (var key in clients) {
      if (key!=serverid) {
  //    clients[key].send("Send "+key+" "+message);
      }
    }
});
ws.on('close', function(){
//console.log("lost one client");
//console.log('connect close ' + id);
//delete clients[id];
});
//ws.send("new client connected");
//console.log("new client connected");
//console.log("");
});
server.listen(3000);