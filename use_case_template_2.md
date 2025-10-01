# Uitwerking Use Case: Gesprekken uitlezen met laptop (onderzoeker)

## 1. Doel van de use case

Het doel van deze use case is om het proces te beschrijven waarmee een onderzoeker de opnames van gesprekken van de recorder (bijvoorbeeld Tascam) kan uitlezen op zijn laptop, door de recorder via USB te verbinden. De onderzoeker kan vervolgens een overzicht krijgen van alle gesprekken, met tijd, datum en patiëntcode, en gesprekken openen in Audacity voor analyse van o.a. geluidsintensiteit.

---

## 2. Actoren

- **Onderzoeker:** Verbindt recorder met laptop, leest gesprekken uit, bekijkt overzichten, opent gesprekken in Audacity.
- **Systeem (laptop + uitleesprogramma):** Herkent recorder, toont overzicht, koppelt metadata, start Audacity.

---

## 3. Precondities

- Recorder bevat opgenomen gesprekken.
- Laptop beschikt over het uitleesprogramma en Audacity.
- USB-verbinding tussen recorder en laptop is mogelijk.

---

## 4. Trigger

- Onderzoeker sluit de recorder via USB aan op de laptop en start het uitleesprogramma.

---

## 5. Stroom (Hoofdsucces scenario)

1. **Recorder aansluiten:**  
   De onderzoeker verbindt de recorder via USB met de laptop.
2. **Uitleesprogramma starten:**  
   De onderzoeker start het uitleesprogramma op de laptop.
3. **Gesprekken laden:**  
   Het programma herkent de recorder en toont een overzicht van alle beschikbare gesprekken (bestanden).
4. **Overzicht tonen:**  
   Het overzicht bevat:
   - Tijd en datum van elk gesprek
   - Patiëntcode (indien beschikbaar)
5. **Gesprek selecteren:**  
   De onderzoeker selecteert een gesprek uit de lijst.
6. **Gesprek openen in Audacity:**  
   Het geselecteerde gesprek wordt geopend in Audacity voor verdere analyse/beluistering.
7. **Geluidsintensiteit bekijken:**  
   In Audacity kan de onderzoeker desgewenst de geluidsintensiteit visualiseren of analyseren.

---

## 6. Alternatieve stromen

- **Recorder niet herkend:**  
  Het programma geeft een foutmelding en vraagt om te controleren of de recorder correct is aangesloten.
- **Geen gesprekken gevonden:**  
  Het programma meldt dat er geen bestanden beschikbaar zijn.

---

## 7. Systeemgedrag

- **Automatische herkenning:**  
  Zodra de recorder via USB wordt aangesloten, zoekt het programma automatisch naar gespreksbestanden.
- **Metadata extractie:**  
  Het programma leest tijd/datum en patiëntcode uit de bestandsmetadata of bijbehorend logbestand.
- **Integratie met Audacity:**  
  Bij selectie van een bestand wordt Audacity automatisch gestart met het juiste gesprek.

---

## 8. Postcondities

- Onderzoeker heeft toegang tot alle opgenomen gesprekken en bijbehorende metadata op laptop.
- Gesprekken kunnen in Audacity worden beluisterd en geanalyseerd.

---

## 9. Use Case diagram toelichting

Zie afbeelding (Image 2).
- Actor: onderzoeker.
- Hoofdusecase: overzicht van alle gesprekken op de Tascam.
- Includes: overzicht tijd en datum, overzicht patiëntcode.
- Onderzoeker kan een gesprek openen in Audacity (waar mogelijk geluidsintensiteit wordt weergegeven).

---

## 10. Notities

- Overzicht is alleen beschikbaar als de recorder via USB is verbonden.
- Patiëntcode wordt alleen getoond indien aanwezig in het bestand/log.
- Audacity dient op de laptop geïnstalleerd te zijn voor volledige functionaliteit.
