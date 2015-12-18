===LINKJES===

Gegeneerde doxygen kan altijd worden terug gevonden via deze link:

http://koen-github.github.io/TeamHayFaseGame-Delinquent/



===ALGEMEEN===
De rapidjson map in de include folder moet worden gekopieerd naar C:\SFML
De bestanden van Visual Studio .sdf en .openssdf moeten worden genegeerd door github 

De volgende bestanden zijn nodig voor het goed laten fucntioneren van het visual studio project:

- DeDelinquent.sln
- DeDelinquent.vcxproj
- DeDelinquent.vcxproj.filters

En verder alle .cpp, .hpp, .h bestanden.

===Regels===

-Header bestanden moeten op .hpp eindigen
-Gebruik overal pragma once zodat header niet per ongeluk meerdere keren worden geinclude.
#pragma once

-Geen binaries naar git committen

-Remco neemt altijd zijn nieuwe laptop mee
-Klassendiagram bijwerken aan de hand van je code, maar dan omgedraait.

-Code in het engels
-Git commits ook in het engels.
-Klassen moeten met een Hoofdletter beginnen.
-Documentaar in het engels

-Volg verder zoveel mogelijk de regeltjes van Wouter die op sharepoint zijn terug te vinden.

===GEBRUIK DOXYGEN===

Gebruik JAVA-DOC style voor het documenteren van de klassen en functies:
Bijvoorbeeld:

    /**
       * a normal member taking two arguments and returning an integer value.
       * @param a an integer argument.
       * @param s a constant character pointer.
       * @see Test()
       * @see ~Test()
       * @see testMeToo()
       * @see publicVar()
       * @return The test results
       */
       int testMe(int a,const char *s);
	  
-Doxygen wordt soms gegenereerd en vervolgens op de branch gh-pages gezet. 	  
