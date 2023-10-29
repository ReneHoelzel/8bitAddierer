# 8bit Addierer

Das Projekt wurde in Zusammenarbeit mit dem Zuseum in Bautzen entwickelt und soll Schülern und interessierten die Rechenweise eine Computers anhand eines Bytes (=8bit) veranschaulichen. 

## Aufbau

Im unteren Bereich befindet sich ein Arduino Nano V3 als Steuereinheit sowie 8x Doppelpins an die je ein Taster angeschlossen werden kann. Ein Anschlus für den neunten Taster (Erläterung siehe Funktion) befindet sich unter dem Arduino.<br/>

## Funktion

Mit den 8 Tastern kann die dazugehörige 7-Segment Anzeige/rote LED jeweils ein- oder ausgeschalten werden. Je nachdem welche "Bits" eingeschalten sind wird oben in der 3-stelligen Anzeige der entsprechende Wert als Ganzzahl (Byte) ausgegeben.

Zum Beispiel: 0 0 0 0 1 0 0 1 entspricht dem Wert 9.

Mit dem neunten Taster kann das Ergebnis "ausgeblendet" werden (es wird der Wert 999 angezeigt). Dies kann verwendet werden, um einen Wert einzustellen und die Schüler herausfinden zu lassen welcher Byte-Wert sich dahinter verbirgt.

<br/>

<span style="color:red;">Die Verwendung erfolgt auf eigene Gefahr.<span/>

Zum Arduino Programm geht es [hier](https://github.com/ReneHoelzel/8bitAddierer/tree/main/Program).

Es gibt aktuell 2 Board Revisionen:

- [Revision A](8BitZahlAnzeige_RevA/README.MD)

- [Revision B](8BitZahlAnzeige_RevB/README.MD)