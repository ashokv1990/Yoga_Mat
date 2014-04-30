
/**

Node JS Video Event Server
Modules: socket io
         express

Ports : 7778 Server socket that listens to the yoga mat
        7777 Server socket that listens to polling web client
**/



var io = require('socket.io').listen(7777);

/*
Status flags depending on the active state of the mat
*/
var flag1 = 1;
var flag2 = 1;
var flag3 = 1;
var flag4 = 1;
var flag5 = 1;
var flag6 = 1;
var flag7 = 1;
var flag0 = 1;


require('net').createServer(function (socket) {
    
     console.log("connected");
socket.on('data', function (data) {
      
      //convert data to integer
      var d = data.toString();
      var num = parseInt(d);
      //log the data
      console.log(d );
      console.log(num); 

      console.log("parsed");
      
      //store the encoded mat status received in status variable
      status = num;

      /*

      status flags record the yoga mat status
      */

      if(parseInt(num.valueOf(),10) == 7) 
      {
         flag7 = 0; 
      }
      else if(parseInt(num.valueOf(),10) == 5) 
      {
        flag5 = 0;
      }
      else if(parseInt(num.valueOf(),10) == 6) 
      {
        flag6 = 0;
      }
     else if(parseInt(num.valueOf(),10) == 5) 
      {
        flag5 = 0;
       
      }
      else if(parseInt(num.valueOf(),10) == 4) 
      {
        flag4 = 0;
      
      }
       else if(parseInt(num.valueOf(),10) == 3) 
      {
        flag3 = 0;
      
        
      }
       else if(parseInt(num.valueOf(),10) == 2)
      {
        flag2 = 0;
        
      }
       else if(parseInt(num.valueOf(),10) == 1) 
      {
        flag1 = 0;
      
        
      }
      else if(parseInt(num.valueOf(),10) == 0) 
      {
        flag0 = 0;
      
        
      }  
        
    });
})

.listen(7778);

io.set("transports", ["xhr-polling"]); // set the connection to long polling

io.set("polling duration", 10);  //set the poll duration of the server to 10 seconds 

io.set("close timeout", 3);  //set connection timeout to 3 seconds

io.sockets.on('connection', function (socket) {

    //log address of received connection
     var address = socket.handshake.address;
     
     console.log("New connection from " + address.address + ":" + address.port);
    
    
    //send out javascript events to clients to indicate mat status
   if(flag7 == 0)
  {
   socket.emit('news7' , {test:"success!"});
   flag7 = 1;
  }

    else if(flag5 == 0)
  {
   socket.emit('news5' , {test:"success!"});
   flag5 = 1;
  }
  else if(flag6 == 0)
  {
   socket.emit('news6' , {test:"success!"});
   flag6 = 1;
  }
  else if(flag3 == 0)
  {
    socket.emit('news3' , {test:"success!"});
    flag3 = 1;
  }

  else if(flag1 == 0)
  {
    socket.emit('news1' , {test:"success!"});
    flag1 = 1;
  }
  else if(flag2 == 0)
  {
    socket.emit('news2' , {test:"success!"});
    flag2 = 1;
  }
  
  else if(flag4 == 0)
  {
    socket.emit('news4' , {test:"success!"});
    flag4 = 1;
  }
 
  else if(flag0 == 0)
  {
   socket.emit('news0' , {test:"success!"});
 
   flag0 = 1; 
  }
  





   
    //socket.disconnect();
});

