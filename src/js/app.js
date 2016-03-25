
// Called when the message send attempt succeeds
function messageSuccessHandler() {
	console.log("Message send succeeded.");  
}

// Called when the message send attempt fails
function messageFailureHandler() {
	console.log("Message send failed.");
}

// Called when JS is ready
Pebble.addEventListener("ready", function(e) {
	console.log("JS is ready!");
	getData();
	setInterval(getData, 60000);
});

// Called when incoming message from the Pebble is received
// We are currently only checking the "message" appKey defined in appinfo.json/Settings
Pebble.addEventListener("appmessage", function(e) {
	console.log("Received Message: " + e.payload.message);
});

Pebble.addEventListener('showConfiguration', function() {
	var url = 'http://ukhasnet.github.io/pebble-app/configure.html';

	Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  // Decode the user's preferences
  var configData = JSON.parse(decodeURIComponent(e.response));
  localStorage.setItem('nodeName', configData.name);
});

//The pebble message contains data on the node's most recent packets
//each individual packet data has the structure. 
//{"p":"0lV4.28[BOX0,AH1,AJ1,AJ2,RUSS1,DB02,MB31]","t":"2016-03-20T14:29:36.875Z"}
function getData(){
	var method = 'GET';
	var url = 'https://ukhas.net/api/nodePackets?name='+localStorage.getItem('nodeName');
	console.log("Attempting to retreive JSON from website for " + localStorage.getItem('nodeName'));
	// Create the request
	var request = new XMLHttpRequest();

	// Specify the callback for when the request is completed
	request.onload = function() {
	  // The request was successfully completed!
	  console.log("Received JSON from website for " + localStorage.getItem('nodeName'));
	  var response = JSON.parse(this.responseText);
	  if (response.lPackets.length>0){
	  	var newestMessage = response.lPackets[0];
	  	var dict = {
		  'AppKeyPacket': newestMessage.p
		};
		console.log("Sending: "+newestMessage.p);
	  	Pebble.sendAppMessage(dict, messageSuccessHandler, messageFailureHandler);
	  }
	};

	// Send the request
	request.open(method, url);
	request.send();
}

