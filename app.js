var express = require('express');
const { disconnect, exit } = require("process");
var app = express();
app.use(express.static('public'));
app.set('view engine', 'ejs');
app.set('views', './views');

var server = require('http').Server(app);
var io = require('socket.io')(server);
var mysql = require('mysql');

var bodyParser =  require('body-parser');
var logger =  require ('morgan');


app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(logger('dev'));

server.listen(3000);

let stateDoor;
let isEnable;

// connect database:
var con = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "",
    database: "iot_autodoor"
  });
  
  con.connect(function(err) {
    if (err) throw err;
    console.log("Connected!");
  });

const sendShut = () => {
    let date = new Date;
    let hour = date.getHours();
    let minute = date.getMinutes();
    let sencond = date.getSeconds();
    if(minute === 0 && sencond === 0){
        if(hour === 6){
            io.emit("status-change", JSON.parse('{"sts":"'+3+'"}'));
            isEnable = 3;
            
        }
        if(hour === 18){
            isEnable = 4;
            io.emit("status-change", JSON.parse('{"sts":"'+4+'"}'));
        }

        con.query(`update iot_controll set state = ${isEnable} where id = 2`, function (err, result) {
            if (err) throw err;
        });
        con.commit();
    }
}

setInterval(sendShut, 1000);


io.on('connection', (socket) => {

    io.to(socket.id).emit("status-change", JSON.parse('{"sts":"'+0+'"}'));

    socket.on('disconnect', () => {
        console.log(`${socket.id} disconnectted`);
    });

});

app.post('/state', (req, res) => {
    if(0 === parseInt(req.body.state)){
        stateDoor=0;
    }
    else {stateDoor = 1;}
    
    con.query(`update iot_controll set state = ${stateDoor} where id = 1`, function (err, result) {
        if (err) throw err;
    });
    con.commit();

    io.emit('status-change', JSON.parse('{"state":"'+req.body.state+'"}'));
    res.send();
});

app.get('/', (req, res) => {

    con.query("select * from iot_controll", function (err, result) {
        if (err) throw err;
        
        isEnable = result[1].state;
        stateDoor = result[0].state;

    });

    res.render('index', {isEnable});
});

