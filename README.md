# Qt_Opencv
Program umożliwiający wykrywanie w filmie pieszych, którzy są zliczani i zaznaczani. 

Wykonany w frameworku Qt 5.12 w języku C++ z użyciem biblioteki OpenCV 3.6.4. Qt zostało wykorzystane przede wszystkim do GUI.

Zasada działania wykrywania i zliczania ludzi:
1. Przy pomocy BackgroundSubtractoraMOG2 z obrazu źródłowego wycinane jest tło (cienie są z obrazu wycinane).
2. Obraz jest binaryzowany - threshold.
3. Na obrazie dokonywana jest operacja morfologicznego otwarcia.
4. W obrazie znajdowane są kontury.
5. Każdy kontur jest klasyfikowany jako człowiek jeśli ma odpowiedni rozmiar - ustalone są w programie wartości minimalne i maksymalne powierzchni (metoda contourArea).
6. Otrzymani ludzie są porównywani z wynikami z poprzedniej klatki - program sprawdza czy to ta sama osoba, czy też pojawiła się nowa. Odpowiednio nowe osoby są zaktualizowane albo dodawane. Jeśli osoba która była w poprzedniej klatce nie ma swojego odpowiednika w obecnej jest ona usuwana.
7. Sprawdzenie czy osoba w obecnej klatce przekroczyła środek obrazka. Jeśli tak to odpowiednie liczniki są zaktualizowane.
8. Rysowanie na obrazku wynikowym punktów i prostokątów oznaczających ludzi. 

Program dobrze radzi sobie z wykrywaniem i zliczaniem ludzi w standardowych warunkach. Niestety w sytuacjach, w których odległości między ludźmi są bardzo małe albo gdy ludzie mają przy sobie wielkie gabarytowo przedmioty np. wózek,  detekcja może być niedokładna. 
