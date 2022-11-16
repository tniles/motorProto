/*******************************************************************************
    arduinoMoto

    Copyright 2022 Tyler Niles

    This file is part of motorProto. 

    Prints a message to the Serial Monitor.
    Graphical representation is available using Serial Plotter (Tools > Serial 
    Plotter menu).

    Arduino Redboard/Uno is the slave in this configuration with Software Flow 
    Control of the UART (PC is the master).

    Created leveraging arduino examples in the public domain and online 
    documentation.

    This program is free software: you can redistribute it and/or modify it 
    under the terms of the GNU General Public License as published by the Free 
    Software Foundation, either version 3 of the License, or (at your option) 
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT 
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
    more details.

    You should have received a copy of the GNU General Public License along 
    with this program. If not, see <https://www.gnu.org/licenses/>. 

*******************************************************************************/

#define BREATHER    10
#define PAUSE       100
#define SER_INIT    1000
#define BAUD_RATE   9600        // works in arduino ser mon and qt app
//#define BAUD_RATE   19200     // works in arduino ser mon, not in qt app (gets chars but its junk)
//#define BAUD_RATE   115200    // works in arduino ser mon, not in qt app (no chars)
//#define BAUD_RATE   500000    // works in arduino ser mon, not in qt app (no chars)


String RTS  = "RTS";
String CTS  = "CTS";
String XON  = "XON";
String XOFF = "XOFF";

bool flowCtrlEn = false;
int  number     = 0;


// the setup routine runs once when you press reset:
void setup()
{
    // initialize serial communication at 9600 bits per second:
    Serial.begin(BAUD_RATE);

    delay(SER_INIT);

    // wait for serial to become available
    while(!Serial.available())
    {
        delay(BREATHER);
    }

    // let master know we're ready to receive (assert clear to send)
    Serial.println(CTS);
    delay(BREATHER);
}

// the loop routine runs over and over again forever:
void loop()
{
    String readStr = " ";

    // if disabled, wait for master to enable flow control (must assert RTS)
    while (!flowCtrlEn)
    {
        readStr = " ";
        if ( Serial.available() )
        {
            readStr = Serial.readString();
            readStr.trim();
        }

        if ( (readStr.equalsIgnoreCase(RTS)) || (readStr.equalsIgnoreCase(XON)) )
        {
            flowCtrlEn = true;
            Serial.println("Flow Enabled");
        }

        delay(BREATHER);
        //Serial.println(readStr);                            // debug
    }

    // if XON, read from serial port, then boomerang:
    if ( Serial.available() && flowCtrlEn )
    {
        readStr = Serial.readString();
        readStr.trim();

        if ( (readStr.equalsIgnoreCase(XOFF)) )
        {
            flowCtrlEn = false;
            Serial.println("Flow Disabled");
        }

        delay(PAUSE);
        Serial.println(readStr);
    }
  
    delay(BREATHER);
}


