#include "WebServer.h"
WebServer server(80);

// HTML page with dropdowns
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>Time Setter</title>
</head>
<body>

<h2>Set Time (Hours & Minutes)</h2>

<label>Hour:</label>
<select id="hour">
  <!-- JS will fill this with 0–23 -->
</select>

<br><br>

<label>Minute:</label>
<select id="minute">
  <!-- JS fills 0–59 -->
</select>

<br><br>

<button onclick="sendData()">Send</button>

<p id="response"></p>

<script>
// populate hour dropdown
const hourSel = document.getElementById("hour");
for (let i = 0; i <= 23; i++) {
  let opt = document.createElement("option");
  opt.value = i;
  opt.text = i;
  hourSel.appendChild(opt);
}

// populate minute dropdown
const minSel = document.getElementById("minute");
for (let i = 0; i <= 59; i++) {
  let opt = document.createElement("option");
  opt.value = i;
  opt.text = i;
  minSel.appendChild(opt);
}

function sendData() {
  let h = document.getElementById("hour").value;
  let m = document.getElementById("minute").value;

  fetch(`/set?hour=${h}&minute=${m}`)
    .then(response => response.text())
    .then(data => {
      document.getElementById("response").innerHTML = data;
    });
}
</script>

</body>
</html>
)=====";

int receivedHour = -1;
int receivedMinute = -1;

// Serve the page
void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}

// Handle incoming values
void handleSet() {
  if (server.hasArg("hour") && server.hasArg("minute")) {
    
    receivedHour = server.arg("hour").toInt();
    receivedMinute = server.arg("minute").toInt();

    Serial.printf("Received Hour=%d  Minute=%d\n", receivedHour, receivedMinute);

    server.send(200, "text/plain", "Received hour=" + String(receivedHour) +
                                   " minute=" + String(receivedMinute));
  }
  else {
    server.send(400, "text/plain", "Missing hour or minute");
  }
}