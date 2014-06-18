var WebSocketServer = require('ws').Server,
  http = require('http'),
  request = require('request'),
  fs = require('fs'),
  gphoto2 = require('gphoto2'),
  GPhoto = new gphoto2.GPhoto2();

var dataFolder = "../FluxOF/bin/data/"
var imageFolder = dataFolder+"images/";
var incommingItems = [];

//
// Setup the WebSocket to the oF app
var wss = new WebSocketServer({port: 8080});
var ofConnection;

wss.on('connection', function(ws) {
  ofConnection = ws;

  ws.on('message', function(message) {
    console.log('received: %s', message);

    try {
      //Parse the incomming message as JSON
      //
      var object = JSON.parse(message);
      if (object.type && object.type == 'photoTrigger') {
        //TODO: Trigger the photo
        console.log("Trigger photo " + object.id);

        uploadImage(dataFolder+"images_screenshots/"+object.id+".png", object.id);

      }
    } catch(e){
      console.log(e);
    }
  });
  ws.on('close', function(){
    console.log("oF WebSocket closed")
    ofConnection = undefined;
  });

});


//
//Send the incomminImages array to oF app over the websocket
//
var sendIncommingImage = function(){
  if(ofConnection) {
    ofConnection.send(JSON.stringify({queue:incommingItems}));
  } else {
    console.log("No of connection")
  }
};


//
// Pull new images from the UDOX twitter api
//
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
      //The response is done, go parse it
      var responseObject = JSON.parse(responseString);
      handleIncommingImages(responseObject);
    });
  }).end();
};

//
// Parse the response from UDOX
//
var handleIncommingImages = function(data){
  //First delete items in the local queue that are not present at udox anymore
  for(var u in incommingItems){
    var itemFound = false;
    for(var i in data) {
      if(incommingItems[u].id == data[i].id) {
        itemFound = true;
      }
    }
    if(!itemFound){
      console.log("Remove item "+incommingItems[u].username);
      incommingItems.splice(u,1);
    }
  }

  //Look for the items in the local version of the queue, and see if its new
  for(var i=data.length-1; i>=0;i--){
    //console.log("Search for ",data[i])
    var itemFound = false;
    for(var u in incommingItems){
      if(incommingItems[u].id == data[i].id){
        itemFound = true;
      }
    }

    if(!itemFound){
      console.log("New item "+data[i].username);

      //If its new, then download it
      downloadImage(data[i]);
      incommingItems.push(data[i]);
    }
  }

  //Send the queue to oF app
  sendIncommingImage();

  //Do this again after 3 seconds
  setTimeout(pullImages, 3000);
};

//Start the pulling of images
pullImages();



//
// Download a item to the local folder
//
var downloadImage = function(item){
  console.log("Going to download "+item.image_url+" to "+imageFolder+item.id+".png id: "+item.id)
  request(item.image_url).pipe(fs.createWriteStream(imageFolder+item.id+".png"))
};

//
// Upload image to UDOX
//
var uploadImage = function(path, id){
  var r = request.post('http://dev.adi063.adidas.u-dox.com/api/returned-image/', function optionalCallback (err, httpResponse, body) {
    if (err) {
      return console.error('upload failed:', err);
    }
    console.log('Upload successful!  Server responded with:', body);
  }).auth('adidas', 'udox and adidas', true)
  var form = r.form()
  form.append('id', id)
  form.append('returned_image', fs.createReadStream(path))
}


//
// List cameras / assign list item to variable to use below options
//
GPhoto.list(function (list) {
  if (list.length === 0) return;
  var camera = list[0];
  console.log('Found', camera.model);

  // get configuration tree
  camera.getConfig(function (er, settings) {
    console.log(settings);
  });
});



