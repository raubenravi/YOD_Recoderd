# Uitwerking Use Case: Gesprek Opnemen (Sessie)

## 1. Doel van de use case

Het doel van deze use case is om het proces te beschrijven waarmee een onderzoeker een sessie (opname van een gesprek) start, pauzeert, hervat en beëindigt, waarbij ook de spraak-stilte-ratio wordt weergegeven. De sessie is gekoppeld aan één afspraak en wordt onder één uniek sessie-ID opgeslagen.

---

## 2. Actors

- **Onderzoeker:** Start, pauzeert, hervat en beëindigt de opname, en scant de patiëntenpas.
- **Spreker:** Kan de opname pauzeren via de blauwe knop.
- **Systeem:** Registreert, bewaart en verwerkt audio, toont status en spraak-stilte-ratio.

---

## 3. Precondities

- Er is een afspraak aanwezig tussen onderzoeker en spreker.
- De apparatuur is aangesloten en functioneert.
- De patiëntenpas is aanwezig.

---

## 4. Trigger

- Onderzoeker drukt op de groene knop óf scant de patiëntenpas.

---

## 5. Stroom (Hoofdsucces scenario)

1. **De onderzoeker scant de patiëntenpas** via het bedieningspaneel.
2. Het systeem koppelt de sessie aan de afspraak en genereert een uniek sessie-ID.
3. De onderzoeker drukt op de groene knop om de sessie te starten.  
   - Het systeem toont "idle" op het scherm; de opname staat standaard op pauze.
4. **Opname starten:** De onderzoeker drukt nogmaals op de groene knop.  
   - Het systeem geeft een pieptoon en schakelt de indicatorlampen aan.
   - De opname start; de spraak-stilte-ratio verschijnt op het scherm.
5. **Opname pauzeren:**  
   - De onderzoeker kan tijdens het opnemen op de gele knop drukken om te pauzeren.
   - De spreker kan op de blauwe knop drukken om te pauzeren.
   - Systeem toont "idle", opname stopt tijdelijk.
6. **Opname hervatten:**  
   - Alleen de onderzoeker kan op de groene knop drukken om te hervatten.
   - Pieptoon klinkt, indicatorlampen gaan aan, opname gaat verder.
7. **Opname beëindigen:**  
   - Onderzoeker drukt op de rode knop om de sessie te stoppen.
   - De opname wordt veilig opgeslagen onder het sessie-ID.

---

## 6. Alternatieve stromen

- **Spreker pauzeert opname:**  
  Spreker drukt op de blauwe knop, opname wordt gepauzeerd. Alleen de onderzoeker kan hervatten.
- **Onderzoeker pauzeert opname:**  
  Onderzoeker drukt op de gele knop, opname wordt gepauzeerd. Alleen de onderzoeker kan hervatten.

---

## 7. Systeemgedrag

- **Spraak-stilte-ratio:**  
  Elke 250 ms wordt in het frequentiegebied 300-3400 Hz gemeten of het signaal >5dB is.  
  - 2x achter elkaar signaal = spraak
  - Dit wordt 240 keer gemeten en als percentage weergegeven.
- **Audiovisuele weergave:**  
  Status ("idle", "record...") wordt getoond; lampjes en pieptoon geven status aan.
- **Beveiliging:**  
  Opnames kunnen alleen door de onderzoeker worden hervat na pauze.

---

## 8. Postcondities

- De sessie is opgeslagen met alle relevante metadata (sessie-ID, tijdstippen, spraak-stilte-ratio).
- De opname kan later worden teruggehaald of geanalyseerd.

---

## 9. Use Case diagram toelichting

Zie afbeelding (Image 1).  
- Actoren: onderzoeker, spreker.
- Hoofdusecase: gesprek opnemen.
- Extends: opnemen beëindigen, opnemen pauzeren.
- Includes: audiovisuele weergave, spraak/stilte ratio weergeven.
- Onderzoeker kan de patiëntpas scannen.

---

## 10. Notities

- Alleen de onderzoeker kan een sessie hervatten of stoppen.
- Spreker kan alleen pauzeren.
- De sessie blijft actief tot deze expliciet wordt beëindigd.
