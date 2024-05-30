# flight-helper
A paragliding helper giving me an offline map with the flight obstacles based on the M5Core2 (M5Unified).
It is a learning project in the matter of language, tooling and platform (ESP32).

Initial Idea came from: https://github.com/akchan/cycle_navi which seemed easy enought to adapt.

## SD-Card Access and LCD Access ##
There is a limitation with the M5Stack Core2 that the LCD and SD-Card can not be accessed in parallel.
Therefore there might be some issues while drawing or accessing SD-Card.
Accessing SD-Card should have a higher priority (portMAX_DELAY block everything else). The Drawingtasks should load regularly und refresh therefore automatically.

## In-Flight example ##
[![examples](http://img.youtube.com/vi/pJnNsUJWWy8/0.jpg)](http://www.youtube.com/watch?v=pJnNsUJWWy8 "In Flight")</br>
https://github.com/oli4wolf/flight-helper/tree/main/media/2024-03-24.mp4</br>
Still not correct is the Vario.</br>
Next Features: Zoom, Scrolling, Hike Paths?

## Inspiration ##
https://github.com/Oganisyan/Variometer-LilyGoEPD47
https://github.com/akchan/cycle_navi
    