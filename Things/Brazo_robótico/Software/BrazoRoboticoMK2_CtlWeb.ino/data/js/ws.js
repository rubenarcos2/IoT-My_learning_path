var connection = new WebSocket('ws://' + location.hostname + '/ws', ['arduino']);
connection.onopen = function () {
    connection.send('Connect ' + new Date());
};
connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
    alert('Se ha perdido la conexi&oacuten', error);
};
connection.onmessage = function (e) {
    console.log('Server: ', e.data);
};

function send(id, value, max) {
    console.log("Eje " + id + ":" + value + " max " + max);
    var data = {"command":"Set","id":id,"value": value}
    data = JSON.stringify(data);
    connection.send(data);
}

