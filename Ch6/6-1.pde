import processing.core.*;
import processing.serial.*;
import mqtt.*;
MQTTClient client;     
String PC_IP="mqtt://172.20.10.2";  //hello/broker
int sequence_ID=0;
int sendSequence_ID=0;
String MQTTMessage="  ";
String MQTTMessageX="  ";
String MQTTTime="  ";
String recordFile[]= new String[1200];
int recordCounter=0;
String myFileName = "record"+day()+'_'+month()+'_'+year()+'_'+hour()+'_'+minute()+".txt";
//==========================
boolean RxComplete = false;         // Rx Complete　完成一 Packet 的接收
int[] inputBytes = new int[120]; 
int index = 0;     // Packet Array 之 Index
int TotalLength=0;      // Packetlength1
    // Your Serial Port
    Serial portSerial;   
    //Serial MEGAportSerial;  
//==============================
public void setup() {
        size(720, 360);
            PFont font = createFont("標楷體", 20);  //系統字型轉成可用字型
            textFont(font); 
        println("Available serial ports:");
        println(Serial.list());
        //portSerial= new Serial(this, "COM6", 115200);
        portSerial= new Serial(this, "/dev/ttyACM0", 115200);
        //MEGAportSerial= new Serial(this, "/dev/ttyACM0", 115200);        
        
//========  MQTT
   client =  new MQTTClient(this);
   //client.connect( "mqtt://192.168.1.106");  // test ok 
   client.connect( PC_IP);  // test ok 
   client.subscribe("hello/pi");  //  my Own 192.168.1.102
   //client.subscribe("hello/pc");  //  my Own 192.168.1.102
    }

//====================
void draw()
{
  if (RxComplete ) 
  {
      MQTTMessage="";
      println(" < data in " + TotalLength +" bytes >");
      for ( int j= 0 ; j < TotalLength ;j++)
         {
            //print(char(inputBytes[j]));
            MQTTMessage = MQTTMessage+char(inputBytes[j]);
          }   // end for
          //println("Time(m/d): "+ month()+'/'+ day()+"  "+ hour()+':'+minute()+':'+second());
          //==========MQTT
          // Note: MQTT message with CR LF  will cause a problem
          MQTTMessageX = MQTTMessage +"    Time:"+month()+'/'+ day()+" "+ hour()+':'+minute()+':'+second();
          //client.publish("hello/pc", MQTTMessageX ); 
          //client.publish("hello/pc", MQTTMessageX ); 

          client.publish("hello/broker", MQTTMessageX ); 
          client.publish("hello/sensor", MQTTMessage ); 
    fill(0,255,0);
    clear(); 
    if(char(inputBytes[0])== 'U') {
    fill(0, 0, 255);
    text(MQTTMessage+"  "+month()+'/'+ day()+"  "+ hour()+':'+minute()+':'+second(), 20, 80);  
    MQTTMessageX = MQTTMessage+"    Time: "+month()+'/'+ day()+"  "+ hour()+':'+minute()+':'+second();
    //recordFile[recordCounter]= MQTTMessageX;
    //saveStrings(myFileName,recordFile);
    recordCounter++;  
    }
    else
    { text(MQTTMessageX , 20, 80);  
    text(MQTTMessage+"  "+month()+'/'+ day()+"  "+ hour()+':'+minute()+':'+second(), 20, 200);
    //recordFile[recordCounter]= MQTTMessage+"  "+month()+'/'+ day()+"  "+ hour()+':'+minute()+':'+second();
    //saveStrings(myFileName,recordFile);
    recordCounter++;  
  }
    index = 0;
    TotalLength=0;
    RxComplete = false;
    println();
  }   // end if
  delayMs(50);
}
//===============================================
  void serialEvent(Serial myPort)
{
  try{
  //===============
  while (myPort.available()==1) 
    {
     // get the new byte:
        int inChar =   myPort.read(); 
        print(char(inChar) );
        inputBytes[index]= inChar;
        index++;
    }
    RxComplete = true;
    TotalLength =index;
    //===========
    }
  catch(RuntimeException e) {
   
  }

}
   


 
//================
void delayMs(int ms)
{
 int time =millis();
  while (millis() -time < ms);
  
}
//====================== UDP=====
void keyPressed() { 
    String message  = str( key );     // the message to send
    client.publish("hello/broker", "Hi:  ************************");
    //client.publish("hello/pc", "Hi:  ************************");
    delay(1000);
    client.publish("hello/groupx", "Hi:I am NB,  pi glad to see your world $$$$$$$$$$$$$$$$$$$$$$");
    //client.publish("hello/pc", "Hi:I am NB,  pi glad to see your world $$$$$$$$$$$$$$$$$$$$$$");
           fill(0,255,0);
           clear();
  sendSequence_ID++;
  text("Send message to hello/bk & hello/pgrounx", 20, 220);   
  text("Tx Sequence: "+sendSequence_ID, 20, 240);
}

//==============  MQTT Rx
void messageReceived(String topic, byte[] payload) {
  println("new message from: " + topic + ".    Message: " + new String(payload));
  fill(255,0,0);
  clear();
  sequence_ID++;
  text("Receive message from: " + topic + ".    Message: " + new String(payload), 20, 120);
  text("Rx Sequence: "+sequence_ID, 20, 220);
}
