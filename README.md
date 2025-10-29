Example:
```
#Disable logging
logger:
  level: DEBUG
  baud_rate: 0

#External component
external_components:
  - source:
      type: git
      url: https://github.com/VAC91/HeatPumpLogger
    components: [HPLogger]


#Konfiguracja UART - integracja używana bezpośrednio w kodzie komponentu Heat Pump Logger
uart:
  tx_pin: GPIO01
  rx_pin: GPIO03
  baud_rate: 115200

#Konfiguracja komponentu Heat Pump Logger
#sensory aktualizują się co 5s, bo co 5s atmega wysyła stany czujników
sensor:
  - platform: heat_pump_logger
    id: uart_sensor
    gas_inlet:
      name: Gaz zasilanie
    gas_outlet:
      name: Gaz powrot
    water_inlet:
      name: Woda CO
    water_outlet:
      name: Woda powrot
    outdoor_temp:
      name: Przy pompie ciepla
    water_flow:
      name: Przeplyw wody
    cop:
      name: COP
    power_active:
      name: Moc czynna
    power_reactive:
      name: Moc bierna
    power_apparent:
      name: Moc pozorna
    fwd_act_energy:
      name: Energia calkowita
    voltage:
      name: Napiecie
    current:
      name: Prad
    freq:
      name: Czestotliwosc
    power_factor:
      name: Wspolczynnik mocy
    heating_power:
      name: Moc grzewcza
button:
  - platform: template
    name: "Reset licznika energii"
    on_press: 
      then:
        - uart.write: "RESET\n"

  - platform: template
    name: "Test"
    on_press: 
      then:
        - uart.write: "TEST\n"
```
