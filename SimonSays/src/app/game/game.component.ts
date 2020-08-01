import { Component, OnInit } from '@angular/core';
import { SocketioService } from 'src/app/socketio.service';



@Component({
  selector: 'app-game',
  templateUrl: './game.component.html',
  styleUrls: ['./game.component.css']
})
export class GameComponent implements OnInit {
  order = Object[10];
  amount = 0;
  constructor(private socketService: SocketioService) { 
    this.order = [];
  }

  ngOnInit() {
    
  }

  sendMsg(){
    this.socketService.setUpPins(this.order);
    this.order=[];
    this.amount = 0;
  }

  clearResponse(){
    this.order=[];
    this.amount = 0;
  }

  pushButton(id){
    if(this.order.length<4){
      this.order.push(id);
      this.amount = this.amount + 1;
    }
    
    
  }

  

}
