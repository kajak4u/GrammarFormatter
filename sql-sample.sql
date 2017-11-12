/* Zad1
Dla każdego pracownika odpowiedzialnego za jakiś przedmiot podaj liczbę przedmiotów z których prowadzi zajęcia (tabela ROZKLADY).
*/

select pra.NAZWISKO, count(distinct r.NR_PRZEDM) as 'Liczba przedmiotow'
	from pracownicy pra join przedmioty prz on pra.NR_PRAC=prz.NR_ODP_PRAC
								join rozklady r on pra.NR_PRAC=r.NR_PRAC
	group by pra.NAZWISKO, pra.NR_PRAC;
	
/* Zad2
Podaj nazwiska i średnią ocen z przedmiotu ALGEBRA tych studentów, którzy z tego przedmiotu mają więcej niż jedną ocenę.
*/

select s.NAZWISKO, avg(o.OCENA)
	from przedmioty prz	join oceny o on o.NR_PRZEDM=prz.NR_PRZEDM
								join studenci s on s.NR_STUD=o.NR_STUD
	where prz.NAZWA_PRZEDM='Algebra'
	group by s.NAZWISKO, s.NR_STUD
	having count(o.OCENA)>1;
	
/* Zad3
Podaj przedmioty realizowane przez największą liczbę pracowników.
*/

select prz.NAZWA_PRZEDM, prz.NR_PRZEDM, count(distinct pra.NR_PRAC)
	from pracownicy pra join przedmioty prz on pra.NR_PRAC=prz.NR_ODP_PRAC
								join rozklady r on pra.NR_PRAC=r.NR_PRAC
	group by prz.NR_PRZEDM, prz.NAZWA_PRZEDM
	having count(distinct pra.NR_PRAC)=
		(
			select max(cnt)
				from
					(
						select count(distinct pra.NR_PRAC) as cnt
							from pracownicy pra	join przedmioty prz on pra.NR_PRAC=prz.NR_ODP_PRAC
														join rozklady r on pra.NR_PRAC=r.NR_PRAC
														
							group by prz.NR_PRZEDM, prz.NAZWA_PRZEDM 
					) temp
		);

/* Zad4
Podaj numery i nazwiska pracowników, którzy w temacie DYSK otrzymali wypłatę mniejszą od co najmniej jednej
z wypłat pracownika o nazwisku GRZYBEK w dowolnym temacie.
*/

select p.NR_PRAC, p.NAZWISKO, w.KWOTA
	from tematy t	join wyplaty w on t.NR_TEM=w.NR_TEM
						join pracownicy p on w.NR_PRAC=p.NR_PRAC
	where t.TEMAT='DYSK'
		and w.KWOTA <
		(
			select max(w.kwota)
				from wyplaty w join pracownicy p on w.NR_PRAC=p.NR_PRAC
				where p.NAZWISKO='GRZYBEK'
		);

/* Zad5
Podaj numery, nazwiska i daty urodzenia mężczyzn zatrudnionych w zespole OPROGRAMOWANIE,
którzy są młodsi od każdego pracownika zespołu BUDOWA.
*/

select p.NR_PRAC, p.NAZWISKO, p.DATA_UR
	from pracownicy p join zespoly z on p.NR_ZESP=z.NR_ZESP
	where z.NAZWA_ZESP='OPROGRAMOWANIE'
		and p.DATA_UR > 
			(
				select max(pp.DATA_UR)
					from pracownicy pp join zespoly zz on pp.NR_ZESP=zz.NR_ZESP
					where zz.NAZWA_ZESP='BUDOWA'
			);

/* Zad6
Podaj nazwiska najstarszych studentów na poszczególnych kierunkach.
*/

select s.NAZWISKO, s.DATA_UR, s.NR_KIER
	from studenci s
	where s.DATA_UR =
		(
			select max(ss.DATA_UR)
				from studenci ss join kierunki k on ss.NR_KIER=k.NR_KIER
			where s.NR_KIER=ss.NR_KIER
			group by ss.NR_KIER
		);

/* Zad7
Podaj nazwy zespołów, w których nie pracuje żadna kobieta.
*/

select NAZWA_ZESP
	from zespoly
	where NR_ZESP NOT IN
	(
		select z.NR_ZESP
		from zespoly z join pracownicy p on z.NR_ZESP=p.NR_ZESP
		where p.PLEC='K'
	);

/* Zad8
Zestawienie zespołów z liczbą pracowników o postaci:
Nrz	nazwa_zespołu	liczebność
W zestawieniu powinny znaleźć się również zespoły z liczebnością zerową.
*/

select z.NR_ZESP as nrz, z.NAZWA_ZESP as nazwa_zespolu, count(p.NR_ZESP) as liczebność
	from zespoly z left join pracownicy p on z.NR_ZESP=p.NR_ZESP
	group by z.NR_ZESP, z.NAZWA_ZESP;

/* Zad9
Utworzyć perspektywę zawierającą wyszczególnione informacje, oraz określić, czy można aktualizować dane
w tej perspektywie. Jeżeli nie można - podać wszystkie tego przyczyny.
Po pomyślnym wykonaniu zadania należy usunąć utworzoną przez siebie perspektywę.

Zestawienie pracowników mających wypłaty w poszczególnych tematach w postaci:
nr_tem	nazwa_tematu	nr_prac	nazwisko
Rekordy nie powinny powtarzać się nawet w sytuacji, gdy pracownik uzyskał wielokrotnie dochód w tym samym temacie.
*/

