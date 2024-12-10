**David Vlad-Mihail**
**311CA**

## Segregated free list, tema 1

### Descriere:

	Am implementat o structura tip lista dublu inlantuita generica pentru a o folosi in structura segregated free list. 
Aceasta structura retine adresele memoriei nealocate din heap intr-un vector de liste dublu inlantuite unde fiecare nod retine 
o variabila de tip int care este "numele" adresei. Dimensiunea blocului de la adresa respectiva este retinuta in indexul de la 
pozitia unde se afla lista de adrese. De asemenea, mai avem lista in care retinem blocurile alocate. Diferenta este ca nodurile 
acestei liste retin structura mem_block, care contine un sir de ccaractere pentru a scrie date, adresa de start a blocului si 
marimea sa. In plus, pentru genericitate, fiecare lista retine o functie de comparare, corespunzatoare tipului de date pe care 
nodurile acesteia le va retine. Eliminand si inserand blocuri atat din sfl cat si din lista de blocuri alocate am observat 
necesitatea unei functii care, de fiecare data cand inserez date, le insereaza sortate. Aici este utila functia de comparare.

### Comentarii asupra temei:

	Consider ca ar exista loc de imbunatatire in aceasta implementare la "eleganta" functiilor din cardul listei dublu 
inlantuite.
	Ce mi s-a parut cel mai interesant a fost conceptul de segregated free list, despre care nu stiam. De asemenea, acesta 
mi s-a parut si cel mai greu concept de inteles, in special cum functioneaza fragmentarile. Odata ce am reusit sa concep o 
strategie de implementare a structurilor, a fost mult mai usor sa scriu codul.
	Din realizarea acestei teme mi-am consolidat cunostintele legate de folosirea listelor, a structurilor de date in sine
si in special a necesitatii genericitatii acestora. De altfel, tematica de "joaca de-a mallocul" m-a ajutat sa inteleg mai 
bine cum functioneaza alocarea si eliberarea memoriei "in spate" si cum gestioneaza sistemul de operare adresele.

