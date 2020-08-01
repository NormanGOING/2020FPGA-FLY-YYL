var AWSIoTData = require('aws-iot-device-sdk');
var AWS = require("aws-sdk");
var AWSConfiguration = require("./aws-configuration.js");
//
// Replace the values of '<YourUniqueClientIdentifier>' and '<YourCustomEndpoint>'
// with a unique client identifier and custom host endpoint provided in AWS IoT.
// NOTE: client identifiers must be unique within your AWS account; if a client attempts 
// to connect with a client identifier which is already in use, the existing 
// connection will be terminated.
//
/*
 * Copyright 2015-2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

//
// Instantiate the AWS SDK and configuration objects.  The AWS SDK for
// JavaScript (aws-sdk) is used for Cognito Identity/Authentication, and
// the AWS IoT SDK for JavaScript (aws-iot-device-sdk) is used for the
// WebSocket connection to AWS IoT and device shadow APIs.
//

console.log("Loaded AWS SDK for JavaScript and AWS IoT SDK for Node.js");
//
// Remember our current subscription topic here.
//
var currentlySubscribedTopic = "subscribe-topic";

//
// Remember our message history here.
//
var messageHistory = "";

//
// Create a client id to use when connecting to AWS IoT.
//
var clientId = "mqtt-explorer-" + Math.floor(Math.random() * 100000 + 1);

//
// Initialize our configuration.
//
AWS.config.region = AWSConfiguration.region;

AWS.config.credentials = new AWS.CognitoIdentityCredentials({
  IdentityPoolId: AWSConfiguration.poolId,
});

//
// Create the AWS IoT device object.  Note that the credentials must be
// initialized with empty strings; when we successfully authenticate to
// the Cognito Identity Pool, the credentials will be dynamically updated.
//
const mqttClient = AWSIoTData.device({
  //
  // Set the AWS region we will operate in.
  //
  region: AWS.config.region,
  //
  ////Set the AWS IoT Host Endpoint
  host: AWSConfiguration.host,
  //
  // Use the clientId created earlier.
  //
  clientId: clientId,
  //
  // Connect via secure WebSocket
  //
  protocol: "wss",
  //
  // Set the maximum reconnect time to 8 seconds; this is a browser application
  // so we don't want to leave the user waiting too long for reconnection after
  // re-connecting to the network/re-opening their laptop/etc...
  //
  maximumReconnectTimeMs: 8000,
  //
  // Enable console debugging information (optional)
  //
  debug: true,
  //
  // IMPORTANT: the AWS access key ID, secret key, and sesion token must be
  // initialized with empty strings.
  //
  accessKeyId: "",
  secretKey: "",
  sessionToken: "",
});

//
// Attempt to authenticate to the Cognito Identity Pool.  Note that this
// example only supports use of a pool which allows unauthenticated
// identities.
//
var cognitoIdentity = new AWS.CognitoIdentity();
AWS.config.credentials.get(function (err, data) {
  if (!err) {
    console.log("retrieved identity: " + AWS.config.credentials.identityId);
    var params = {
      IdentityId: AWS.config.credentials.identityId,
    };
    cognitoIdentity.getCredentialsForIdentity(params, function (err, data) {
      if (!err) {
        //
        // Update our latest AWS credentials; the MQTT client will use these
        // during its next reconnect attempt.
        //
        mqttClient.updateWebSocketCredentials(
          data.Credentials.AccessKeyId,
          data.Credentials.SecretKey,
          data.Credentials.SessionToken
        );
      } else {
        console.log("error retrieving credentials: " + err);
        alert("error retrieving credentials: " + err);
      }
    });
  } else {
    console.log("error retrieving identity:" + err);
    alert("error retrieving identity: " + err);
  }
});

mqttClient.on("connect", function () {
  console.log("connect");
  mqttClient.subscribe("$aws/things/myTestThing/shadow/update");
});

var most="0";
var number=0;
mqttClient.on("message", function (topic, payload) {
  var num_Count = JSON.parse(payload.toString());
  console.log(num_Count);

  var current=num_Count.key;
  
  function getbigger(most,current){
    if(current)
      number++;

    return number;
  }
  most=getbigger(most,current);
  console.log(most);
  //document.getElementsByClassName("count")[0].innerHTML=num_Count.key;
  document.getElementsByClassName("count")[0].innerHTML=most;

  // console.log("message", topic, payload.toString());
});

