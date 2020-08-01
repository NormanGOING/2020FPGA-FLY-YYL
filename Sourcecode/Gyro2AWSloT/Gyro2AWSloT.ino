#include <AWS_IOT.h>
#include <SimpleDHT.h>
#include <WiFi.h>
#include "sea_esp32_qspi.h"
#include "string.h"

#include <spartan-edge-esp32-boot.h>
#include "ESP32IniFile.h"

// initialize the spartan_edge_esp32_boot library
spartan_edge_esp32_boot esp32Cla;

const size_t bufferLen = 80;
char buffer[bufferLen];
char buffer1[bufferLen];

AWS_IOT hornbill;

char WIFI_SSID[]="WAS-AL00";
char WIFI_PASSWORD[]="147258@@@";
char HOST_ADDRESS[]="a1e4xn0rexdbtf-ats.iot.us-east-1.amazonaws.com";
char CLIENT_ID[]= "MyTestClient";
char TOPIC_NAME[]= "$aws/things/myTestThing/shadow/update";

const char aws_root_ca_pem[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\
rqXRfboQnoZsG4q5WTP468SQvvG5\
-----END CERTIFICATE-----\n"};

const char certificate_pem_crt[] = {"-----BEGIN CERTIFICATE-----\n\
MIIDLDCCAhSgAwIBAgIUCUtxJpHkv/1tTOrfzyhCHj4nUFIwDQYJKoZIhvcNAQEL\
BQAwIDEeMBwGA1UECwwVQW1hem9uIFdlYiBTZXJ2aWNlcyBPMB4XDTIwMDcyOTAz\
NDAxOFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZp\
Y2F0ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMflBCyf+iL0MkUf\
l4qaANMF8iCHcLKbyKQENoxeJ+1heIrBNOTb58mLJH3rgUg5Blh2/WERX60PFcxs\
vTbBO+gVa8MiaMEn8AbDh4yu6dnEQb4plsjBnraDeJ3Dg7SFVD1/x/y7LOBnSC8P\
y64u1pVNr21u3bQ7F77W6Rh+kKVuqEjIWqDKI5vQw+xD9neklWE/8q3fbS+YiD/A\
PXX2zORRpDwC3jD+ouaPqM9uZ/+GKbm1WR2dW5MA97D6isUWdkI5psag9NaK2wUf\
3ZxXeo2rbAhg2yPWthJRgRHaG+yUrxb7YsdCh+nLTJ04AvjgFe+BI5s/xGkfTSyC\
aUQCMtsCAwEAAaNgMF4wHwYDVR0jBBgwFoAUJP/Bd1S8zerSD6OEHkjDk90j7rEw\
HQYDVR0OBBYEFEu3rXxfeArqQKSrbFudA1NNhkZAMAwGA1UdEwEB/wQCMAAwDgYD\
VR0PAQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCQHFy9XTou/kL1sDQRCgk1\
id3OBRoMcCPZZp0JcWSb12P8uAXpry4Mjbib5IU7/gv91nTOtIHs+iNXZpGNkmZ0\
aHv1/RxkcPU5S2AQOI9tRq0cBHgF8A3FZfIlfIBu8H/7t7CuNiSpgqNeUdUTsKXo\
qLpJZPT6xjJirVLkwYsDReoJS1hY8oHzyHu1yIvQQZ0BUZlCMSuE0QQIYwImFD/C\
Te3az47nxFvLzIYUj/DG1rr/C24b4OF+DnXUdDp65VVKOKoyiAhYsA3BJDq1+MHm\
h+Qrm5VoJ4fvPeCzUxC1EeGmEisBZ7rPEPI1EJzQB74wcKQENBQioBwMS2XNPtwa\
-----END CERTIFICATE-----\n"};

const char private_pem_key[] = {"-----BEGIN RSA PRIVATE KEY-----\n\
MIIEowIBAAKCAQEAx+UELJ/6IvQyRR+XipoA0wXyIIdwspvIpAQ2jF4n7WF4isE0\
5NvnyYskfeuBSDkGWHb9YRFfrQ8VzGy9NsE76BVrwyJowSfwBsOHjK7p2cRBvimW\
yMGetoN4ncODtIVUPX/H/Lss4GdILw/Lri7WlU2vbW7dtDsXvtbpGH6QpW6oSMha\
oMojm9DD7EP2d6SVYT/yrd9tL5iIP8A9dfbM5FGkPALeMP6i5o+oz25n/4YpubVZ\
HZ1bkwD3sPqKxRZ2QjmmxqD01orbBR/dnFd6jatsCGDbI9a2ElGBEdob7JSvFvti\
x0KH6ctMnTgC+OAV74Ejmz/EaR9NLIJpRAIy2wIDAQABAoIBAQCFsr36Av5RBrfg\
1+KKB7tpaV6rNyu/qsamPVMmy4s1EK6JT/TK5fLAMNq1hiEwZ0PBQ8Txeadenupb\
kpdz2gbxkfJDz+3003uNapZtGEi9KLE4OaTqneZD+OdhzfV3dn4DrEXc2myHo0z6\
fTwkQpuvvQgKz85lz+hv8lpx0aV5n7yNi0HbKoIXpr6Go6MRYlg8G9nVwvPSwmmH\
Ekosj5J68HhUkSExt+B1rqkOcSAClHNCGV3qcF3CGRpp+ZKi0AU1l0+jNbpGCTb/\
dMn0fTN2JSCDpc5UiHE4pRfJkBHwBbnSbEMwZQvMJTy8/7Y8PVcRdKBwYrYyObkD\
PuTmt9SxAoGBAPIPJbd6oWWHcm1+0Nd9B7ggID/L5bgT8MJIx38O5E7aHbi6h/4p\
3nFYq6vCctt4IgQSEQ9+w4umfWyyc0ZFDJRHNBh+rWm+1Dr4cSzgPgNJKtFWK4MT\
dgZuo7Sy0tfrcHl1Tcmcs1DvPyIeTlU50bblwIyhJ63rLcvjot4XU9o/AoGBANNo\
NM9JdI2F6LnvzWaY7sgHRSiBxVK5tq0Esn2JRiYYS6iqD5p6Q+NPQ+djhZjpbx2v\
gr3Fl9oGVZDXAHvICG8qCEsjjw9CEdHozSUKgOJ8b77dq1z6vbLkUQqfwxz9xIw0\
vvUrTjxDUn6a63thbaQQlk6lUo6DvMK5diB83OhlAoGAT6ksgHWbKA6rWd2iXUiI\
gdAQzO4Na8ckNMw+21HLUSf3IBRDLSIaUrMbzCryIcgnaa+IbdEKCauBG4Se60FO\
In5MZsNJzsM+lSmuFVa3Ed3CeaIiL8D31qYSuiUfvm99VHuGXhNz7+7MmC3hGast\
mUODWsSuik4bhD4mG3ER54ECgYBpUxJ0b4lE9QE0mbeAtRLJz21yN+0iQOo1GxD+\
IEU1t4cCOe/aK63k8tnGzzuBKlO0u3tspqP3+I3qwmM4SDmDgLJvFQEyPd148f46\
xRffPtXBhtjYUwftt3h0zZG8wOWKYMD+jpvExFm2P+KwLqMcSTNJbW5LGHETpqim\
mO7KTQKBgG2d7oFK5LtclxjdnJIlwxh21NPnpNox6uVAzIJQag61YegWN/QRsetk\
IYj1kZxEzavKdqLTux03T5ql9XTYJSztcveDuqi9xTTWPoPcTQExxGnTGjaOWDPx\
uyqkX5eknTzb3YpEYzi30XYvVuMpgI8myppRWU981qZIUFy0ijMd\
-----END RSA PRIVATE KEY-----\n"};
int status = WL_IDLE_STATUS;
int tick=0,msgCount=0,msgReceived = 0,tick1=0;
char payload[512];
char rcvdPayload[512];
uint8_t data1[4] = {41,42,43,44};
uint8_t data2[32];
void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    strncpy(rcvdPayload,payLoad,payloadLen);
    rcvdPayload[payloadLen] = 0;
    msgReceived = 1;
}



