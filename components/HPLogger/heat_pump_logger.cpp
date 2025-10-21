#include "esphome/core/log.h"
#include "heat_pump_logger.h"

namespace esphome {
namespace heat_pump_logger {

static const char *TAG = "heat_pump_logger.component";

void HeatPumpLogger::setup() {
    #define MAX_BUFFER_SIZE 256
}

void HeatPumpLogger::loop() {
    yield();
    
    if (this->available()) {
        uint8_t data;

        if (this->read_byte(&data)) {
            if (data == '\r' || data == '\n') {
                //34.75;33.61;33.33;32.31;3.95;661.30;2698.84;242.20;2.82;50.00;0.97;16.07;1.71; wysłane dwukrotnie

                if (buffer_.length() > 0) {
                    // Przetwarzaj całą wiadomość
                    ESP_LOGD("UART", "Odebrano wiadomość: %s", buffer_.c_str());

                    // Rozdziel dane na części i zapisz do tablicy
                    std::vector<float> values = split_buffer_to_floats(buffer_, ';');

                    //Sprawdzanie czy dane się pokrywają
                    dataError = 0;
                    for (int i = 0; i <= 12; i++) {
                        if (values[i] != values[i + 13]) dataError++;
                    }

                    if (!dataError) {
                        float apparent = (values[7] * values[8]); //moc pozorna
                        float reactive = 0; //moc bierna

                        if (apparent > values[5]) reactive=sqrt(apparent*apparent-values[5]*values[5]);
                        else reactive=0;

                        if (values[12] < 0) values[12] = 0; //minimum cop 0, max 10
                        else if (values[12] > 10) values[12] = 10;
                        float heating = (values[5] * values[12]); // moc grzewcza

                        // Publikuj wartości do sensorów
                        if (gas_inlet_ != nullptr) gas_inlet_->publish_state(values[0]);
                        if (gas_outlet_ != nullptr) gas_outlet_->publish_state(values[1]);
                        if (water_inlet_ != nullptr) water_inlet_->publish_state(values[2]);
                        if (water_outlet_ != nullptr) water_outlet_->publish_state(values[3]);
                        if (outdoor_temp_ != nullptr) outdoor_temp_->publish_state(values[4]);
                        if (water_flow_ != nullptr) water_flow_->publish_state(values[11]);
                        if (cop_ != nullptr) cop_->publish_state(values[12]);
                        if (power_active_ != nullptr) power_active_->publish_state(values[5]);
                        if (power_reactive_ != nullptr) power_reactive_->publish_state(reactive);
                        if (power_apparent_ != nullptr) power_apparent_->publish_state(apparent);
                        if (fwd_act_energy_ != nullptr) fwd_act_energy_->publish_state(values[6]);
                        if (voltage_ != nullptr) voltage_->publish_state(values[7]);
                        if (current_ != nullptr) current_->publish_state(values[8]);
                        if (freq_ != nullptr) freq_->publish_state(values[9]);
                        if (power_factor_ != nullptr) power_factor_->publish_state(values[10]*100);
                        if (heating_power_ != nullptr) heating_power_->publish_state(heating);
                    }
                    else ESP_LOGD("UART", "Błąd CRC lub nieprawidłowe dane");
                }

                // Wyczyść bufor
                buffer_.clear();
            } else {
                buffer_ += static_cast<char>(data);
            }
        }

        if (buffer_.length() > MAX_BUFFER_SIZE) {
            ESP_LOGW("UART", "Bufor przekroczył maksymalny rozmiar, czyszczenie...");
            buffer_.clear();
        }
    }
}

void HeatPumpLogger::dump_config() {
    ESP_LOGCONFIG(TAG, "Heat Pump Logger");
}

// Implementacja funkcji split_buffer_to_floats
std::vector<float> HeatPumpLogger::split_buffer_to_floats(const String &buffer, char delimiter) {
    std::vector<float> values;
    int start = 0;
    int end = buffer.indexOf(delimiter);

    while (end != -1) {
        String part = buffer.substring(start, end);  // Wyodrębnij część między separatorami
        if (part.length() > 0) {
            float value = part.toFloat();  // Konwersja String na float
            values.push_back(value);
        }
        start = end + 1;  // Przesuń start na pozycję za ostatnim separatorem
        end = buffer.indexOf(delimiter, start);  // Znajdź kolejny separator
    }

    // Dodaj ostatnią część (jeśli istnieje)
    if (start < buffer.length()) {  // Użyj length() zamiast size()
        String part = buffer.substring(start);
        if (part.length() > 0) {
            float value = part.toFloat();  // Konwersja String na float
            values.push_back(value);
        }
    }

    return values;
}

}  // namespace heat_pump_logger
}  // namespace esphome