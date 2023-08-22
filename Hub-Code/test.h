const char test_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>AJAX Example</title>
  <script src="/jquery.min.js"></script>
</head>
<body>
  <button id="sendDataBtn">Send Data</button>

  <script>
    $(document).ready(function() {
      $('#sendDataBtn').click(function() {
        // Data to send
        var dataToSend = {
          key1: 'value1',
          key2: 'value2'
        };

        // Send data to server
        $.ajax({
          type: 'POST',
          url: 'http://your-server-url.com/receive-data', // Change this to your server URL
          data: JSON.stringify(dataToSend),
          contentType: 'application/json',
          success: function(response) {
            console.log('Data sent successfully');
          }
        });
      });
    });
  </script>
</body>
</html>
)=====";