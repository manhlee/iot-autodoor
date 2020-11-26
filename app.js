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


//connect database:
// var con = mysql.createConnection({
//     host: "localhost",
//     user: "root",
//     password: "",
//     database: "iot_prof"
//   });
  
//   con.connect(function(err) {
//     if (err) throw err;
//     console.log("Connected!");
//   });
const sendShut = () => {
    let date = new Date;
    let hour = date.getHours();
    let minute = date.getMinutes();
    let sencond = date.getSeconds();
    if(minute === 41 && sencond === 0){
        if(hour === 95){
            console.log(date);
            io.emit("status-change", JSON.parse('{"sts":"'+3+'"}'));
        }
        if(hour === 9){
            io.emit("status-change", JSON.parse('{"sts":"'+4+'"}'));
        }
    }
}

setInterval(sendShut, 1000);


io.on('connection', (socket) => {
    // console.log(`A user connected : ${socket.id}`);

    io.to(socket.id).emit("status-change", JSON.parse('{"sts":"'+0+'"}'));

    socket.on('disconnect', () => {
        console.log(`${socket.id} disconnectted`);
    });

});

app.post('/state', (req, res) => {
    console.log(req.body);
    io.emit('status-change', JSON.parse('{"state":"'+req.body.state+'"}'));
    res.send();
});

app.get('/', (req, res) => {
    console.log(req.params);
    res.render('index');
});

app.get('/disable-server', (req, res) => {
   
});
