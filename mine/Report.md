# Pejskova prostata
Report

## Řešení
Řešení je prostá Dijkstra s přidáním si paměti jak jsem se do vrchodu dostal.
Navíc se musí vzdálenost převést na minuty, ale to snad není takový problém.

## Čas a velikost problémů
Asi každého řešitele Dijkstra napadne skoro okamžitě,
přidanou hodntou je právě maska a paměť.
Proto by problémy měly být škálovany podle `n log(n)`,
tedy až v miliónech (ale rozumně k velikosti vstupních souborů).

## Testy
### 01
Základní testy s prozrazením (záludných) edge casů:
- kontroluje neorientovanost
- kontrala načtení jako multigrafu
- kontrola využití jedné hrany tam a zpět (park projit tam a zpět)
- cesta pouze při využití 2 tranvají zasebou (neexistující)

### 02
Testuje se:
- neexistující cesta
- začátek i konec v stejné stanici
- prázdný graf
- smyčky

### 03
Prostě velká data

## Autor
Pro předmět EP2 na FIT CVUT vytvořil Petr Laštovička v roce 2023.
