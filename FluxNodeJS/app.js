var WebSocketServer = require('ws').Server,
  http = require('http'),
  request = require('request'),
  fs = require('fs');


var imageFolder = "../FluxOF/bin/data/images/";
var incommingItems = [];


var wss = new WebSocketServer({port: 8080});

var ofConnection;

wss.on('connection', function(ws) {
  ofConnection = ws;

  ws.on('message', function(message) {
    console.log('received: %s', message);

    //try {
      var object = JSON.parse(message);
      if (object.type && object.type == 'photoTrigger') {
        console.log("Trigger photo " + object.id);
      }
    //} catch(e){
      //console.log(e);
    //}
  });
  ws.on('close', function(){
    console.log("CLOSE")
    ofConnection = undefined;
  });

});



var sendIncommingImage = function(){
  if(ofConnection) {
    ofConnection.send(JSON.stringify({queue:incommingItems}));
  } else {
    console.log("No of connection")
  }
};


var pullImages = function(){
  console.log("Pulling for new images");

  var username = "adidas",
    password = "udox and adidas",
    auth = "Basic " + new Buffer(username + ":" + password).toString("base64");

  var options = {
    host: 'dev.adi063.adidas.u-dox.com',
    port: 80,
    path: '/api/image-feed/',
    method: 'GET',
    headers : {
      "Authorization" : auth
    }
  };

  http.request(options, function(res) {
    res.setEncoding('utf8');

    var responseString = '';

    res.on('data', function(data) {
      responseString += data;
    });

    res.on('end', function() {
      var responseObject = JSON.parse(responseString);
      handleIncommingImages(responseObject);
    });

  }).end();
};

var handleIncommingImages = function(data){
//  console.log(data);

  for(var i in data){
    //console.log(i)
    var itemFound = false;
    for(var u in incommingItems){
      if(incommingItems[u].id == data[i].id){
        itemFound = true;
      }
    }

    if(!itemFound){
      console.log("New item "+data[i].username);
      downloadImage(data[i]);
      incommingItems.push(data[i]);
    }
  }
  sendIncommingImage();

  setTimeout(pullImages, 2000);
};

var downloadImage = function(item){
  console.log("Going to download "+item.image_url+" to "+imageFolder+item.id+".png")
  request(item.image_url).pipe(fs.createWriteStream(imageFolder+item.id+".png"))

};

pullImages();