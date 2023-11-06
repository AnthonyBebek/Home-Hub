const char graphs_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<style>
  body,
  h1,
  h2,
  p,
  ul,
  li {
    margin: 0;
    padding: 0;
  }

  h3,
  h4 {
    width: 50%;
    height: 60px;
    margin: 0;
    padding: 0;
    display: inline;
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
    border-radius: 5px;
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
      top: 60px;
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

    .nav-list.active+.close-button {
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

  .device-card2 {
    background-color: #ffffff;
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
    -webkit-transition: .4s;
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
    -webkit-transition: .4s;
    transition: .4s;
    border-radius: 50%;
  }

  input:checked+.slider {
    background-color: #2196F3;
  }

  input:focus+.slider {
    box-shadow: 0 0 1px #2196F3;
  }

  input:checked+.slider:before {
    -webkit-transform: translateX(26px);
    -ms-transform: translateX(26px);
    transform: translateX(26px);
  }

  .category-caption {
    font-weight: bold;
    font-size: 18px;
    color: #171616;
    margin: 0;
    padding: 0;
  }

  .category-table {
    margin-bottom: 20px;
  }

  table {
    border-collapse: collapse;
    width: 100%;
  }

  th,
  td {
    padding: 8px;
    text-align: left;
    border-bottom: 1px solid #ddd;
  }

  tr:nth-child(even) {
    background-color: #e4e4e4;
  }

  /* 
      @media (max-width: 768px) {
      .category-caption {
        font-weight: bold;
        font-size: 18px;
        color: #171616;
        margin: 0;
        padding: 0;
      }  
      .category-table {
        margin-bottom: 20px;
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
    }
         */

  /* Switch CSS */

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
    -webkit-transition: .4s;
    transition: .4s;
  }

  .slider:before {
    position: absolute;
    content: "";
    height: 26px;
    width: 26px;
    left: 4px;
    bottom: 4px;
    background-color: white;
    -webkit-transition: .4s;
    transition: .4s;
  }

  input:checked+.slider {
    background-color: #2196F3;
  }

  input:focus+.slider {
    box-shadow: 0 0 1px #2196F3;
  }

  input:checked+.slider:before {
    -webkit-transform: translateX(26px);
    -ms-transform: translateX(26px);
    transform: translateX(26px);
  }

  /* Rounded sliders */
  .slider.round {
    border-radius: 34px;
  }

  .slider.round:before {
    border-radius: 50%;
  }
</style>

<head>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js"></script>
  <script src = "https://code.jquery.com/jquery-3.7.1.min.js"></script>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Smart Home Hub</title>
</head>

<body>
  <header>
    <div class="logo">
      <h1>Smart Home Hub</h1>
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
      <li><a href="graphs">Graphs</a></li>
    </ul>
    <div class="close-button" onclick="toggleNav()"></div>
  </nav>
  <!-- Nav Bar end-->

  <main>

    <script>
      const xValues2 = [100,200,300,400,500,600,700,800,900,1000];
      
      new Chart("myChart", {
        type: "line",
        data: {
          labels: xValues2,
          datasets: [{ 
            data: [860,1140,1060,1060,1070,1110,1330,2210,7830,2478],
            borderColor: "red",
            fill: false
          }, { 
            data: [1600,1700,1700,1900,2000,2700,4000,5000,6000,7000],
            borderColor: "green",
            fill: false
          }, { 
            data: [300,700,2000,5000,6000,4000,2000,1000,200,100],
            borderColor: "blue",
            fill: false
          }]
        },
        options: {
          legend: {display: false}
        }
      });
      </script>
      

  <script>
    function toggleNav() {
      var navList = document.querySelector('.nav-list');
      navList.classList.toggle('active');
      console.log("Nav Toggle");
    }
  </script>
  <script>
        const colors = ["rgba(0, 0, 255)", "rgba(0, 255, 0)", "rgba(255, 0, 0)"];
    const subcolors = ["rgba(0, 0, 255, 0.5)", "rgba(0, 255, 0, 0.5)", "rgba(255, 0, 0, 0.5)"];
  $.ajax({
    type: "GET",
    url: "/Graph_Data",
    dataType: 'text',
    async: false,
    success: function(data) {
      console.log(String(data));
      const dataset = data.split("!");
    for (let y = 1; y < dataset.length; y++){
    const datasetsub = dataset[y].split("\n");
    let name;
    let fulldata = [];
    var temp;
    var times = [];
    var data1 = [];
    var data2 = [];
    for (let i = 0; i < datasetsub.length; i++) {
      if (!datasetsub[i] == ""){
      if (!datasetsub[i].endsWith(".txt")) {
        temp = datasetsub[i].split(",");
        fulldata.push(temp);
        for (var p = 0; p < fulldata.length; p++){
        times[p] = fulldata[p][0];
        data1[p] = fulldata[p][1];
        data2[p] = fulldata[p][2];
    }
      }
      else {
        name = datasetsub[i];
        name = name.slice(0, -4);
        console.log("Name: " + name);
      }
    }}
    createChart(name, times, data1, data2);
  }
    },
    error: function(xhr, status, error) {
      console.error("Error fetching devices:", error);
    }
  });


    function createChart(SensorName, xAxisData, plotData, plotData2) {
    console.log("Sensor Name: " + SensorName);
    const canvasId = "dynamicChartCanvas";

    const containerDiv = document.createElement('div');
    containerDiv.className = 'device-card';

    const canvas = document.createElement('canvas');
    canvas.id = canvasId;

    containerDiv.appendChild(canvas);
    const mainElement = document.querySelector('main');
    mainElement.appendChild(containerDiv);

    const ctx = canvas.getContext('2d');

    const maxData1Value = Math.max(...plotData);
    const maxData2Value = Math.max(...plotData2);

  const bufferValue = 1;

    new Chart(ctx, {
      type: "line",
      data: {
        labels: xAxisData,
        datasets: [{
          label: "Temperature",
          data: plotData,
          borderColor: colors[0],
          backgroundColor: subcolors[0],
          fill: true,
          yAxisID: 'y-axis-1'
        }, {
          label: "Humidity",
          data: plotData2,
          borderColor: colors[1],
          backgroundColor: subcolors[1],
          fill: true,
          yAxisID: 'y-axis-2'
        }]
      },
      options: {
        title: {
          display: true,
          text: SensorName,
        },
        scales: {
          yAxes: [{
            id: 'y-axis-1',
            type: 'linear',
            position: 'left',
            scaleLabel: {
              display: true,
              labelString: 'Temperature'
            }
          }, {
            id: 'y-axis-2',
            type: 'linear',
            position: 'right',
            scaleLabel: {
              display: true,
              labelString: 'Humidity'
            }
          }]
        },
        legend: { display: true }
      }
    });
  }
  window.setTimeout( function() {
  window.location.reload();
}, 30000);
</script>
</main>
</body>
<footer>
  <p>This website is running off a Home Hub Device</p>
  <p>&copy; 2023 Home Hub. All rights reserved.</p>
</footer>
</html>
)=====";