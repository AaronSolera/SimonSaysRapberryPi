let app = require('express')();
let http = require('http').createServer(app);
let io = require('socket.io')(http);

let cors = require('cors')
const {exec} = require('child_process');


let corsOptions = {
	origin: 'http://localhost:4200',
	credentials: true
}

app.use(cors(corsOptions));
//app.options('*',cors());
//app.use('/',express.static('public'));

app.get('/', (req,res) => {
	res.sendFile(__dirname + '/public/index.html');
});



io.on('connection', socket => {
  console.log('Client Connected');
  socket.on('setup', (msg) => {
    let result=[2,3,4,5];
    let count = 0;
    let response = [msg[0], msg[2], msg[4], msg[6] ];
    for(let i=0; i<4; i++){
	console.log(`${response[i]}   ${result[i]}`);
	if(response[i]==result[i]){
		count = count +1;
	}
    }

    if(count == 4){
	console.log("Iguales");
    }
    else{
	console.log("Distintos")
    }

  });


  socket.on('disconnect', () => {
    exec('Client disconnected');
  });

});

let port = 3344;
http.listen(port, () =>{
	console.log(`Taken port ${port} to serve HTTP server`);
});