# gEngine

2D spiel engine system aufgebaut auf **gRenderer**, einem custom 2D renderer mit möglicher Erweiterung auf 3D kapazitäten,
aufgebaut mit OpenGL.

### Bisherige Funktionen:


## Benutzung
Um die Engine zu benutzen, muss zuerst ein **gApplication** (File: "gEngine.h") Objekt, oder eine erbende Klasse erstellt werden.
Eine gApplication dient als Level Manager, die in eine Engine Instanz eingebunden wird.
Um die Engine mit einer Application zu initiieren und zu starten, hat der Benutzer zwei Optionen.

### 1. Nutzerdefinierte main Funktion
Der Benutzer kann eine eigene main funktion schreiben und nach folgendem Ablauf eine gEngine instanz starten:
*gApplication game* Erstellen einer Application
*gEngine *engine = new gEngine(game)* Erstellen der Engine Instanz
*engine->start()* Starten der Engine; initiieren aller relevanten Spielobjekte innerhalb des aktiven Levels
*engine->run()* Starten des Gameplay loops. Ein Fenster wird gestartet und Level's **tick** funktion wird wiederholt aufgerufen, und objekte gezeichnet
*engine->end()* Engine Instanz wird beendet und alle Objekte innerhalb sauber gelöscht
### 2. gEngineMain.h einbinden
Anstatt einer eigenen main Funktion besteht auch die Möglichkeit, lediglich nutzerdefiniertes Verhalten zu schreiben, und dann mithilfe von
*G_ENTRY_POINT* (File: "gEngineMain.h") wieder eine Application, die aufgerufen werden soll, zu spezifizieren.
Durch *#define G_ENGINE_MAIN* wird das Verhalten aktiviert, und durch einbindung der Datei "gEngineMain.h" aktiviert.
Eine gEngine Instanz wird initiiert, gestarted und beendet.

Die Engine kümmert sich um den Rest von alleine.
Funktionen innerhalb von gApplication beinhalten:
start() - wird einmal beim Start aufgerufen
tick(float delta) - wird jeden tick(frame) aufgerufen. delta entspricht hierbei der Zeit, in der ein Frame in bearbeitung war
end() - wird am ende des Lebenszyklus der Engine Instanz aufgerufen.
per default tun diese Funktionen nichts.

Die aktive Szene(Level) wird durch den pointer activeScene festgelegt. 
Dieser ist eine gScene (File: "gScene.h"), welche aus einem vector aus std::unique_ptr<gEntity> besteht