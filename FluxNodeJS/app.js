var WebSocketServer = require('ws').Server,
  http = require('http'),
  request = require('request'),
  fs = require('fs'),
  gphoto2 = require('gphoto2'),
  GPhoto = new gphoto2.GPhoto2(),
//  sys = require('sys'),
  exec = require('child_process').exec,
  gm = require('gm');

var dataFolder = "../FluxOF/bin/data/"
var imageFolder = dataFolder+"images/";
var incommingItems = [];

// Kill PTPCamera (otherwise we can't connect the camera)
exec("killall PTPCamera");

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
        
        takePhoto({name:object.id}, function(path){          
          uploadImage(path, object.id);          
        })

        //uploadImage(dataFolder+"images_screenshots/"+object.id+".png", object.id);

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


function generateHash() {
    return ("000000" + (Math.random()*Math.pow(36,6) << 0).toString(36)).slice(-6)
}

//
// List cameras / assign list item to variable to use below options
//
console.log("\n\n#############################################")
console.log("################ Starting up ################")
console.log("#############################################\n\n")
var camera;
GPhoto.list(function (list) {
  if (list.length === 0) {
    console.log("No cameras found")
    
    //Start the pulling of images
    pullImages();
    
    return;
  };
  camera = list[0];
  console.log('Found', camera.model);

  // get configuration tree
  /*camera.getConfig(function (er, settings) {
    console.log(settings.main.children);
  });
*/
  // Set configuration values

  //Set the image format 
  /*
  "choices":[
     "Large Fine JPEG",
     "Large Normal JPEG",
     "Medium Fine JPEG",
     "Medium Normal JPEG",
     "Small Fine JPEG  (S1 Fine)",
     "Small Normal JPEG (S1 Normal)",
     "Smaller JPEG (S2)",
     "Tiny JPEG (S3)",
     "RAW + Large Fine JPEG",
     "RAW"
  ]
  */

  console.log("Preparing camera... hold on");
  camera.setConfigValue('imageformat', "Small Fine JPEG  (S1 Fine)", function (er) {
   // camera.setConfigValue('copyright', "", function (er) {
     // camera.setConfigValue('artist', "", function (er) {
        console.log("Taking test photo");
        setTimeout(function(){
          takePhoto({name:'test'}, function(){

            console.log("### Setup done, starting to pull images from UDOX ###\n");
            //Start the pulling of images
            pullImages();
          });
        }, 1000);
   //   });
   // });
  });

   // Take picture with camera object obtained from list()
  
 
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

  var username = "USER",
    password = "PASSWORD",
    auth = "Basic " + new Buffer(username + ":" + password).toString("base64");

  var options = {
    host: 'SERVER',
    port: 80,
    path: '/api/image-feed/',
    method: 'GET',
    headers : {
      "Authorization" : auth
    }
  };

  var req = http.request(options, function(res) {
    res.setEncoding('utf8');

    var responseString = '';
    res.on('data', function(data) {
      responseString += data;
    });

    res.on('end', function() {
      //The response is done, go parse it
      try{
        var responseObject = JSON.parse(responseString);
        handleIncommingImages(responseObject);
      } catch(e){
        console.log("ERROR: ",e);
        setTimeout(pullImages, 3000);
      }
    });
  })


  req.on('error', function(e) {
    console.log('problem with request: ' + e.message);
    setTimeout(pullImages, 3000);
  });
  

  req.end();
};

//
// Parse the response from SERVER
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



  //Send the queue to oF app
  sendIncommingImage();

  if(data.length > 0){
    data = data.reverse();
    parseFirstDataElement(data);
  } else {
    setTimeout(pullImages, 3000);
  }
};



var parseFirstDataElement = function(data){
  console.log("Parse, Data length "+data.length)

  if(data.length == 0){
    setTimeout(pullImages, 3000);
    sendIncommingImage();
    return;
  }

  var itemFound = false;
  for(var u in incommingItems){
    if(incommingItems[u].id == data[0].id){
      itemFound = true;
    }
  }

  if(!itemFound){
    console.log("New item "+data[0].username);
    downloading = true;
    //If its new, then download it
    var d = data[0];
    downloadImage(d, function(){
      incommingItems.push(d);
      data.shift();
        parseFirstDataElement(data);
      
     // setTimeout(pullImages, 3000);
    });
  } else {
    data.shift();
    parseFirstDataElement(data);
  }
}

//
// Download a item to the local folder
//
var downloadImage = function(item, cb){
  console.log("Going to download "+item.image_url+" to "+imageFolder+item.id+".png id: "+item.id)
  var r = request(item.image_url).pipe(fs.createWriteStream(imageFolder+item.id+".png"))

  r.on('close', function () { 
    console.log("Downlaod complete");
    cb();
  });

};

//
// Upload image to UDOX
//
var uploadImage = function(path, id){
  console.log("Uploading "+path+" to "+id);
  var r = request.post('http://prod.adi063.adidas.u-dox.com/api/returned-image/', function optionalCallback (err, httpResponse, body) {
    if (err) {
      return console.error('upload failed:', err);
    }
    console.log('Upload successful!  Server responded with:', body);
  }).auth('USERNAME', 'USERNAME', true)
  var form = r.form()
  form.append('id', id)
  form.append('returned_image', fs.createReadStream(path))
}



var takePhoto = function(options, cb){
  console.log("Take photo")
  if(camera){
    camera.takePicture({download: true}, function (er, data) {
      if(er){
        console.log("CAMERA ERROR!");
      } else {
        var path = __dirname+"/../photos" + '/'+options.name+"_"+generateHash()+'.jpg';
        fs.writeFileSync(path, data);
        console.log("photo taken, resizing");

        gm(path)
        .resize(1024,683)
        .crop(1024, 512,0,0)
        .noProfile()
        .write(path, function (err) {
          if (!err) console.log('done');
          if(cb) cb(path);

        });
      }

    });
  }
}



