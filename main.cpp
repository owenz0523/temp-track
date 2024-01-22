#include <OneWire.h>
#include <DallasTemperature.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#define ONE_WIRE_BUS 4

char server[] = ADDRESS;
char user[] = USER;
char password[] = PASSWORD;
char database[] = DATABASE;


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
EthernetClient client;
MYSQL_Connection conn((Client *)&client);


void setup() {
    Serial.begin(9600);
    sensors.begin();

    if (conn.connect(server, 3306, user, password)) {
        Serial.println("Connected to MySQL server.");
    } else {
        Serial.println("Connection failed.");
    }
}

void loop() {
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0); // Celsius
    float tempK = tempC + 273.15; // Kelvin
    Serial.println(tempK);

    if (conn.connected()) {
        MySQL_Cursor cur = MySQL_Cursor(&conn);
        char query[128];
        sprintf(query, "INSERT INTO temperatures (temperature) VALUES (%f)", tempK);
        cur.execute(query);
    }

    delay(2000);    
}

void disconnect() {
    conn.close();
    Serial.println("Disconnected from MySQL server.");
}