import processing.serial.*;
Serial myPort; //creates a software serial port on which you will listen to Arduino
Table table; //table where we will read in and store values. You can name it something more creative!
 
int numReadings = 500; //keeps track of how many readings you'd like to take before writing the file. 
int readingCounter = 0; //counts each reading to compare to numReadings. 
 
String fileName;
void setup()
{
  String portName = Serial.list()[31]; 
  //CAUTION: your Arduino port number is probably different! Mine happened to be 1. Use a "handshake" sketch to figure out and test which port number your Arduino is talking on. A "handshake" establishes that Arduino and Processing are listening/talking on the same port.
  //Here's a link to a basic handshake tutorial: https://processing.org/tutorials/overview/
  
  myPort = new Serial(this, portName, 9600); //set up your port to listen to the serial port
   
  table.addColumn("id"); //This column stores a unique identifier for each record. We will just count up from 0 - so your first reading will be ID 0, your second will be ID 1, etc. 
  
  //the following adds columns for time. You can also add milliseconds. See the Time/Date functions for Processing: https://www.processing.org/reference/ 
  table.addColumn("DV");
  table.addColumn("CV");
  table.addColumn("HV");
  table.addColumn("LV");
  table.addColumn("D");
  table.addColumn("MeasHV");
  table.addColumn("MeasLV");
  table.addColumn("MeasD");
  table.addColumn("CTIABefore");
  table.addColumn("CTIAAfter");
 
}
 
void serialEvent(Serial myPort){
  String val;
  val = myPort.readStringUntil('\n'); //
  if (val!= null) { //We have a reading! Record it.
    val = trim(val); //gets rid of any whitespace or Unicode nonbreakable space
    println(val); //Optional, useful for debugging. If you see this, you know data is being sent. Delete if  you like. 
    float sensorVals[] = float(split(val, ',')); //parses the packet from Arduino and places the valeus into the sensorVals array. I am assuming floats. Change the data type to match the datatype coming from Arduino. 
   
    TableRow newRow = table.addRow(); //add a row for this new reading
    newRow.setInt("id", table.lastRowIndex());//record a unique identifier (the row's index)
    
    //record sensor information. Customize the names so they match your sensor column names. 
    newRow.setFloat("DV", sensorVals[0]);
    newRow.setFloat("CV", sensorVals[1]);
    newRow.setFloat("HV", sensorVals[2]);
    newRow.setFloat("LV", sensorVals[3]);
    newRow.setFloat("D", sensorVals[4]);
    newRow.setFloat("MeasHV", sensorVals[5]);
    newRow.setFloat("MeasLV", sensorVals[6]);
    newRow.setFloat("MeasD", sensorVals[7]);
    newRow.setFloat("CTIABefore", sensorVals[8]);
    newRow.setFloat("CTIAAfter", sensorVals[9]);
    
    readingCounter++; //optional, use if you'd like to write your file every numReadings reading cycles
    
    //saves the table as a csv in the same folder as the sketch every numReadings. 
    if (readingCounter % numReadings ==0)//The % is a modulus, a math operator that signifies remainder after division. The if statement checks if readingCounter is a multiple of numReadings (the remainder of readingCounter/numReadings is 0)
    {
      fileName = str(year()) + str(month()) + str(day()) + str(dataTable.lastRowIndex()); //this filename is of the form year+month+day+readingCounter
      saveTable(table, fileName); //Woo! save it to your computer. It is ready for all your spreadsheet dreams. 
    }
   }
}
 
void draw()
{ 
   //visualize your sensor data in real time here! In the future we hope to add some cool and useful graphic displays that can be tuned to different ranges of values. 
}