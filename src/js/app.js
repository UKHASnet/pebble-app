// Function to send a message to the Pebble using AppMessage API
// We are currently only sending a message using the "status" appKey defined in appinfo.json/Settings
function sendMessage() {
	Pebble.sendAppMessage({"status": 1}, messageSuccessHandler, messageFailureHandler);
}

// Called when the message send attempt succeeds
function messageSuccessHandler() {
  console.log("Message send succeeded.");  
}

// Called when the message send attempt fails
function messageFailureHandler() {
  console.log("Message send failed.");
  sendMessage();
}

// Called when JS is ready
Pebble.addEventListener("ready", function(e) {
  console.log("JS is ready!");
  sendMessage();
});
												
// Called when incoming message from the Pebble is received
// We are currently only checking the "message" appKey defined in appinfo.json/Settings
Pebble.addEventListener("appmessage", function(e) {
  console.log("Received Message: " + e.payload.message);
});

//The pebble message contains data on the node's most recent packets
//each individual packet data has the structure. 
//{"p":"0lV4.28[BOX0,AH1,AJ1,AJ2,RUSS1,DB02,MB31]","t":"2016-03-20T14:29:36.875Z"}
function getData(){
	var method = 'GET';
	var url = 'https://ukhas.net/api/nodePackets?name=BOX0';

	// Create the request
	var request = new XMLHttpRequest();

	// Specify the callback for when the request is completed
	request.onload = function() {
	  // The request was successfully completed!
	  response = JSON.parse(this.responseText);
	  response.status = 1;
	  Pebble.sendAppMessage(response, messageSuccessHandler, messageFailureHandler);
	};

	// Send the request
	request.open(method, url);
	request.send();
}