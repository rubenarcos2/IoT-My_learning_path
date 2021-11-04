var connection = new WebSocket('ws://' + location.hostname + '/ws', ['arduino']);

connection.onopen = function () {
  console.log('Connected: ');
};

connection.onerror = function (error) {
  console.log('WebSocket Error ', error);
};

connection.onmessage = function (e) {
  console.log('Server: ', e.data);
  processData(e.data);
};

connection.onclose = function () {
  console.log('WebSocket connection closed');
};

function updateData(elem) {
  let data;

  if (elem.id < 16) {
    updateDataTxt(elem);
    data = {
      command: "Set",
      id: elem.id,
      value: elem.value
    }
  } else {
    data = {
      command: "Set",
      id: elem.id,
      value: elem.checked
    }
  }

  let json = JSON.stringify(data);

  connection.send(json);
}

function getData() {
  for (var i = 0; i < 5; i++) {
    let data = {
      command: "Get",
      id: i
    }
    let json = JSON.stringify(data);

    connection.send(json);
  }
}

function processData(data) {
  let json = JSON.parse(data);
  json.value = json.value + 102;
  console.log(json);
  document.getElementById(json.id).value = json.value;
  updateDataTxt(json);
  document.getElementById('receivedText').textContent = "Actualizado";
}

function updateDataTxt(elem) {
  var porc = ((elem.value - 102) / (512 - 102)) * 100;
  document.getElementById("txt" + elem.id).innerHTML = porc.toFixed(2) + ' %';
}