create view perspektywa1 as 
	select distinct t.NR_TEM as nr_tem, t.TEMAT as nazwa_tematu, p.NR_PRAC as nr_prac, p.NAZWISKO as nazwisko
		from tematy t	join wyplaty w on t.NR_TEM=w.NR_TEM
							join pracownicy p on p.NR_PRAC=w.NR_PRAC;

/*update perspektywa1 set nazwa_tematu='ABBA SRODOWISKO' where nr_tem=1;*/

/***
	*	Nie można aktualizować danych w tej perspektywie. W tym przypadku jest to spowodowane słowem kluczowym DISTINCT.
	*	Jeżeli widok zawiera funkcje agregujące, grupowanie itd, wówczas widok taki jest tylko do odczytu.
****/

drop view perspektywa1;

/* Zad10
Zrealizować zestawienie średnich, minimalnych i maksymalnych zarobków pracowników w poszczególnych zespołach.
Uwaga!
Zestawienie zrealizować w dwóch krokach:
- w pierwszym utworzyć perspektywę (widok) o postaci:
  nrz nazwa_zesp nr_prac nazwisko suma_zarobków_prac
  uwzględnić również fakt, że pracownik mógł nie dostać żadnej wypłaty,
- utworzoną perspektywę wykorzystać w zadaniu.
*/

create view perspektywa2 as
	select z.NR_ZESP as nrz, z.nazwa_zesp, p.nr_prac, p.nazwisko, sum(w.kwota) as suma_zarobkow_prac
		from zespoly z join pracownicy p on z.NR_ZESP=p.NR_ZESP
							left join wyplaty w on p.NR_PRAC=w.NR_PRAC
	group by z.NR_ZESP, z.NAZWA_ZESP, p.NR_PRAC, p.NAZWISKO;
	
	select nrz, nazwa_zesp, min(suma_zarobkow_prac) as min_zarobki, avg(suma_zarobkow_prac) as avg_zarobki, max(suma_zarobkow_prac) as max_zarobki
		from perspektywa2
		group by nrz, nazwa_zesp;
drop view perspektywa2;

/* Zad11
Podaj nazwiska pracowników, którzy zostali przydzieleni do tematu, lecz nie otrzymali w tym temacie ani jednej wypłaty.
*/

select distinct pra.NR_PRAC, pra.nazwisko
	from pracownicy pra join przydzialy prz on prz.NR_PRAC=pra.NR_PRAC
								left join wyplaty w on pra.NR_PRAC=w.NR_PRAC and prz.NR_TEM=w.NR_TEM
	where kwota is null;

/* Zad12
Sprawdzić poprawność przydziału kierowników do tematu (jest to niezależnie określone za pomocą klucza obcego
nr_prac_kt w tabeli TEMATY oraz przez zapis w tabeli PRZYDZIAŁY – należy sprawdzić czy oba te zapisy sobie odpowiadają).
*/

select t.NR_TEM as 'nr błędnego tematu', t.TEMAT as 'błędny temat'
	from tematy t
	where t.NR_PRAC_KT NOT IN
	(
		select p.NR_PRAC
			from przydzialy p join funkcje f on p.KOD_FUNKCJI = f.KOD_FUNKCJI
			where p.NR_TEM=t.NR_TEM AND f.NAZWA_FUNKCJI='Kierownik'
	);


/* Zad13
Podaj numery i nazwiska pracowników nie biorących udziału w żadnym z tematów, w którym pracuje pracownik NIEZALEZNY.
*/

select pr.NR_PRAC, pr.NAZWISKO, count(t.NR_TEM)
	from tematy t join przydzialy p on t.NR_TEM=p.NR_TEM
						right join pracownicy pr on pr.NR_PRAC=p.NR_PRAC
	/* zliczamy ilość tematów, w których nie pracuje NIEZALEZNY */
	where t.NR_TEM NOT IN
	(
		select prz.NR_TEM
			from przydzialy prz join pracownicy pra on prz.NR_PRAC=pra.NR_PRAC
		where pra.NAZWISKO='NIEZALEZNY'
	) OR t.NR_TEM is null
	group by pr.NR_PRAC, pr.NAZWISKO
	/* a następnie porównujemy z ilością wszystkich tematów */
	HAVING count(t.NR_TEM)=
		(
			select count(tt.NR_TEM)
				from tematy tt join przydzialy pp on tt.NR_TEM=pp.NR_TEM
				where pp.NR_PRAC=pr.NR_PRAC
				
		);

/* Zad14
Podaj nazwiska pracowników którzy realizowali wszystkie tematy, których kierownikiem jest JASKOLA.
*/

select pra.nazwisko
	from pracownicy pra join przydzialy prz on pra.NR_PRAC=prz.NR_PRAC
	/* zlicz pracownikowi liczbę tematów, w których brał udział a którymi kierował JASKOLA */
	where prz.NR_TEM in
	(
		select NR_TEM
			from tematy t join pracownicy pra2 on t.NR_PRAC_KT=pra2.NR_PRAC
			where pra2.NAZWISKO='JASKOLA'
	)
	group by pra.NR_PRAC, pra.NAZWISKO
	/* sprawdź, czy ta liczba równa jest liczbie wszystkich tematów, którymi kieruje JASKOLA */
	having count(prz.NR_TEM)=
	(
		select count(t.NR_TEM)
			from tematy t join pracownicy pra2 on t.NR_PRAC_KT=pra2.NR_PRAC
			where pra2.NAZWISKO='JASKOLA'
	)

