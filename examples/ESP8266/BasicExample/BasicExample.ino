/*********************************************************************
 *  Read Instructables Author statistics from the Instructables API  *
 *                                                                   *
 *  By Brian Lough                                                   *
 *  https://www.instructables.com/member/witnessmenow/instructables/ *
 *********************************************************************/

 // Uncomment to enable debug logging from the library
 // #define INSTRUCTABLE_DEBUG true

#include <InstructablesApi.h>

// ----------------------------
// Standard Libraries - Already Installed if you have ESP8266 set up
// ----------------------------

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <ArduinoJson.h>
// Required by the InstructablesApi Library for parsing the response
// Available on the library manager (Search for "arduino json")
// https://github.com/bblanchon/ArduinoJson

//------- Replace the following! ------
char ssid[] = "xxx";       // your network SSID (name)
char password[] = "yyyy";  // your network key
#define SCREEN_NAME "witnessmenow" //Replace with your instructables user name
#define INSTRUCABLE_ID "Zelda-Treasure-Chest-With-Lights-Sound" // Makes up the end of the URL for the instructable
// e.g. : https://www.instructables.com/id/Zelda-Treasure-Chest-With-Lights-Sound/

WiFiClient client;
InstructablesApi api(client);

unsigned long delayBetweenRequests = 5 * 60000; //time between api requests (5mins)
unsigned long whenDueToCheck;

void setup() {

  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


}

void loop() {

  if (millis() > whenDueToCheck)  {
    instructablesAuthorStats stats;
    stats = api.getAuthorStats(SCREEN_NAME);
    if(stats.error.equals(""))
    {
      Serial.println("---------Author Stats---------");
      Serial.print("Views: ");
      Serial.println(stats.views);
      Serial.print("Followers: ");
      Serial.println(stats.followersCount);
      Serial.print("Instructables Count: ");
      Serial.println(stats.instructablesCount);
      Serial.print("Featured Count: ");
      Serial.println(stats.featuredCount);

      // The following variables are available from the response (e.g. stats.collaborationsCount)
      /*
      long commentCount;
      long views;
      long featuredCount;
      long favoritesCount;
      long instructablesCount;
      long publishedCollectionsCount;
      long lessonCount;
      long courseCount;
      long topicsCount;
      long questionsCount;
      long answersCount;
      long bestAnswersCount;
      long followersCount;
      long subscriptionsCount;
      long collaborationsCount;
      String error // This will be blank if it gets in here
      */
      Serial.println("------------------------");
    } else {
      Serial.print("Error getting Author Stats: ");
      Serial.println(stats.error);
    }

    instructableStats postStats;
    postStats = api.getInstructableStats(INSTRUCABLE_ID);
    if(postStats.error.equals(""))
    {
      Serial.println("---------Post Stats---------");
      Serial.print("Views: ");
      Serial.println(postStats.views);
      Serial.print("Favorites: ");
      Serial.println(postStats.favorites);
      Serial.print("Comments: ");
      Serial.println(postStats.comments);
      Serial.println("------------------------");
    } else {
      Serial.print("Error getting Post Stats: ");
      Serial.println(postStats.error);
    }

    whenDueToCheck = millis() + delayBetweenRequests;
  }
}