void setup()
{
   // initialization 
  esp32Cla.begin();

  // check the .ini file exist or not
  const char *filename = "/board_config.ini";
  IniFile ini(filename);
  if (!ini.open()) {
    Serial.print("Ini file ");
    Serial.print(filename);
    Serial.println(" does not exist");
    return;
  }
  Serial.println("Ini file exists");

  // check the .ini file valid or not
  if (!ini.validate(buffer, bufferLen)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid: ");
    return;
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Overlay_List_Info", "Overlay_Dir", buffer, bufferLen)) {
    Serial.print("section 'Overlay_List_Info' has an entry 'Overlay_Dir' with value ");
    Serial.println(buffer);
  }
  else {
    Serial.print("Could not read 'Overlay_List_Info' from section 'Overlay_Dir', error was ");
  }

  // Fetch a value from a key which is present
  if (ini.getValue("Board_Setup", "overlay_on_boot", buffer1, bufferLen)) {
    Serial.print("section 'Board_Setup' has an entry 'overlay_on_boot' with value ");
    Serial.println(buffer1);
  }
  else {
    Serial.print("Could not read 'Board_Setup' from section 'overlay_on_boot', error was ");
  }

  // Splicing characters
  strcat(buffer,buffer1);
  
  // XFPGA pin Initialize
  esp32Cla.xfpgaGPIOInit();

  // loading the bitstream
  esp32Cla.xlibsSstream(buffer);

  

  
  Serial.begin(115200);
  SeaTrans.begin();
  delay(2000);

    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_SSID);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        // wait 5 seconds for connection:
        delay(5000);
    }

    Serial.println("Connected to wifi");

    if(hornbill.connect(HOST_ADDRESS,CLIENT_ID,aws_root_ca_pem,certificate_pem_crt,private_pem_key)== 0)
    {
        Serial.println("Connected to AWS");
        delay(1000);

        if(0==hornbill.subscribe(TOPIC_NAME,mySubCallBackHandler))
        {
            Serial.println("Subscribe Successfully");
        }
        else
        {
            Serial.println("Subscribe Failed, Check the Thing Name and Certificates");
            while(1);
        }
    }
    else
    {
        Serial.println("AWS connection failed, Check the HOST Address");
        while(1);
    }

    delay(2000);
}

