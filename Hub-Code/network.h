const char network_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<style>
    body, h1, h2, p, ul, li {
        margin: 0;
        padding: 0;
      }
      
      body {
        font-family: Arial, sans-serif;
      }
      
      header {
        background-color: #222;
        color: #fff;
        text-align: center;
        padding: 1rem;
      }
      
      .logo img {
        width: 60px;
        height: 60px;
        margin-bottom: 10px;
      }
      
      /* Nav Bar */
      nav {
        background-color: #444;
        padding: 0.5rem;
        display: flex;
        justify-content: space-between;
        align-items: center;
      }
      
      .nav-list {
        list-style: none;
        display: flex;
        justify-content: center;
        margin-right: auto;
      }
      
      .nav-list li {
        margin-right: 1rem;
      }
      
      .nav-list a {
        color: #fff;
        text-decoration: none;
        padding: 0.5rem 1rem;
        border-radius: 50px;
      }
      
      .nav-list a:hover {
        background-color: #666;
      }
      
      .menu-toggle {
        display: none;
        flex-direction: column;
        cursor: pointer;
        z-index: 2;
        position: relative;
      }
      
      .menu-toggle .bar {
        width: 25px;
        height: 3px;
        background-color: white;
        margin: 2px 0;
      }
      
      @media (max-width: 768px) {
        .nav-list {
          display: none;
          flex-direction: column;
          align-items: center;
          width: 100%;
          position: absolute;
          background-color: #333;
          top: 68px;
          left: 0;
          transition: transform 0.3s ease-in-out;
          transform: translateX(-100%);
        }
      
        .nav-list.active {
          transform: translateX(0);
          display: flex;
        }
      
        .menu-toggle {
          display: flex;
        }
      
        .close-button {
          display: none;
          cursor: pointer;
          position: absolute;
          top: 10px;
          right: 10px;
          z-index: 1;
        }
      
        .nav-list.active + .close-button {
          display: block;
        }
      }
      /* Nav Bar End */
      
      main {
        display: grid;
        grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
        gap: 1rem;
        padding: 1rem;
      }
      
      .device-card {
        background-color: #f0f0f0;
        padding: 1rem;
        border-radius: 10px;
        box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
      }
      
      .control-panel {
        background-color: #f7f7f7;
        padding: 1rem;
        text-align: center;
        margin-top: 2rem;
      }
      
      footer {
        background-color: #333;
        color: #fff;
        text-align: center;
        padding: 0.5rem;
      }
      
      .switch {
        position: relative;
        display: inline-block;
        width: 60px;
        height: 34px;
      }
      
      .switch input {
        opacity: 0;
        width: 0;
        height: 0;
      }
      
      .slider {
        position: absolute;
        cursor: pointer;
        top: 0;
        left: 0;
        right: 0;
        bottom: 0;
        background-color: #ccc;
        transition: .4s;
        border-radius: 34px;
      }
      
      .slider:before {
        position: absolute;
        content: "";
        height: 26px;
        width: 26px;
        left: 4px;
        bottom: 4px;
        background-color: white;
        transition: .4s;
        border-radius: 50%;
      }
      
      input:checked + .slider {
        background-color: #2196F3;
      }
      
      input:focus + .slider {
        box-shadow: 0 0 1px #2196F3;
      }
      
      input:checked + .slider:before {
        transform: translateX(26px);
      }
      
      table {
        border-collapse: collapse;
        width: 100%;
      }
      
      th, td {
        padding: 8px;
        text-align: left;
        border-bottom: 1px solid #ddd;
      }
      
      tr:nth-child(even) {
        background-color: #e4e4e4;
      }
      
      .button {
        background-color: #4CAF50;
        border: none;
        color: white;
        padding: 10px 16px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 12px;
        margin: 2px 1px;
        cursor: pointer;
      }      
</style>

<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>WASP Hub</title>
</head>

<body>
  <header>
    <div class="logo">
      <h1>WASP Hub</h1>
    </div>
  </header>
  <!-- Nav Bar -->
  <nav>
    <div class="menu-toggle" onclick="toggleNav()">
      <div class="bar"></div>
      <div class="bar"></div>
      <div class="bar"></div>
    </div>
    <ul class="nav-list" id="navList">
      <li><a href="/">Home</a></li>
      <li><a href="settings">Settings</a></li>
      <li><a href="hub">Hub</a></li>
      <li><a href="actions">Actions</a></li>
      <li><a href="network">Network</a></li>
    </ul>
    <div class="close-button" onclick="toggleNav()"></div>
  </nav>
  <!-- Nav Bar end-->

  <main>
    <div class="device-card">
      <h2>Found Devices</h2>
      <br>
      <table style="width:100%" id="devices">
      </table>
    </div>
    <div>
    </div>
  </main>

  <div class="control-panel">
  </div>

  <footer>
    <p>This website is running off a WASP Hub Device</p>
    <p>&copy; 2023 WASP Hub. All rights reserved.</p>
  </footer>
  <script>
    function toggleNav() {
      var navList = document.querySelector('.nav-list');
      navList.classList.toggle('active');
      console.log("Nav Toggle");
    }
  </script> 
  <script>

    function request(success, error) {
      var request = new XMLHttpRequest();
      request.open('GET', '/Network_Found', true);
    
      request.onload = function () {
        if (this.status >= 200 && this.status < 400) {
          // Success
          console.log("Devices Found: ", data);
    
          let table = document.querySelector("#devices");
          let keys = Object.keys(data[0]);
          generateTableHead(table, keys);
          generateTable(table, data);
          success(this.responseText, this.status);
        } else {
          //Error
          console.error("Error fetching devices:", error);
          error();
        }
      };
    
      request.onerror = function () {
        error();
      };
    
      request.send();
    }
    
/*
    $.ajax({
      type: "GET",
      url: "/Network_Found",
      dataType: "json",
      success: function(data) {
        console.log("Devices Found: ", data);
    
        let table = document.querySelector("#devices");
        let keys = Object.keys(data[0]);
        generateTableHead(table, keys);
        generateTable(table, data);
      },
      error: function(xhr, status, error) {
        console.error("Error fetching devices:", error);
      }
    });
*/
function generateTableHead(table, data) {
  let thead = table.createTHead();
  let row = thead.insertRow();
  for (let key of data) {
    let th = document.createElement("th");
    let text = document.createTextNode(key);
    th.appendChild(text);
    row.appendChild(th);
  }
  let th = document.createElement("th");
  let button = document.createTextNode("");
  th.appendChild(button);
  row.appendChild(th);
}

function generateTable(table, data) {
  for (let element of data) {
    let row = table.insertRow();
    for (key in element) {
      let cell = row.insertCell();
      let text = document.createTextNode(element[key]);
      cell.appendChild(text);
      var MAC = element[key];
    }
    let cell = row.insertCell();
    let button = document.createElement("button");
    button.setAttribute("class", "button");
    button.setAttribute('id', String(MAC));
    button.setAttribute('onclick','run('+ "'" + String(MAC) + "'" + ')');
    button.innerHTML = "Add to Network";
    cell.appendChild(button);
  }
}

function run(MAC){
  console.log(MAC);
  document.getElementById(MAC).style.backgroundColor= '#0398fc';

  var data = {
    MAC_Address: MAC
  };

  
  $.ajax({
    type: "POST",
    url: "/Network_Add",
    data: data,
    success: function(response) {
        console.log("Info sent successfully:", response);
    },
    error: function(error) {
        console.error("Error sending Info:", error);
    }
});
}
</script>
</body>

</html>
)=====";
