# Parking-Barrier-Control-System

## Projekat iz predmeta Mikroprocesorski upravljački uređaji
Zadatak projekta:
Implementirati program za rad parking rampe. 
Rampa se sastoji od motora koji moze da se okrece u dva smera (podizanje i spustanje rampe) koji se kontrolise pomocu dva digitalna izlaza. 
Na samoj rampi se nalaze dva granicna prekidaca koji govore o krajnjim polozajima rampe (rampa spustena, rampa podignuta). 
Sistem takođe poseduje i dva potpatosna prekidaca koja govore o tome da li automobil hoće da uđe na parking ili izađe. 
Parking može da primi maksimalno 15 automobila. 

Algoritam rada je sledeći: kada automobil priđe rampi, aktivan je potpatosni prekidak na ulazu, rampa se podiže. 
Kada rampa stigne do kraja, ostaje u tom položaju 5 sec. da bi kola mogla nesmetano da prođu ispod rampe. 
Posle 5sec rampa se spusta i ostaje spuštena dok se ne pojave vozila na ulazu ili izlazu. 
Stanje o zauzetosti parking mesta prikazati na displeju i slati serijskom komunikacijom na PC. 