void loop()
{
    
    /*
    if(msgReceived == 1)
    {
        msgReceived = 0;
        Serial.print("Received Message:");
        Serial.println(rcvdPayload);
    }
    */
    int tag = 0;
    int count=0;
    float _data[3]={0,0,0};
    int flag[3]={0,0,0};
    while(1){
        SeaTrans.read(0, data2, 16);
        
        _data[0]=data2[3];
        _data[1]=data2[5];
        _data[2]=data2[7];
        
        for(int i=0;i<3;i++){
          if (_data[i]>50 && _data[i] < 150) {
            if (flag[i] == 0) {
              tag = 1;
              flag[i] = 1;
              break;
            }
            flag[i]= 1;
        }
          else if (_data[0] < 200 && _data[0]>150) {
            if (flag[i] == 1) {
              tag = 1;
              flag[i] = 0;
              break;
            }
            flag[i]= 0;
          }
        }
        
      if (tag == 1){
        count++;
        if(count%2==0){
          Serial.println(count/2);
          sprintf(payload,"{\"key\":\"%d\"}",count/2);//json:{"key":"value"} 字符而不是结尾:\"
          printf("%s\n",payload);
          if(hornbill.publish(TOPIC_NAME,payload) == 0)
          {        
              Serial.println("Publish successfully!");
              //Serial.println(payload);
          }
          else
          {
              Serial.println("Publish failed!");
          }
        }
      }
      tag=0;    
      delay(50);  
  }   
}
