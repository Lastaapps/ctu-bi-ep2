# Pejskova prostata

Paní Helena a její pejsek Buřtík se vydaly na druhou stranu Kocourkova, města tramvají, za tetou Evženií. Tetička je stále fit a ráda chodí parky, ale cesta je dlouhá, proto se nebrání využití hromadné dopravy. Problém ale je, že pejsek je ale již stár a dlouho nevydrží bez navštívení patníku. Proto se po každé jízdě tramvají musí vyvenčit v parku vedle. Když už teta v parku je, tak rovnou projde na druhou stranu, tak neváhá a cestu parkem využije. Když dorazí na konec parku, buďto hned naváže na další park, kde je Buřtík vždy v dosahu travičky, nebo nastoupí do další tramvaje.

Jsme v obrozeneckém městě, tramvaje tedy jezdí průměrnou rychlostí 30 km/h, zatímco tetička s Buřtíkem parkem prochází parky průměrnou rychlostí 4 km/h. Mají to štěstí, že kocourkovští investují do dopravy nemalé částky, a proto jim jezdí spoje doslova každou chvíli, nemusejí tedy na spoje čekat. Nevýhodou ale je, že tramvaje jezdí pouze mezi 2 zastávkami (cestou nestaví) a na konci se hned obrací zpět - je nutné na každé zastávce přestupovat. Každá zastávka má své unikátní číslo, východy z parků jsou strategicky u těchto zastávek.

Za jak dlouho se Tetička s Buřtíkem může co nejrychleji ze svého bytu (zastávky u něj) dostat na kafíčko u tety Evženie? Co jí nahlásí předem do telefonu?

## Vstup
Na vstupu přijde řádek s číslem `N` s počtem případů k vyřešení.
Pro každý vstup následuje řádek s číslem `Z` - počet všech zastávek města - a čísly zastávek `H` a `E` - počáteční a koncová.
poté další s číslem `P` s počtem parků, poté tento počet řádků s 2 čísly zastávek spojených parkem a 1 číslo s vzdáleností cesty parkem v kilometrech.
Poté následuje číslo `T` s počtem tramvajových linek a linky ve stejném formátu jako výše.
Čísla zastávek jsou v intervalu `[0, Z)`.

## Výstup
Na výstupu bude udán počet minut cesty, případně bědování tety, že si nepopovídá, viz. ukázka.

## Ukázka
### Vstup
```
3

6 0 5
4
0 1 1
2 3 1
2 5 2
3 4 1
4
2 1 2
4 5 10
2 4 1
0 2 40

4 3 2
4
3 0 4
1 0 1
0 2 4
0 3 3
2
0 3 5
2 0 8

4 0 3
1
0 1 1
2
1 2 2
2 3 3
```

### Výstup
```
Za 49 min. tam budu.
Za 56 min. tam budu.
O beda, o beda, co ja budu ted delat?
```
