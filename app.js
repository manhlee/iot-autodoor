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
var con = mysql.createConnection({
    host: "localhost",
    user: "root",
    password: "",
    database: "iot_prof"
  });
  
  con.connect(function(err) {
    if (err) throw err;
    console.log("Connected!");
  });


//CRUD:


let esp;

io.on('connection', (socket) => {
    // console.log(`A user connected : ${socket.id}`);

    io.to(socket.id).emit("req_info_esp", JSON.parse('{"e":"esp"}'));
    socket.on('get-ports', (data) =>{
        console.log(data);
        if(esp === null || esp === undefined){
            esp = {
                name : data.name,
                id: socket.id
            };
            con.query("select * from iot_sensor", function (err, result) {
                if (err) throw err;
                var res = `{`;
                result.forEach(element => {
                    res+=`\"${element.id}\":\"${element.status}\",`
                });
                res = res.substr(0, res.length-1);
                res+=`}`;


                io.to(esp.id).emit('status-ports', JSON.parse(res));
            });
        }
    });

    socket.on('disconnect', () => {
        console.log(`${socket.id} disconnectted`);
    });

    socket.on('get-data', data => {
        con.query("select * from iot_sensor", function (err, result) {
            if (err) throw err;
            io.emit('data-homepage', {'title': 'Home page', 'data': result});
            // console.log(result);
          });
    });

    socket.on('success-notify', data => {
        console.log('----------------DONE---------------');
    });

    socket.on('data-mucnuoc', data => {
        let date = new Date();
        let da = `\"${date.getFullYear()}-${date.getMonth()}-${date.getDate()}:${date.getHours()}-${date.getMinutes()}-${date.getSeconds()}\"`;

        
        let sql = `INSERT INTO iot_wt(id, value, time) VALUES (NULL,${(parseInt(data))},${da})`;
        console.log(da);
        console.log(sql);
        con.query(sql, function (err, result) {
            if (err) throw err;
            else console.log(result);    
        });
    });

});

app.get('/', (req, res) => {
    res.render('index', );
});

app.post('/ajax', (req, res) => {
    con.query(`update iot_sensor set status = ${req.body.status} where id = ${req.body.port}`, function (err, result) {
        if (err) throw err;
    });
    con.commit();
    var obj =  `{\"${req.body.port}\":\"${req.body.status}\"}`;
    io.emit("status-port-change", JSON.parse(obj));
    res.send();
});
