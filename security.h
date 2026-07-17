#ifndef SECURITY_H
#define SECURITY_H

#include <WebServer.h>


extern WebServer server;

extern bool clientLocked;

extern IPAddress activeClient;



bool isClientAllowed()
{

    IPAddress currentClient = server.client().remoteIP();


    // If old client disconnected
    if(clientLocked)
    {
        if(!server.client().connected())
        {
            clientLocked = false;

            Serial.println("CLIENT LOCK RELEASED");
        }
    }



    // First client
    if(!clientLocked)
    {

        clientLocked = true;

        activeClient = currentClient;


        Serial.println();
        Serial.println("======================================");
        Serial.println("CLIENT LOCK CREATED");

        Serial.print("Client : ");
        Serial.println(activeClient);

        Serial.println("======================================");


        return true;
    }



    // Same client allowed
    if(currentClient == activeClient)
    {
        return true;
    }



    // Different client blocked

    Serial.println();
    Serial.println("======================================");
    Serial.println("ACCESS DENIED");

    Serial.print("Current Client : ");
    Serial.println(currentClient);

    Serial.print("Active Client  : ");
    Serial.println(activeClient);

    Serial.println("======================================");


    return false;

}



#endif