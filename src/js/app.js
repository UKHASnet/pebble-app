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

function getData(){
	var method = 'GET';
	var url = 'https://ukhas.net/api/nodeData?id=143&period=60';

	// Create the request
	var request = new XMLHttpRequest();

	// Specify the callback for when the request is completed
	request.onload = function() {
	  // The request was successfully completed!
	  response = JSON.parse(this.responseText);
	};

	// Send the request
	request.open(method, url);
	request.send();
}